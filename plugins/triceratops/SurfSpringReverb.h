// SurfSpringReverb.h
// -----------------------------------------------------------------------------
// A 1960s-style "drippy" spring reverb (think Fender 6G15 / Accutronics Type 4
// tank driven hard for surf). Header-only, no dependencies beyond <cmath>/<vector>,
// so it drops into JUCE, iPlug2, VST3 SDK, etc.
//
// How the sound is made
//   * Each tank = two springs of slightly different length, summed.
//   * Each spring = delay line in a feedback loop containing a long chain of
//     first-order allpass filters. This is the classic "dispersion" model of a
//     real spring: highs race ahead of lows, producing the chirpy "pew / drip"
//     on every transient. The DRIP knob sets how strong that chirp is.
//   * Input DWELL knob = drive into a soft clipper (the driver-tube push that
//     gives surf reverb its splashy, slightly crunchy edge).
//   * SPLASH modulates the spring delay from an envelope follower, giving the
//     "boing" when you hit it hard, plus a tiny slow wobble for realism.
//   * Loop low-pass (TONE) and high-pass keep it band-limited like a real tank
//     (nothing much below ~100 Hz or above ~5 kHz comes back).
//
// Usage (JUCE example)
//   // member:   Surf::SpringReverb spring;
//   // prepareToPlay(sr, block):  spring.prepare(sr);
//   // parameters (any time, from the audio thread / at block start):
//   //     spring.setDwell(0.6f); spring.setDecay(0.6f); spring.setDrip(0.7f);
//   //     spring.setTone(0.6f);  spring.setSplash(0.4f); spring.setMix(0.35f);
//   // processBlock:
//   //     auto* l = buffer.getWritePointer(0);
//   //     auto* r = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : l;
//   //     for (int i = 0; i < n; ++i) spring.process(l[i], r[i], l[i], r[i]);
//   //   (mono: pass the same sample as both inputs and use only outL)
//   // Use setMix(1.0f) if you want a wet-only signal for a send/return.
// -----------------------------------------------------------------------------
#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

namespace Surf
{
    constexpr float kPi = 3.14159265358979f;

    // C++11/14-safe clamp (takes args by value, so no C++17 needed)
    template <typename T>
    inline T clampT(T v, T lo, T hi) { return v < lo ? lo : (v > hi ? hi : v); }

    // ---- small building blocks ----------------------------------------------

    struct OnePoleLP
    {
        float z = 0.f, a = 0.f;
        void setFreq(float f, float sr) { a = 1.f - std::exp(-2.f * kPi * f / sr); }
        float process(float x) { z += a * (x - z); return z; }
        void reset() { z = 0.f; }
    };

    struct OnePoleHP
    {
        OnePoleLP lp;
        void setFreq(float f, float sr) { lp.setFreq(f, sr); }
        float process(float x) { return x - lp.process(x); }
        void reset() { lp.reset(); }
    };

    // y[n] = a*x[n] + x[n-1] - a*y[n-1]   ->   H(z) = (a + z^-1) / (1 + a z^-1)
    // a < 0 delays lows more than highs (highs arrive first = descending "pew").
    struct Allpass1
    {
        float x1 = 0.f, y1 = 0.f;
        float process(float x, float a)
        {
            float y = a * x + x1 - a * y1;
            x1 = x; y1 = y;
            return y;
        }
        void reset() { x1 = y1 = 0.f; }
    };

    struct DelayLine
    {
        std::vector<float> buf;
        int mask = 0, w = 0;

        void init(int maxSamples)
        {
            int n = 1;
            while (n < maxSamples + 4) n <<= 1;
            buf.assign((size_t) n, 0.f);
            mask = n - 1; w = 0;
        }
        void reset() { std::fill(buf.begin(), buf.end(), 0.f); w = 0; }
        void write(float x) { buf[(size_t) w] = x; w = (w + 1) & mask; }
        // delay in samples (>= 1), linear interpolation
        float read(float d) const
        {
            float rp = (float) w - d + (float) (mask + 1);
            int   i  = (int) rp;
            float f  = rp - (float) i;
            float a  = buf[(size_t) (i & mask)];
            float b  = buf[(size_t) ((i + 1) & mask)];
            return a + f * (b - a);
        }
    };

    // ---- one spring ----------------------------------------------------------

    class Spring
    {
    public:
        void prepare(double sampleRate, float baseDelayMs, float lfoRateHz, float lfoPhase)
        {
            sr = (float) sampleRate;
            baseDelayMs_ = baseDelayMs;
            delay.init((int) (0.12f * sr));
            numStages = clampT<int>((int) std::lround(40.f * sr / 44100.f), 8, (int) kMaxStages);
            for (auto& s : stages) s.reset();
            lp.reset(); hp.reset(); delay.reset();
            hp.setFreq(110.f, sr);
            lfoInc = lfoRateHz / sr;
            lfoPh  = lfoPhase;
            env = 0.f;
            updateLength();
        }

        void setParams(float feedbackGain, float allpassA, float lpFreq, float splashAmt)
        {
            fb = feedbackGain;
            a  = allpassA;
            lp.setFreq(lpFreq, sr);
            splash = splashAmt;
        }

        // Average group delay of a 1st-order allpass over 0..pi is exactly 1
        // sample, so the chain adds ~numStages samples to the loop.
        float loopSamples() const { return baseDelayMs_ * 0.001f * sr; }
        int   stageCount()  const { return numStages; }

        float process(float in)
        {
            // envelope follower on the incoming signal -> spring "boing"
            float rect = std::fabs(in);
            env += (rect > env ? 0.02f : 0.0004f) * (rect - env);

            lfoPh += lfoInc; if (lfoPh >= 1.f) lfoPh -= 1.f;
            float lfo = std::sin(2.f * kPi * lfoPh);

            float mod = splash * (std::min(env, 1.f) * 10.f) + 0.6f * lfo;
            float d   = std::max(lenSamples + mod, 4.f);

            float out = delay.read(d);               // what comes back from the spring
            float fbk = hp.process(lp.process(out)) * fb;

            float x = in + fbk;
            for (int i = 0; i < numStages; ++i)      // dispersion = the drip
                x = stages[(size_t) i].process(x, a);

            // gentle limiter in the loop keeps it musical (and stable)
            x = x / (1.f + 0.15f * std::fabs(x));
            delay.write(x);
            return out;
        }

        void updateLength()
        {
            lenSamples = std::max(baseDelayMs_ * 0.001f * sr - (float) numStages, 16.f);
        }

    private:
        static constexpr int kMaxStages = 160;
        float sr = 44100.f, baseDelayMs_ = 33.f;
        float fb = 0.f, a = -0.6f, splash = 0.f;
        float lenSamples = 1000.f, env = 0.f, lfoPh = 0.f, lfoInc = 0.f;
        int   numStages = 40;
        DelayLine delay;
        OnePoleLP lp;
        OnePoleHP hp;
        std::array<Allpass1, kMaxStages> stages {};
    };

    // ---- stereo reverb -------------------------------------------------------

    class SpringReverb
    {
    public:
        void prepare(double sampleRate)
        {
            sr = (float) sampleRate;
            inHP[0].setFreq(120.f, sr);       inHP[1].setFreq(120.f, sr);
            outBright[0].setFreq(1800.f, sr); outBright[1].setFreq(1800.f, sr);
            reset();
        }

        void reset()
        {
            // Two springs per channel, slightly different lengths (like a real 2-spring tank)
            springs[0].prepare((double) sr, 33.1f, 0.31f, 0.00f);
            springs[1].prepare((double) sr, 41.7f, 0.23f, 0.25f);
            springs[2].prepare((double) sr, 35.3f, 0.27f, 0.50f);
            springs[3].prepare((double) sr, 44.9f, 0.19f, 0.75f);
            for (auto& f : inHP)      f.reset();
            for (auto& f : outBright) f.reset();
            updateAll();
        }

        // all parameters 0..1
        void setDwell(float v)  { dwell  = clamp01(v); }                 // drive into the tank
        void setDecay(float v)  { decay  = clamp01(v); updateAll(); }    // ~0.6 s .. ~5 s
        void setDrip(float v)   { drip   = clamp01(v); updateAll(); }    // chirp amount
        void setTone(float v)   { tone   = clamp01(v); updateAll(); }    // dark .. bright
        void setSplash(float v) { splash = clamp01(v); updateAll(); }    // boing on hits
        void setMix(float v)    { mix    = clamp01(v); }

        void process(float inL, float inR, float& outL, float& outR)
        {
            // mono-sum into the tank (like the amp's single driver)
            float x = 0.5f * (inL + inR);
            x = inHP[0].process(x);

            // dwell: drive into soft clipper
            float g = 1.f + dwell * 7.f;
            x = std::tanh(g * x) / std::sqrt(g);

            float wetL = 0.5f * (springs[0].process(x) + springs[1].process(x));
            float wetR = 0.5f * (springs[2].process(x) + springs[3].process(x));

            // surf "sizzle": add a bit of the highs back in
            float bright = 0.2f + 0.9f * tone;
            wetL += bright * (wetL - outBright[0].process(wetL)) * 0.5f;
            wetR += bright * (wetR - outBright[1].process(wetR)) * 0.5f;

            const float wetGain = 1.6f;
            outL = inL * (1.f - mix) + wetL * wetGain * mix;
            outR = inR * (1.f - mix) + wetR * wetGain * mix;
        }

    private:
        static float clamp01(float v) { return clampT(v, 0.f, 1.f); }

        void updateAll()
        {
            if (sr <= 0.f) return;
            float rt60   = 0.6f + decay * 4.4f;                  // seconds
            float a      = -(0.35f + 0.50f * drip);              // more negative = more chirp
            float lpFreq = 2000.f + tone * 4500.f;               // loop damping
            float splashAmt = splash * 0.8f;

            for (auto& s : springs)
            {
                float loop = s.loopSamples();
                float g = std::pow(10.f, -3.f * loop / (rt60 * sr));  // RT60 -> loop gain
                s.setParams(std::min(g, 0.985f), a, lpFreq, splashAmt);
            }
        }

        float sr = 0.f;
        float dwell = 0.5f, decay = 0.55f, drip = 0.7f, tone = 0.6f, splash = 0.4f, mix = 0.3f;
        Spring springs[4];
        OnePoleHP inHP[2];
        OnePoleLP outBright[2];
    };
}
