// SmoothVerb.h  -  header-only stereo reverb, C++11 or newer, no dependencies.
//
// Design (why it doesn't sound metallic):
//   1. 8-line Feedback Delay Network with a Householder (orthogonal) mixing
//      matrix, so energy is conserved and echo density builds quickly.
//   2. Delay lengths are mutually-prime and not harmonically related, so no
//      comb-filter resonances line up into a "ring".
//   3. Every delay line is slowly modulated (different rate and phase per
//      line). This smears any remaining modal peaks over time.
//   4. Four-stage allpass input diffusion per channel removes the discrete
//      early "slap" echoes that make small digital reverbs sound pingy.
//   5. Per-line one-pole low-pass in the loop gives natural frequency
//      dependent decay (highs die faster, like real rooms/air).
//   6. Per-line gain is derived from the RT60 formula, so "decay" is
//      really seconds and stays stable at any size setting.
//
// ---------------------------------------------------------------------------
// USAGE (any framework)
//
//   #include "SmoothVerb.h"
//   SmoothVerb verb;                              // member of your processor
//
//   void prepare(double sampleRate) { verb.prepare(sampleRate); }
//
//   void process(float* L, float* R, int numSamples) {   // in place
//       verb.process(L, R, numSamples);
//   }
//
//   // Set from your parameters (any thread-safe path; values are smoothed):
//   verb.setDecay(2.5f);        // RT60 in seconds        0.1 .. 30
//   verb.setSize(1.0f);         // room size scale        0.5 .. 2.0
//   verb.setDamping(6000.f);    // HF damping cutoff, Hz  500 .. 20000
//   verb.setDiffusion(0.7f);    // input diffusion        0 .. 1
//   verb.setModulation(0.5f);   // anti-ring modulation   0 .. 1
//   verb.setPreDelay(15.f);     // milliseconds           0 .. 200
//   verb.setWidth(1.0f);        // stereo width           0 .. 1
//   verb.setMix(0.25f);         // dry/wet                0 .. 1
//
// JUCE example (AudioProcessor):
//
//   void prepareToPlay(double sr, int) override { verb.prepare(sr); }
//   void processBlock(juce::AudioBuffer<float>& b, juce::MidiBuffer&) override {
//       if (b.getNumChannels() >= 2)
//           verb.process(b.getWritePointer(0), b.getWritePointer(1), b.getNumSamples());
//       else
//           verb.processMono(b.getWritePointer(0), b.getNumSamples());
//   }
//
// Set the "Mix" to 1.0 and use it as a send effect if you prefer.
// ---------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

class SmoothVerb
{
public:
    SmoothVerb() { prepare(44100.0); }

    // ---- lifecycle ---------------------------------------------------------
    void prepare(double sampleRate)
    {
        sr = (float) std::max(8000.0, sampleRate);
        const float k = sr / 48000.0f;               // scale from 48 kHz tuning

        for (int i = 0; i < N; ++i)
        {
            baseLen[i] = fdnLengths()[i] * k;
            fdn[i].init((int) (baseLen[i] * kMaxSize()) + (int) (0.004f * sr) + 8);
            lfoPhase[i] = lfoPhases()[i];
            lfoInc[i]   = lfoRates()[i] / sr;
        }
        for (int c = 0; c < 2; ++c)
        {
            preDelay[c].init((int) (0.2f * sr) + 8);
            for (int s = 0; s < kAP; ++s)
                ap[c][s].init((int) (apLengths(c)[s] * k) + 8);
        }
        modRangeSamples = 0.0015f * sr;              // +/-1.5 ms at full depth
        reset();

        // snap smoothed values to targets
        sizeS = sizeT; dampS = dampT; mixS = mixT; widthS = widthT;
        preS = preT * 0.001f * sr; diffS = diffT; modS = modT;
        updateGains(true);
    }

    void reset()
    {
        for (int i = 0; i < N; ++i) { fdn[i].clear(); lp[i] = 0.f; }
        for (int c = 0; c < 2; ++c)
        {
            preDelay[c].clear();
            for (int s = 0; s < kAP; ++s) ap[c][s].clear();
            dcX[c] = dcY[c] = 0.f;
        }
    }

    // ---- parameters (call from any thread; audio thread reads them) ---------
    void setDecay     (float seconds) { decayT = clampf(seconds, 0.1f, 30.f); }
    void setSize      (float scale)   { sizeT  = clampf(scale, 0.5f, kMaxSize()); }
    void setDamping   (float hz)      { dampT  = clampf(hz, 500.f, 20000.f); }
    void setDiffusion (float amount)  { diffT  = clampf(amount, 0.f, 1.f); }
    void setModulation(float amount)  { modT   = clampf(amount, 0.f, 1.f); }
    void setPreDelay  (float ms)      { preT   = clampf(ms, 0.f, 200.f); }
    void setWidth     (float amount)  { widthT = clampf(amount, 0.f, 1.f); }
    void setMix       (float amount)  { mixT   = clampf(amount, 0.f, 1.f); }

    // ---- processing -----------------------------------------------------------
    // In-place stereo.
    void process(float* left, float* right, int numSamples)
    {
        updateGains(false);
        const float sm = 0.002f;    

        for (int n = 0; n < numSamples; ++n)
        {
            sizeS  += sm * (sizeT  - sizeS);
            dampS  += sm * (dampT  - dampS);
            mixS   += sm * (mixT   - mixS);
            widthS += sm * (widthT - widthS);
            diffS  += sm * (diffT  - diffS);
            modS   += sm * (modT   - modS);
            preS   += sm * (preT * 0.001f * sr - preS);

            const float dryL = left[n] * 2;
            const float dryR = right[n] * 2;

            // pre-delay
            preDelay[0].push(dryL);
            preDelay[1].push(dryR);
            float inL = preDelay[0].read(preS);
            float inR = preDelay[1].read(preS);

            // input diffusion (allpass chains)
            const float g = 0.15f + 0.5f * diffS;     // 0.15 .. 0.65
            for (int s = 0; s < kAP; ++s)
            {
                inL = ap[0][s].process(inL, g);
                inR = ap[1][s].process(inR, g);
            }

            const float wet = fdnTick(inL, inR, wetL, wetR);

            // width (mid/side) + DC block
            const float mid  = 0.5f * (wetL + wetR);
            const float side = 0.5f * (wetL - wetR) * widthS;
            float oL = mid + side, oR = mid - side;
            oL = dcBlock(oL, 0);
            oR = dcBlock(oR, 1);
            (void) wet;

            left[n]  = dryL * (1.f - mixS) + oL * mixS;
            right[n] = dryR * (1.f - mixS) + oR * mixS;
        }
    }

    // In-place mono (output stays mono: the stereo wet is summed).
    void processMono(float* data, int numSamples)
    {
        std::vector<float> r(data, data + numSamples);
        process(data, r.data(), numSamples);
        for (int n = 0; n < numSamples; ++n) data[n] = 0.5f * (data[n] + r[n]);
    }

private:
    // ------------------------------------------------------------------------
    static const int   N = 8;
    static const int   kAP = 4;
    static constexpr float kMaxSize() { return 2.0f; }

    // Tuning tables. Function-local statics keep this header-only and valid
    // in C++11/14/17 (no out-of-class definitions needed, no ODR problems).
    // Delay lengths: mutually prime, ~23-54 ms at 48 kHz.
    static const float* fdnLengths()
    { static const float v[N] = { 1087, 1283, 1493, 1709, 1931, 2153, 2377, 2609 }; return v; }
    // Allpass lengths (different for L/R for decorrelation).
    static const float* apLengths(int c)
    {
        static const float v[2][kAP] = { { 142, 107, 379, 277 }, { 151, 113, 397, 263 } };
        return v[c];
    }
    // Slow, unrelated LFO rates (Hz) and starting phases.
    static const float* lfoRates()
    { static const float v[N] = { 0.61f, 0.73f, 0.89f, 1.03f, 0.47f, 0.97f, 0.67f, 0.83f }; return v; }
    static const float* lfoPhases()
    { static const float v[N] = { 0.00f, 0.37f, 0.71f, 0.19f, 0.53f, 0.89f, 0.11f, 0.63f }; return v; }

    struct DelayLine
    {
        std::vector<float> buf; int mask = 0, w = 0;
        void init(int maxSamples)
        {
            int n = 1; while (n < maxSamples + 4) n <<= 1;
            buf.assign((size_t) n, 0.f); mask = n - 1; w = 0;
        }
        void clear() { std::fill(buf.begin(), buf.end(), 0.f); }
        inline void push(float x) { w = (w + 1) & mask; buf[(size_t) w] = x; }
        // delay in samples (fractional); 0 = most recently pushed sample
        inline float read(float d) const
        {
            const float p = (float) w - d;
            const int   i = (int) std::floor(p);
            const float f = p - (float) i;
            return buf[(size_t) (i & mask)] * (1.f - f) + buf[(size_t) ((i - 1) & mask)] * f;
        }
    };

    struct Allpass
    {
        DelayLine d; int len = 1;
        void init(int n) { d.init(n); len = n - 8; }
        void clear() { d.clear(); }
        inline float process(float x, float g)
        {
            const float z = d.read((float) len - 1.f);   // v[n-len]
            const float v = x + g * z;
            d.push(v);
            return z - g * v;
        }
    };

    static inline float clampf(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

    inline float dcBlock(float x, int c)
    {
        const float y = x - dcX[c] + 0.995f * dcY[c];
        dcX[c] = x; dcY[c] = y; return y;
    }

    void updateGains(bool snap)
    {
        // RT60 -> per-line feedback gain: g = 10^(-3 * delaySec / RT60)
        for (int i = 0; i < N; ++i)
        {
            const float delaySec = baseLen[i] * sizeT / sr;
            const float gt = std::pow(10.f, -3.f * delaySec / decayT);
            gainT[i] = gt;
            if (snap) gainS[i] = gt;
        }
    }

    inline float fdnTick(float inL, float inR, float& outL, float& outR)
    {
        const float a = std::exp(-6.2831853f * dampS / sr);   // one-pole LP coeff
        const float depth = modS * modRangeSamples;

        float x[N];
        float sum = 0.f;
        for (int i = 0; i < N; ++i)
        {
            gainS[i] += 0.002f * (gainT[i] - gainS[i]);

            lfoPhase[i] += lfoInc[i];
            if (lfoPhase[i] >= 1.f) lfoPhase[i] -= 1.f;
            const float m = std::sin(6.2831853f * lfoPhase[i]) * depth;

            float d = baseLen[i] * sizeS + m;
            d = std::max(d, 2.f);
            float v = fdn[i].read(d - 1.f);

            lp[i] = (1.f - a) * v + a * lp[i];          // HF damping in loop
            x[i]  = lp[i] * gainS[i];
            sum  += x[i];
        }

        // Householder: y = x - (2/N) * sum(x)   (orthogonal, lossless)
        const float h = (2.f / (float) N) * sum;
        for (int i = 0; i < N; ++i)
        {
            const float in = (i & 1) ? inR : inL;       // alternate channels
            fdn[i].push(x[i] - h + in * 0.5f);
        }

        // decorrelated stereo taps (orthogonal sign patterns)
        static const float sL[N] = { 1, -1,  1, -1,  1, -1,  1, -1 };
        static const float sR[N] = { 1,  1, -1, -1,  1,  1, -1, -1 };
        float l = 0.f, r = 0.f;
        for (int i = 0; i < N; ++i) { l += sL[i] * x[i]; r += sR[i] * x[i]; }
        outL = l * 0.35f;
        outR = r * 0.35f;
        return 0.f;
    }

    // ---- state ----------------------------------------------------------------
    float sr = 44100.f;
    DelayLine fdn[N];
    DelayLine preDelay[2];
    Allpass   ap[2][kAP];
    float baseLen[N] = {};
    float lp[N] = {}, gainS[N] = {}, gainT[N] = {};
    float lfoPhase[N] = {}, lfoInc[N] = {};
    float modRangeSamples = 0.f;
    float dcX[2] = {}, dcY[2] = {};
    float wetL = 0.f, wetR = 0.f;

    // targets
    float decayT = 2.5f, sizeT = 1.f, dampT = 6000.f, diffT = 0.7f,
          modT = 0.5f, preT = 15.f, widthT = 1.f, mixT = 0.25f;
    // smoothed
    float sizeS = 1.f, dampS = 6000.f, diffS = 0.7f, modS = 0.5f,
          preS = 0.f, widthS = 1.f, mixS = 0.25f;
};
