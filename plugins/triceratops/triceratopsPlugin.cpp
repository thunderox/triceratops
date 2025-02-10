
// ThunderOx Triceratops synth - By Nick S. Bailey <tb303@gmx.com>
// DISTRHO Plugin Framework (DPF) Copyright (C) 2012-2019 Filipe Coelho <falktx@falktx.com>

#include "DistrhoPlugin.hpp"
#include <string.h>
#include <string>
#include <iostream>
#include <vector> 
#include <math.h>
#include "synth.h"
#include "lfo.h"
#include "nix-echo.hpp"
#include "JCRev.h"
#include "eq.h"


using namespace std;

const int max_notes = 12;

START_NAMESPACE_DISTRHO

class triceratopsPlugin : public Plugin
{
	public:
		// Parameters
		float fParameters[kParameterCount+3];
		
		// Audio Struct
		struct audio_stereo
		{ 
			float left,right; 
		};
		float srate = getSampleRate();

		int midi_keys[128];
			 
		int current_synth = 0;
		int old_synth = 0;
		
		float* pitch_bend = new float();
		float* channel_after_touch = new float();
	
		synth synths[max_notes];
		params* synth_params;

		nixecho* echo;
		noise* nixnoise;
		JCRev* reverb[16];
		


		uint32_t buffer_frame;
		
		// Set Up LFOs
	
		LFO* lfo1 = new LFO(srate);	
		LFO* lfo2 = new LFO(srate);
		LFO* lfo3 = new LFO(srate);
	
		float* lfo1_out;
		float* lfo2_out;
		float* lfo3_out;
			
		int lfo1_count = 0;
		int lfo2_count = 0;
		int lfo3_count = 0;
		
		float lfo1_rand;
		float lfo2_rand;
		float lfo3_rand;
		

			
		// triceratops Audio Buffer
		vector <audio_stereo> audio_buffer; 
					
		triceratopsPlugin() : Plugin(kParameterCount, 0, 0)
		{

			srate = getSampleRate();

			audio_buffer.resize(srate*5);


			
			// Initialise LFOs
		
			lfo1_out = (float*)malloc(sizeof(float)*4096);	
			lfo2_out = (float*)malloc(sizeof(float)*4096);
			lfo3_out = (float*)malloc(sizeof(float)*4096);
		
			memset( lfo1_out, 0, sizeof(float)*4096 );
			memset( lfo2_out, 0, sizeof(float)*4096 );
			memset( lfo3_out, 0, sizeof(float)*4096 );
			
			// Initialise Echo
		
			echo = new nixecho();
			echo->set_sample_rate(srate);
		
			//Initialize Noise
		
			nixnoise = new noise();
					
			lfo1_rand = nixnoise->tick();
			lfo2_rand = nixnoise->tick();
			lfo3_rand = nixnoise->tick();
		
			// -----------------
		

			buffer_frame = 0;
			
			for (int x=0; x<16; ++x)
			{
				reverb[x] = new JCRev((float)x/3,srate);
				reverb[x]->clear();
				reverb[x]->setEffectMix(1.0);
			}
			
			for (int x=0; x<128; x++) { midi_keys[x] = -1; }
			
			for (int x=0; x<max_notes; x++)
			{
				synths[x].rate = srate;
				synths[x].lfo1_out = &lfo1_out;			
				synths[x].lfo2_out = &lfo2_out;	
				synths[x].lfo3_out = &lfo3_out;
				synths[x].pitch_bend = pitch_bend;
				synths[x].channel_after_touch = channel_after_touch;	
				synths[x].init_oscillators(srate);
	
			}	
			
			for (int x=0; x<128; x++) { midi_keys[x] = -1; }
			
			// clear all parameters
			std::memset(fParameters, 0, sizeof(float)*kParameterCount);

			// we can know buffer-size right at the start
			fParameters[kParameterBufferSize] = getBufferSize();

		}

		const char* getDescription() const override
		{
			return "Triceratops is a subtractive analogue style three oscillator synthesizer instrument.";
		}
		
		const char* getHomePage() const override
		{
			return "https://thunderox.com/synths/triceratops";
		}

		const char* getLabel() const
		{
			return "ThunderOx Triceratops";
		}

		const char* getMaker() const
		{
			return "Nick S. Bailey";
		}

		const char* getLicense() const
		{
			return "ISC";
		}

		//Get the plugin version, in hexadecimal.
    
		uint32_t getVersion() const override
		{
			return d_version(0, 7, 0);
		}

		int64_t getUniqueId() const
		{
			return d_cconst('T', 'O', 't', 'r');
		}

		void set_param (Parameter& param, const char* name, int index, float min, float max, float def)
		{	
			param.name = name;
			param.symbol = name;
			param.ranges.min = min;
			param.ranges.max = max;
			param.ranges.def = def;
			param.hints = kParameterIsAutomatable;
			fParameters[index] = def; 
			
		}

		void initParameter(uint32_t index, Parameter& parameter) override
		{
			switch (index)
			{

				case TRICERATOPS_MASTER_VOLUME:
					set_param (parameter, "TRICERATOPS_MASTER_VOLUME", index, 0.0f, 2.0f, 0.6f);
					break;
				case TRICERATOPS_MASTER_TUNE:
					set_param (parameter, "TRICERATOPS_MASTER_TUNE", index, 7.0f, -7.0f, 0.0f);
					break;
				case TRICERATOPS_AMP_DRIVE:
					set_param (parameter, "TRICERATOPS_AMP_DRIVE", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_FILTER_MODE:
					set_param (parameter, "TRICERATOPS_FILTER_MODE", index, 0.0f, 5.0f, 1.0f);
					break;
				case TRICERATOPS_FILTER_FREQUENCY:
					set_param (parameter, "TRICERATOPS_FILTER_FREQUENCY", index, 0.0f, 1.0f, 0.5f);
					break;
				case TRICERATOPS_FILTER_RESONANCE:
					set_param (parameter, "TRICERATOPS_FILTER_RESONANCE", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_FILTER_KEY_FOLLOW:
					set_param (parameter, "TRICERATOPS_FILTER_KEY_FOLLOW", index, -1.0f, 1.0f, 0.0f);
					break;
				
					
				// -- OSCILATOR ONE ------------------------------------------------------
					
				case TRICERATOPS_OSC1_ACTIVE:
					set_param (parameter, "TRICERATOPS_OSC1_ACTIVE", index, -1.0f, 1.0f, 1.0f);
					break;
				case TRICERATOPS_OSC1_VOLUME:
					set_param (parameter, "TRICERATOPS_OSC1_VOLUME", index, 0.0f, 1.0f, 0.8f);
					break;
				case TRICERATOPS_PANIC:
					set_param (parameter, "TRICERATOPS_PANIC", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_OSC1_PAN:
					set_param (parameter, "TRICERATOPS_OSC1_PAN", index, 0.0f, 1.0f, 0.5f);
					break;
				case TRICERATOPS_OSC1_PULSEWIDTH:
					set_param (parameter, "TRICERATOPS_OSC1_PULSEWIDTH", index, -0.5f, 0.5f, 0.0f);
					break;
				case TRICERATOPS_OSC1_WAVE:
					set_param (parameter, "TRICERATOPS_OSC1_WAVE", index, 0.0f, 5.0f, 0.0f);
					break;
				case TRICERATOPS_OSC1_OCTAVE:
					set_param (parameter, "TRICERATOPS_OSC1_OCTAVE", index, -5.0f, 5.0f, -2.0f);
					break;		
				case TRICERATOPS_OSC1_DETUNE:
					set_param (parameter, "TRICERATOPS_OSC1_DETUNE", index, -7.0f, 7.0f, -0.0f);
					break;
				case TRICERATOPS_OSC1_DETUNE_CENTRE:
					set_param (parameter, "TRICERATOPS_OSC1_DETUNE_CENTRE", index, 0.0f, 1.0f, -0.0f);
					break;
				case TRICERATOPS_OSC1_INERTIA:
					set_param (parameter, "TRICERATOPS_OSC1_INERTIA", index, 0.0f, 1.0f, -0.0f);
					break;

				// -- OSCILATOR TWO ------------------------------------------------------
					
				case TRICERATOPS_OSC2_ACTIVE:
					set_param (parameter, "TRICERATOPS_OSC2_ACTIVE", index, -1.0f, 1.0f, 1.0f);
					break;
				case TRICERATOPS_OSC2_VOLUME:
					set_param (parameter, "TRICERATOPS_OSC2_VOLUME", index, 0.0f, 1.0f, 0.8f);
					break;
				case TRICERATOPS_OSC2_PAN:
					set_param (parameter, "TRICERATOPS_OSC2_PAN", index, 0.0f, 1.0f, 0.5f);
					break;
				case TRICERATOPS_OSC2_PULSEWIDTH:
					set_param (parameter, "TRICERATOPS_OSC2_PULSEWIDTH", index, -0.5f, 0.5f, 0.0f);
					break;
				case TRICERATOPS_OSC2_WAVE:
					set_param (parameter, "TRICERATOPS_OSC2_WAVE", index, 0.0f, 5.0f, 0.0f);
					break;
				case TRICERATOPS_OSC2_OCTAVE:
					set_param (parameter, "TRICERATOPS_OSC2_OCTAVE", index, -5.0f, 5.0f, -2.0f);
					break;		
				case TRICERATOPS_OSC2_DETUNE:
					set_param (parameter, "TRICERATOPS_OSC2_DETUNE", index, -7.0f, 7.0f, -0.0f);
					break;
				case TRICERATOPS_OSC2_DETUNE_CENTRE:
					set_param (parameter, "TRICERATOPS_OSC2_DETUNE_CENTRE", index, 0.0f, 1.0f, -0.0f);
					break;
				case TRICERATOPS_OSC2_INERTIA:
					set_param (parameter, "TRICERATOPS_OSC2_INERTIA", index, 0.0f, 1.0f, -0.0f);
					break;


					
				// -- OSCILATOR THREE ----------------------------------------------------------------------
					
				case TRICERATOPS_OSC3_ACTIVE:
					set_param (parameter, "TRICERATOPS_OSC3_ACTIVE", index, -1.0f, 1.0f, 1.0f);
					break;
				case TRICERATOPS_OSC3_VOLUME:
					set_param (parameter, "TRICERATOPS_OSC3_VOLUME", index, 0.0f, 1.0f, 0.8f);
					break;
				case TRICERATOPS_OSC3_PAN:
					set_param (parameter, "TRICERATOPS_OSC3_PAN", index, 0.0f, 1.0f, 0.5f);
					break;
				case TRICERATOPS_OSC3_PULSEWIDTH:
					set_param (parameter, "TRICERATOPS_OSC3_PULSEWIDTH", index, -0.5f, 0.5f, 0.0f);
					break;
				case TRICERATOPS_OSC3_WAVE:
					set_param (parameter, "TRICERATOPS_OSC3_WAVE", index, 0.0f, 5.0f, 0.0f);
					break;
				case TRICERATOPS_OSC3_OCTAVE:
					set_param (parameter, "TRICERATOPS_OSC3_OCTAVE", index, -5.0f, 5.0f, -2.0f);
					break;		
				case TRICERATOPS_OSC3_DETUNE:
					set_param (parameter, "TRICERATOPS_OSC3_DETUNE", index, -7.0f, 7.0f, -0.0f);
					break;
				case TRICERATOPS_OSC3_DETUNE_CENTRE:
					set_param (parameter, "TRICERATOPS_OSC3_DETUNE_CENTRE", index, 0.0f, 1.0f, -0.0f);
					break;
				case TRICERATOPS_OSC3_INERTIA:
					set_param (parameter, "TRICERATOPS_OSC3_INERTIA", index, 0.0f, 1.0f, -0.0f);
					break;

										
				// UNISON ----------------------------------------------------------------------
					
				case TRICERATOPS_UNISON_ACTIVATE:
					set_param (parameter, "TRICERATOPS_UNISON_ACTIVATE", index, 0.0f, 1.0f, -0.0f);
					break;
				case TRICERATOPS_UNISON_ONE:
					set_param (parameter, "TRICERATOPS_UNISON_ONE", index, 0.0f, 4.0f, -0.0f);
					break;
				case TRICERATOPS_UNISON_TWO:
					set_param (parameter, "TRICERATOPS_UNISON_TWO", index, 0.0f, 4.0f, -0.0f);
					break;
				case TRICERATOPS_UNISON_THREE:
					set_param (parameter, "TRICERATOPS_UNISON_THREE", index, 0.0f, 4.0f, -0.0f);
					break;					
					
				// ADSR ONE ----------------------------------------------------------------------
					
				case TRICERATOPS_ADSR1_ATTACK:
					set_param (parameter, "TRICERATOPS_ADSR1_ATTACK", index, 0.0f, 1.0f, 1.0f);
					break;
				case TRICERATOPS_ADSR1_DECAY:
					set_param (parameter, "TRICERATOPS_ADSR1_DECAY", index, 0.0f, 1.0f, 0.75f);
					break;
				case TRICERATOPS_ADSR1_SUSTAIN:
					set_param (parameter, "TRICERATOPS_ADSR1_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR1_RELEASE:
					set_param (parameter, "TRICERATOPS_ADSR1_RELEASE", index, 0.0f, 1.0f, 0.7f);
					break;
					
				// ADSR ONE Routing ----------------------------------------------------------------------
					
				case TRICERATOPS_ADSR1_ROUTE_ONE:
					set_param (parameter, "TRICERATOPS_ADSR1_ROUTE_ONE", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR1_ROUTE_ONE_DEST:
					set_param (parameter, "TRICERATOPS_ADSR1_ROUTE_ONE_DEST", index, 0.0f, 14.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR1_ROUTE_TWO:
					set_param (parameter, "TRICERATOPS_ADSR1_ROUTE_TWO", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR1_ROUTE_TWO_DEST:
					set_param (parameter, "TRICERATOPS_ADSR1_ROUTE_TWO_DEST", index, 0.0f, 1.0f, 0.0f);
					break;

						
				// ADSR TWO ----------------------------------------------------------------------
					
				case TRICERATOPS_ADSR2_ATTACK:
					set_param (parameter, "TRICERATOPS_ADSR2_ATTACK", index, 0.0f, 1.0f, 1.0f);
					break;
				case TRICERATOPS_ADSR2_DECAY:
					set_param (parameter, "TRICERATOPS_ADSR2_DECAY", index, 0.0f, 1.0f, 0.75f);
					break;
				case TRICERATOPS_ADSR2_SUSTAIN:
					set_param (parameter, "TRICERATOPS_ADSR2_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR2_RELEASE:
					set_param (parameter, "TRICERATOPS_ADSR2_RELEASE", index, 0.0f, 1.0f, 0.3f);
					break;
					
				// ADSR TWO Routing ----------------------------------------------------------------------
					
				case TRICERATOPS_ADSR2_ROUTE_ONE:
					set_param (parameter, "TRICERATOPS_ADSR2_ROUTE_ONE", index, 0.0f, 1.0f, 0.25f);
					break;
				case TRICERATOPS_ADSR2_ROUTE_ONE_DEST:
					set_param (parameter, "TRICERATOPS_ADSR2_ROUTE_ONE_DEST", index, 0.0f, 14.0f, 1.0f);
					break;
				case TRICERATOPS_ADSR2_ROUTE_TWO:
					set_param (parameter, "TRICERATOPS_ADSR2_ROUTE_TWO", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR2_ROUTE_TWO_DEST:
					set_param (parameter, "TRICERATOPS_ADSR2_ROUTE_TWO_DEST", index, 0.0f, 14.0f, 0.0f);
					break;
					
				// ADSR THREE ----------------------------------------------------------------------
				
				case TRICERATOPS_ADSR3_ATTACK:
					set_param (parameter, "TRICERATOPS_ADSR3_ATTACK", index, 0.0f, 1.0f, 1.0f);
					break;
				case TRICERATOPS_ADSR3_DECAY:
					set_param (parameter, "TRICERATOPS_ADSR3_DECAY", index, 0.0f, 1.0f, 0.75f);
					break;
				case TRICERATOPS_ADSR3_SUSTAIN:
					set_param (parameter, "TRICERATOPS_ADSR3_SUSTAIN", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR3_RELEASE:
					set_param (parameter, "TRICERATOPS_ADSR3_RELEASE", index, 0.0f, 1.0f, 0.3f);
					break;
				case TRICERATOPS_ADSR3_LFO1_AMOUNT:
					set_param (parameter, "TRICERATOPS_ADSR3_LFO1_AMOUNT", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR3_LFO2_AMOUNT:
					set_param (parameter, "TRICERATOPS_ADSR3_LFO2_AMOUNT", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_ADSR3_LFO3_AMOUNT:
					set_param (parameter, "TRICERATOPS_ADSR3_LFO3_AMOUNT", index, 0.0f, 1.0f, 0.0f);
					break;
					
					
				// LFO ONE ----------------------------------------------------------------------
					
				case TRICERATOPS_LFO1_RETRIG:
					set_param (parameter, "TRICERATOPS_LFO1_RETRIG", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO1_SPEED:
					set_param (parameter, "TRICERATOPS_LFO1_SPEED", index, 0.0f, 680.0f, 120.0f);
					break;
				case TRICERATOPS_LFO1_WAVE:
					set_param (parameter, "TRICERATOPS_LFO1_WAVE", index, 0.0f, 6.0f, 0.0f);
					break;
				case TRICERATOPS_LFO1_OSC1_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO1_OSC1_AMOUNT", index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO1_OSC2_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO1_OSC2_AMOUNT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO1_OSC3_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO1_OSC3_AMOUNT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO1_FILTER:
					set_param (parameter, "TRICERATOPS_LFO1_FILTER" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO1_ROUTE_ONE:
					set_param (parameter, "TRICERATOPS_LFO1_ROUTE_ONE" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO1_ROUTE_ONE_DEST:
					set_param (parameter, "TRICERATOPS_LFO1_ROUTE_ONE_DEST" , index, 0.0f, 10.0f, 2.0f);
					break;
				case TRICERATOPS_LFO1_ROUTE_TWO:
					set_param (parameter, "TRICERATOPS_LFO1_ROUTE_TWO" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO1_ROUTE_TWO_DEST:
					set_param (parameter, "TRICERATOPS_LFO1_ROUTE_TWO_DEST" , index, 0.0f, 10.0f, 0.0f);
					break;
					
				// LFO TWO ----------------------------------------------------------------------
					
				case TRICERATOPS_LFO2_RETRIG:
					set_param (parameter, "TRICERATOPS_LFO2_RETRIG" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO2_SPEED:
					set_param (parameter, "TRICERATOPS_LFO2_SPEED" , index, 0.0f, 680.0f, 120.0f);
					break;
				case TRICERATOPS_LFO2_WAVE:
					set_param (parameter, "TRICERATOPS_LFO2_WAVE" , index, 0.0f, 6.0f, 0.0f);
					break;
				case TRICERATOPS_LFO2_OSC1_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO2_OSC1_AMOUNT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO2_OSC2_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO2_OSC2_AMOUNT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO2_OSC3_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO2_OSC3_AMOUNT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO2_FILTER:
					set_param (parameter, "TRICERATOPS_LFO2_FILTER" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO2_ROUTE_ONE:
					set_param (parameter, "TRICERATOPS_LFO2_ROUTE_ONE" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO2_ROUTE_ONE_DEST:
					set_param (parameter, "TRICERATOPS_LFO2_ROUTE_ONE_DEST" , index, 0.0f, 10.0f, 2.0f);
					break;
				case TRICERATOPS_LFO2_ROUTE_TWO:
					set_param (parameter, "TRICERATOPS_LFO2_ROUTE_TWO" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO2_ROUTE_TWO_DEST:
					set_param (parameter, "TRICERATOPS_LFO2_ROUTE_TWO_DEST" , index, 0.0f, 10.0f, 0.0f);
					break;
					
				// LFO THREE ----------------------------------------------------------------------
					
				case TRICERATOPS_LFO3_RETRIG:
					set_param (parameter, "TRICERATOPS_LFO3_RETRIG" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO3_SPEED:
					set_param (parameter, "TRICERATOPS_LFO3_SPEED" , index, 0.0f, 680.0f, 120.0f);
					break;				
				case TRICERATOPS_LFO3_WAVE:
					set_param (parameter, "TRICERATOPS_LFO3_WAVE" , index, 0.0f, 6.0f, 0.0f);
					break;			
				case TRICERATOPS_LFO3_OSC1_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO3_OSC1_AMOUNT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO3_OSC2_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO3_OSC2_AMOUNT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO3_OSC3_AMOUNT:
					set_param (parameter, "TRICERATOPS_LFO3_OSC3_AMOUNT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO3_FILTER:
					set_param (parameter, "TRICERATOPS_LFO3_FILTER" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO3_ROUTE_ONE:
					set_param (parameter, "TRICERATOPS_LFO3_ROUTE_ONE" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO3_ROUTE_ONE_DEST:
					set_param (parameter, "TRICERATOPS_LFO3_ROUTE_ONE_DEST" , index, 0.0f, 10.0f, 2.0f);
					break;	
				case TRICERATOPS_LFO3_ROUTE_TWO:
					set_param (parameter, "TRICERATOPS_LFO3_ROUTE_TWO" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_LFO3_ROUTE_TWO_DEST:
					set_param (parameter, "TRICERATOPS_LFO3_ROUTE_TWO_DEST" , index, 0.0f, 10.0f, 0.0f);
					break;

				// Modifiers and effects ----------------------------------------------------------------------
		
				case TRICERATOPS_MIDI_CHANNEL:
					set_param (parameter, "TRICERATOPS_MIDI_CHANNEL" , index, 0.0f, 14.0f, 1.0f);
					break;
				case TRICERATOPS_PITCH_BEND_RANGE:
					set_param (parameter, "TRICERATOPS_PITCH_BEND_RANGE" , index, 0.0f, 24.0f, 24.0f);
					break;
				case TRICERATOPS_LEGATO:
					set_param (parameter, "TRICERATOPS_LEGATO" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_SYNC:
					set_param (parameter, "TRICERATOPS_SYNC" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_WARMTH:
					set_param (parameter, "TRICERATOPS_WARMTH" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_MODIFIER_DIRT:
					set_param (parameter, "TRICERATOPS_MODIFIER_DIRT" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_PRESET_CATEGORY:
					parameter.name   = "Category";
					parameter.symbol = "preset_category";
					fParameters[TRICERATOPS_PRESET_CATEGORY] = parameter.ranges.def;
					break;
				case TRICERATOPS_FM:
					set_param (parameter, "TRICERATOPS_FM" , index, 0.0f, 1.0f, 0.0f);
					break;
				case TRICERATOPS_MODIFIER_STEREO_MODE:
					set_param (parameter, "TRICERATOPS_MODIFIER_STEREO_MODE" , index, 0.0f, 1.0f, 1.0f);
					break;
				case TRICERATOPS_MODIFIER_RING:
					set_param (parameter, "TRICERATOPS_MODIFIER_RING" , index, 0.0f, 1.0f, 0.0f);
					break;
				case kParametertriceratopsTime:
					set_param (parameter, "kParametertriceratopsTime" , index, 0.0f, 1.0f, 0.5f);
					break;		
					
				// ECHO ----------------------------------------------------------------------
					
				case TRICERATOPS_FX_ECHO_ACTIVE:
					set_param (parameter, "kParametertriceratopsTime" , index, 0.0f, 1.0f, 0.0f);
					break;		
				case TRICERATOPS_FX_ECHO_SPEED:
					set_param (parameter, "TRICERATOPS_FX_ECHO_SPEED" , index, 512.0f, 65536.0f, 4096.0f);
					break;					
				case TRICERATOPS_FX_ECHO_DECAY:
					set_param (parameter, "TRICERATOPS_FX_ECHO_DECAY" , index, 0.0f, 0.9f, 0.25f);
					break;		
				case TRICERATOPS_FX_ECHO_EQ_LOW:
					set_param (parameter, "TRICERATOPS_FX_ECHO_EQ_LOW" , index, 0.0f, 2.0f, 1.0f);
					break;		
				case TRICERATOPS_FX_ECHO_EQ_MID:
					set_param (parameter, "TRICERATOPS_FX_ECHO_EQ_MID" , index, 0.0f, 2.0f, 1.0f);
					break;	
				case TRICERATOPS_FX_ECHO_EQ_HIGH:
					set_param (parameter, "TRICERATOPS_FX_ECHO_EQ_HIGH" , index, 0.0f, 2.0f, 1.0f);
					break;			
					
				// REVERB ----------------------------------------------------------------------	
					
				case TRICERATOPS_FX_REVERB_ACTIVE:
					set_param (parameter, "TRICERATOPS_FX_REVERB_ACTIVE" , index, 0.0f, 1.0f, 0.0f);
					break;	
				case TRICERATOPS_FX_REVERB_DECAY:
					set_param (parameter, "TRICERATOPS_FX_REVERB_DECAY" , index, 0.0f, 15.0f, 4.0f);
					break;	
				case TRICERATOPS_FX_REVERB_MIX:
					set_param (parameter, "TRICERATOPS_FX_REVERB_MIX" , index, 0.0f, 0.8f, 0.0f);
					break;	

			}

		}

		/* --------------------------------------------------------------------------------------------------------
		* Internal data 
		Get the current value of a parameter.
 		The host may call this function from any context, including realtime processing.
		*/

		float getParameterValue(uint32_t index) const override
		{
			return fParameters[index];

		}

		/**
  		Change a parameter value.
 		The host may call this function from any context, including realtime processing.
		When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
		@note This function will only be called for parameter inputs.
		*/

		void setParameterValue(uint32_t index, float value) override
		{
			fParameters[index] = value;
			
		}

		void run(const float** inputs, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
		{

			// if (!fParameters[TRICERATOPS_DSP_RUN]) return; // Playing sound during preset load is a bad idea.	
			
			float* out_left = outputs[0];
			float* out_right = outputs[1];
			
			memset( out_left, 0, sizeof(double)*(frames*0.5) );
			memset( out_right, 0, sizeof(double)*(frames*0.5) );
		
			
			if (fParameters[TRICERATOPS_FX_ECHO_ACTIVE])
			{
					echo->eq->lg = fParameters[TRICERATOPS_FX_ECHO_EQ_LOW];
					echo->eq->mg = fParameters[TRICERATOPS_FX_ECHO_EQ_MID];
					echo->eq->hg = fParameters[TRICERATOPS_FX_ECHO_EQ_HIGH];
					echo->eq2->lg = fParameters[TRICERATOPS_FX_ECHO_EQ_LOW];
					echo->eq2->mg = fParameters[TRICERATOPS_FX_ECHO_EQ_MID];
					echo->eq2->hg = fParameters[TRICERATOPS_FX_ECHO_EQ_HIGH];
					
					echo->echo_decay = fParameters[TRICERATOPS_FX_ECHO_DECAY];
					echo->echo_speed = fParameters[TRICERATOPS_FX_ECHO_SPEED];
	
			}			
			
			
			// MIDI 
			
			int midi_channel = fParameters[TRICERATOPS_MIDI_CHANNEL] - 1;

		        for (uint32_t i=0; i<midiEventCount; ++i)
		        
		        // PITCHBEND
		        {
        	        	const uint8_t* ev = midiEvents[i].data;
		        
		        	// PITCHBEND
		        	
				if ((int)ev[0] == 0xE0 + midi_channel) 
				{
					pitch_bend[0] = ((float)(((int)ev[2] * 128) + (int)ev[1]) / 8192)-1;		
					
					// cout << pitch_bend[0] << endl;
				}
				
				if ((int)ev[0] == 0xD0 + midi_channel)
				{
					channel_after_touch[0] = ((float)((int)ev[1])/64);
				}
			
				// NOTE ON / OFF

		        	if ((int)ev[0]  == 0x90 + midi_channel && (int)ev[2] > 0)
				{
					
					for (int x=0; x<max_notes; ++x)
					{
						if (synths[x].env_amp_state  == synths[x].env_state_release
							&& synths[x].env_amp_level < 0.5
								|| synths[x].env_amp_state == synths[x].env_state_dormant)
								
						{
							current_synth = x;
						}
						
						if (fParameters[TRICERATOPS_LEGATO] == 1)
						{
							current_synth = 0;
						}
					}
					
						synths[current_synth].update_counter = -1;
	
						midi_keys[(int)ev[1]] = current_synth;
						synths[current_synth].midi_key = (int)ev[1];
						synths[current_synth].velocity =  (int)ev[2];	
						
						float key_frequency = (44100 / fastishP2F (125-(int)ev[1]));
						if (key_frequency > 18000) key_frequency = 18000;
						
						synths[current_synth].osc_frequency = key_frequency;	
						
						// IN LEGATO MODE ONLY USE INERTIA PORTAMENTO WHEN KEYS HELD DOWN	
						
						if (fParameters[TRICERATOPS_LEGATO] == 1)
						{
							if (synths[current_synth].env_amp_state == synths[current_synth].env_state_release
								|| synths[current_synth].env_amp_state == synths[current_synth].env_state_dormant)
							{
								synths[current_synth].inertia_one.value = key_frequency
									*fastishP2F (12 * fParameters[TRICERATOPS_OSC1_OCTAVE]) ;
								synths[current_synth].inertia_two.value = key_frequency 
									*fastishP2F (12 * fParameters[TRICERATOPS_OSC2_OCTAVE] );
								synths[current_synth].inertia_three.value = key_frequency 
									*fastishP2F (12 * fParameters[TRICERATOPS_OSC3_OCTAVE] );

							}
						}
						
						// IN LEGATO MODE DO NOT RESET ADSR STATES TO ATTACK MODE UNLESS PREVIOUSLY IN RELEASE/DORMANT

						if (fParameters[TRICERATOPS_LEGATO] == 1
							&& synths[current_synth].env_amp_state != synths[current_synth].env_state_release
								&& synths[current_synth].env_amp_state != synths[current_synth].env_state_dormant)
						{
	
							if (fParameters[TRICERATOPS_LFO1_RETRIG] == 1) { lfo1->phase = 0; lfo1_count = 0; }
							if (fParameters[TRICERATOPS_LFO2_RETRIG] == 1) { lfo2->phase = 0; lfo2_count = 0; }
							if (fParameters[TRICERATOPS_LFO3_RETRIG] == 1) { lfo3->phase = 0; lfo3_count = 0; }		
						}
						else
						{
							synths[current_synth].sinewave_osc[0]->phase = 0;
							synths[current_synth].sinewave_osc[1]->phase = 0;
							synths[current_synth].sinewave_osc[2]->phase = 0;
							synths[current_synth].sinewave_osc[3]->phase = 0;
							synths[current_synth].sinewave_osc[4]->phase = 0;
							synths[current_synth].sinewave_osc[5]->phase = 0;
							
							synths[current_synth].bf0_left = 0;
							synths[current_synth].bf1_left = 0;
							synths[current_synth].bf2_left = 0;
							synths[current_synth].bf3_left = 0;
							synths[current_synth].bf4_left = 0;

							synths[current_synth].bf0_right = 0;
							synths[current_synth].bf1_right = 0;
							synths[current_synth].bf2_right = 0;
							synths[current_synth].bf3_right = 0;
							synths[current_synth].bf4_right = 0;
							
							synths[current_synth].env_amp_level = 0;
							synths[current_synth].env_filter_level = 0;
							synths[current_synth].env_lfo_level = 0;

							synths[current_synth].env_amp_state = synths[current_synth].env_state_attack;
							synths[current_synth].env_filter_state = synths[current_synth].env_state_attack;
							synths[current_synth].env_lfo_state = synths[current_synth].env_state_attack;
							
							synths[current_synth].inertia_one.value = synths[old_synth].inertia_one.value; // copies frequency for inertia
							synths[current_synth].inertia_two.value = synths[old_synth].inertia_two.value;
							synths[current_synth].inertia_three.value = synths[old_synth].inertia_three.value;

							if (fParameters[TRICERATOPS_LFO1_RETRIG] == 1) { lfo1->phase = 0; lfo1_count = 0; }
							if (fParameters[TRICERATOPS_LFO2_RETRIG] == 1) { lfo2->phase = 0; lfo2_count = 0; }
							if (fParameters[TRICERATOPS_LFO3_RETRIG] == 1) { lfo3->phase = 0; lfo3_count = 0; }
		
							old_synth = current_synth;
						}		
					}
				
				// midi note depressed
				
				else if ((int)ev[0] == 0x80 + midi_channel || ((int)ev[0]  & 0xF0 == 0x90 && (int)ev[2] == 0))
				{
					for (int x=0; x <max_notes; x++)
					{
						if (synths[x].midi_key == (int)ev[1])
						{
							synths[x].midi_key = -1;
							synths[x].env_amp_state = synths[x].env_state_release;
							synths[x].env_filter_state = synths[x].env_state_release;
							synths[x].env_lfo_state = synths[x].env_state_release;
						}
					}
				}
					
			}
			
			lfo1->setRate( fParameters[TRICERATOPS_LFO1_SPEED]/60 );
			lfo2->setRate( fParameters[TRICERATOPS_LFO2_SPEED]/60 );
			lfo3->setRate( fParameters[TRICERATOPS_LFO3_SPEED]/60 );

			int lfo1_wave = fParameters[TRICERATOPS_LFO1_WAVE];
			int lfo2_wave = fParameters[TRICERATOPS_LFO2_WAVE];
			int lfo3_wave = fParameters[TRICERATOPS_LFO3_WAVE];
			
			switch (lfo1_wave)
			{
				case 0:
				lfo1->setWaveform( LFO::sawtooth);
				break;		

				case 1:
				lfo1->setWaveform( LFO::square);
				break;
				
				case 2:
				lfo1->setWaveform( LFO::sinus);
				break;

				case 5:
				lfo1->setWaveform( LFO::sawtooth);
				break;		

				case 6:
				lfo1->setWaveform( LFO::square);
				break;
			}
			
			switch (lfo2_wave)
			{
				case 0:
				lfo2->setWaveform( LFO::sawtooth);
				break;		

				case 1:
				lfo2->setWaveform( LFO::square);
				break;
				
				case 2:
				lfo2->setWaveform( LFO::sinus);
				break;

				case 5:
				lfo2->setWaveform( LFO::sawtooth);
				break;		

				case 6:
				lfo2->setWaveform( LFO::square);
				break;
			}
			
			switch (lfo3_wave)
			{
				case 0:
				lfo3->setWaveform( LFO::sawtooth);
				break;		

				case 1:
				lfo3->setWaveform( LFO::square);
				break;
				
				case 2:
				lfo3->setWaveform( LFO::sinus);
				break;

				case 5:
				lfo3->setWaveform( LFO::sawtooth);
				break;		

				case 6:
				lfo3->setWaveform( LFO::square);
				break;
			}
			
			int lfo1_speed = (srate*30) / fParameters[TRICERATOPS_LFO1_SPEED];
			int lfo2_speed = (srate*30) / fParameters[TRICERATOPS_LFO2_SPEED];
			int lfo3_speed = (srate*30) / fParameters[TRICERATOPS_LFO3_SPEED];
			
			for (int x=max_notes-1; x>-1; --x)	
			{
				if (synths[x].env_amp_state != synths[x].env_state_dormant)
				{
					synths[x].run(out_left,out_right,frames);
				}
			}
	
			for (uint32_t x=0; x<frames; ++x)
			{
				// DO LFO1

				if (lfo1_count == 0) {lfo1_rand = nixnoise->tick();  }

				if (lfo1_wave < 3) lfo1_out[x] = lfo1->tick();
				if (lfo1_wave == 5 || lfo1_wave == 6) lfo1_out[x] = 1 - lfo1->tick();

				if (lfo1_wave == 3)
				{
					float dummy = lfo1->tick(); // keeps other LFO waveforms on tempo;
					lfo1_out[x] = lfo1_rand;
				}

				if (lfo1_wave == 4)
				{
					float dummy = lfo1->tick(); // keeps other LFO waveforms on tempo;
					lfo1_out[x] = nixnoise->tick();
				}
				
				// DO LFO2

				if (lfo2_count == 0) lfo2_rand = nixnoise->tick();
				if (lfo2_wave == 5 || lfo2_wave == 6) lfo2_out[x] = 1 - lfo2->tick();

				if (lfo2_wave < 3) lfo2_out[x] = lfo2->tick();

				if (lfo2_wave == 3)
				{
					float dummy = lfo2->tick(); // keeps other LFO waveforms on tempo;
					lfo2_out[x] = lfo2_rand;
				}

				if (lfo2_wave == 4)
				{
					float dummy = lfo2->tick(); // keeps other LFO waveforms on tempo;
					lfo2_out[x] = nixnoise->tick();
				}

				// DO LFO3

				if (lfo3_count == 0) lfo3_rand = nixnoise->tick();

				if (lfo3_wave < 3) lfo3_out[x] = lfo3->tick();
				if (lfo3_wave == 5 || lfo3_wave == 6) lfo3_out[x] = 1 - lfo3->tick();

				if (lfo3_wave == 3)
				{
					float dummy = lfo3->tick(); // keeps other LFO waveforms on tempo;
					lfo3_out[x] = lfo3_rand;
				}

				if (lfo3_wave == 4)
				{
					float dummy = lfo3->tick(); // keeps other LFO waveforms on tempo;
					lfo3_out[x] = nixnoise->tick();
				}

				++lfo1_count;
				if (lfo1_count > lfo1_speed) lfo1_count = 0;

				++lfo2_count;
				if (lfo2_count > lfo2_speed) lfo2_count= 0;

				++lfo3_count;
				if (lfo3_count > lfo3_speed) lfo3_count = 0;
			
				if (isnan( out_left[x] )) out_left[x] = 0;
				if (isnan( out_right[x] )) out_right[x] = 0;
				
				if (fParameters[TRICERATOPS_FX_ECHO_ACTIVE] == 1)
				{
					out_left[x] = echo->do_left( out_left[x] );
					out_right[x] = echo->do_right( out_right[x] );
				}
				
				if (fParameters[TRICERATOPS_FX_REVERB_ACTIVE] == 1)
				{
					int reverb_decay = fParameters[TRICERATOPS_FX_REVERB_DECAY];
					reverb[reverb_decay]->setEffectMix( fParameters[TRICERATOPS_FX_REVERB_MIX] );
					reverb[reverb_decay]->tick(((out_left[x]+out_right[x]) * 0.2));
					out_left[x] += reverb[reverb_decay]->lastOutputL();
					out_right[x] += reverb[reverb_decay]->lastOutputR();
				}
							


			}
			
		}


		//----------------------------------------------
		 				
		void initProgramName(uint32_t index, String& program_name) override
		{
		}
	
		
		//----------------------------------------------

	private:

		bool preset_loading = false;
		int bpm;
		uint32_t buffer_length;
};

Plugin* createPlugin()
{
	// cout << TRICERATOPS_MIDI_CHANNEL << endl;

	triceratopsPlugin* triceratops = new triceratopsPlugin();
	for (int x=0; x<max_notes; x++)
	{
		triceratops->synths[x].synth_params->TRICERATOPS_MASTER_VOLUME = &triceratops->fParameters[TRICERATOPS_MASTER_VOLUME];
		triceratops->synths[x].synth_params->TRICERATOPS_MASTER_TUNE = &triceratops->fParameters[TRICERATOPS_MASTER_TUNE];
		triceratops->synths[x].synth_params->TRICERATOPS_AMP_DRIVE = &triceratops->fParameters[TRICERATOPS_AMP_DRIVE];
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_MODE = &triceratops->fParameters[TRICERATOPS_FILTER_MODE];
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_FREQUENCY = &triceratops->fParameters[TRICERATOPS_FILTER_FREQUENCY];
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_RESONANCE = &triceratops->fParameters[TRICERATOPS_FILTER_RESONANCE];		
		triceratops->synths[x].synth_params->TRICERATOPS_FILTER_KEY_FOLLOW =&triceratops->fParameters[TRICERATOPS_FILTER_KEY_FOLLOW];
		triceratops->synths[x].synth_params->TRICERATOPS_LEGATO = &triceratops->fParameters[TRICERATOPS_LEGATO];
		triceratops->synths[x].synth_params->TRICERATOPS_SYNC = &triceratops->fParameters[TRICERATOPS_SYNC];
		triceratops->synths[x].synth_params->TRICERATOPS_WARMTH = &triceratops->fParameters[TRICERATOPS_WARMTH];		
		triceratops->synths[x].synth_params->TRICERATOPS_FM = &triceratops->fParameters[TRICERATOPS_FM];
		triceratops->synths[x].synth_params->TRICERATOPS_PANIC = &triceratops->fParameters[TRICERATOPS_PANIC];		
		
		triceratops->synths[x].synth_params->TRICERATOPS_ACTIVE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_ACTIVE]; 
		triceratops->synths[x].synth_params->TRICERATOPS_VOLUME_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_VOLUME];
		triceratops->synths[x].synth_params->TRICERATOPS_PULSEWIDTH_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_PULSEWIDTH];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_DETUNE];
		triceratops->synths[x].synth_params->TRICERATOPS_WAVE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_CENTRE_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_DETUNE_CENTRE];
		triceratops->synths[x].synth_params->TRICERATOPS_INERTIA_ONE = &triceratops->fParameters[TRICERATOPS_OSC1_INERTIA];
		
				
		triceratops->synths[x].synth_params->TRICERATOPS_ACTIVE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_ACTIVE]; 
		triceratops->synths[x].synth_params->TRICERATOPS_VOLUME_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_VOLUME];
		triceratops->synths[x].synth_params->TRICERATOPS_PULSEWIDTH_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_PULSEWIDTH];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_DETUNE];
		triceratops->synths[x].synth_params->TRICERATOPS_WAVE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_CENTRE_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_DETUNE_CENTRE];
		triceratops->synths[x].synth_params->TRICERATOPS_INERTIA_TWO = &triceratops->fParameters[TRICERATOPS_OSC2_INERTIA];
		
				
		triceratops->synths[x].synth_params->TRICERATOPS_ACTIVE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_ACTIVE]; 
		triceratops->synths[x].synth_params->TRICERATOPS_VOLUME_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_VOLUME];
		triceratops->synths[x].synth_params->TRICERATOPS_PULSEWIDTH_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_PULSEWIDTH];
		triceratops->synths[x].synth_params->TRICERATOPS_OCTAVE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_OCTAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_DETUNE];
		triceratops->synths[x].synth_params->TRICERATOPS_WAVE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_DETUNE_CENTRE_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_DETUNE_CENTRE];
		triceratops->synths[x].synth_params->TRICERATOPS_INERTIA_THREE = &triceratops->fParameters[TRICERATOPS_OSC3_INERTIA];
		
		triceratops->synths[x].synth_params->TRICERATOPS_ATTACK_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_ATTACK];
		triceratops->synths[x].synth_params->TRICERATOPS_DECAY_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_SUSTAIN_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_SUSTAIN];
		triceratops->synths[x].synth_params->TRICERATOPS_RELEASE_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_RELEASE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR1_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_ADSR1_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR1_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_ADSR1_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR1_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_ADSR1_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR1_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_ADSR1_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_ATTACK_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_ATTACK];
		triceratops->synths[x].synth_params->TRICERATOPS_DECAY_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_SUSTAIN_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_SUSTAIN];
		triceratops->synths[x].synth_params->TRICERATOPS_RELEASE_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_RELEASE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR2_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_ADSR2_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR2_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_ADSR2_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR2_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_ADSR2_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR2_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_ADSR2_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_ATTACK_THREE = &triceratops->fParameters[TRICERATOPS_ADSR3_ATTACK];
		triceratops->synths[x].synth_params->TRICERATOPS_DECAY_THREE = &triceratops->fParameters[TRICERATOPS_ADSR3_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_SUSTAIN_THREE = &triceratops->fParameters[TRICERATOPS_ADSR3_SUSTAIN];
		triceratops->synths[x].synth_params->TRICERATOPS_RELEASE_THREE = &triceratops->fParameters[TRICERATOPS_ADSR3_RELEASE];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR3_LFO1_AMOUNT = &triceratops->fParameters[TRICERATOPS_ADSR3_LFO1_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR3_LFO2_AMOUNT = &triceratops->fParameters[TRICERATOPS_ADSR3_LFO2_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_ADSR3_LFO3_AMOUNT = &triceratops->fParameters[TRICERATOPS_ADSR3_LFO3_AMOUNT];
		
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_RETRIG = &triceratops->fParameters[TRICERATOPS_LFO1_RETRIG];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_SPEED = &triceratops->fParameters[TRICERATOPS_LFO1_SPEED];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_WAVE = &triceratops->fParameters[TRICERATOPS_LFO1_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_OSC1_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO1_OSC1_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_OSC2_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO1_OSC2_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_OSC3_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO1_OSC3_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_FILTER = &triceratops->fParameters[TRICERATOPS_LFO1_FILTER];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_LFO1_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_LFO1_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_LFO1_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO1_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_LFO1_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_RETRIG = &triceratops->fParameters[TRICERATOPS_LFO2_RETRIG];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_SPEED = &triceratops->fParameters[TRICERATOPS_LFO2_SPEED];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_WAVE = &triceratops->fParameters[TRICERATOPS_LFO2_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_OSC1_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO2_OSC1_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_OSC2_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO2_OSC2_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_OSC3_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO2_OSC3_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_FILTER = &triceratops->fParameters[TRICERATOPS_LFO2_FILTER];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_LFO2_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_LFO2_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_LFO2_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO2_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_LFO2_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_RETRIG = &triceratops->fParameters[TRICERATOPS_LFO3_RETRIG];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_SPEED = &triceratops->fParameters[TRICERATOPS_LFO3_SPEED];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_WAVE = &triceratops->fParameters[TRICERATOPS_LFO3_WAVE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_OSC1_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO3_OSC1_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_OSC2_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO3_OSC2_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_OSC3_AMOUNT = &triceratops->fParameters[TRICERATOPS_LFO3_OSC3_AMOUNT];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_FILTER = &triceratops->fParameters[TRICERATOPS_LFO3_FILTER];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_ROUTE_ONE = &triceratops->fParameters[TRICERATOPS_LFO3_ROUTE_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_ROUTE_ONE_DEST = &triceratops->fParameters[TRICERATOPS_LFO3_ROUTE_ONE_DEST];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_ROUTE_TWO = &triceratops->fParameters[TRICERATOPS_LFO3_ROUTE_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_LFO3_ROUTE_TWO_DEST = &triceratops->fParameters[TRICERATOPS_LFO3_ROUTE_TWO_DEST];
		
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_ACTIVE = &triceratops->fParameters[TRICERATOPS_FX_ECHO_ACTIVE];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_SPEED = &triceratops->fParameters[TRICERATOPS_FX_ECHO_SPEED];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_DECAY = &triceratops->fParameters[TRICERATOPS_FX_ECHO_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_EQ_LOW = &triceratops->fParameters[TRICERATOPS_FX_ECHO_EQ_LOW];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_EQ_MID = &triceratops->fParameters[TRICERATOPS_FX_ECHO_EQ_MID];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_ECHO_EQ_HIGH = &triceratops->fParameters[TRICERATOPS_FX_ECHO_EQ_HIGH];
		
		triceratops->synths[x].synth_params->TRICERATOPS_UNISON_ACTIVATE = &triceratops->fParameters[TRICERATOPS_UNISON_ACTIVATE];
		triceratops->synths[x].synth_params->TRICERATOPS_UNISON_ONE = &triceratops->fParameters[TRICERATOPS_UNISON_ONE];
		triceratops->synths[x].synth_params->TRICERATOPS_UNISON_TWO = &triceratops->fParameters[TRICERATOPS_UNISON_TWO];
		triceratops->synths[x].synth_params->TRICERATOPS_UNISON_THREE = &triceratops->fParameters[TRICERATOPS_UNISON_THREE];
		
		triceratops->synths[x].synth_params->TRICERATOPS_MODIFIER_DIRT = &triceratops->fParameters[TRICERATOPS_MODIFIER_DIRT];

		triceratops->synths[x].synth_params->TRICERATOPS_FX_REVERB_ACTIVE = &triceratops->fParameters[TRICERATOPS_FX_REVERB_ACTIVE];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_REVERB_DECAY = &triceratops->fParameters[TRICERATOPS_FX_REVERB_DECAY];
		triceratops->synths[x].synth_params->TRICERATOPS_FX_REVERB_MIX = &triceratops->fParameters[TRICERATOPS_FX_REVERB_MIX];
		
		triceratops->synths[x].synth_params->TRICERATOPS_MODIFIER_STEREO_MODE = &triceratops->fParameters[TRICERATOPS_MODIFIER_STEREO_MODE];
		triceratops->synths[x].synth_params->TRICERATOPS_OSC1_PAN = &triceratops->fParameters[TRICERATOPS_OSC1_PAN];
		triceratops->synths[x].synth_params->TRICERATOPS_OSC2_PAN = &triceratops->fParameters[TRICERATOPS_OSC2_PAN];
		triceratops->synths[x].synth_params->TRICERATOPS_OSC3_PAN = &triceratops->fParameters[TRICERATOPS_OSC3_PAN];
		triceratops->synths[x].synth_params->TRICERATOPS_MODIFIER_RING = &triceratops->fParameters[TRICERATOPS_MODIFIER_RING];
		
		triceratops->synths[x].synth_params->TRICERATOPS_CATEGORY = &triceratops->fParameters[TRICERATOPS_PRESET_CATEGORY];
		triceratops->synths[x].synth_params->TRICERATOPS_PITCH_BEND_RANGE = &triceratops->fParameters[TRICERATOPS_PITCH_BEND_RANGE];
		triceratops->synths[x].synth_params->TRICERATOPS_MIDI_CHANNEL = &triceratops->fParameters[TRICERATOPS_MIDI_CHANNEL];

	}
	return triceratops;
}      

END_NAMESPACE_DISTRHO





