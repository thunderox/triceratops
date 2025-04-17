

#include "synth.h"
#include "minblep.h"

// CONSTRUCTOR

//-------------------------------------------------------------
/*
Public source code by alex@smartelectronix.com
Simple example of implementation of formant filter
Vowelnum can be 0,1,2,3,4 <=> A,E,I,O,U
Good for spectral rich input like saw or square 
*/
//-------------------------------------------------------------VOWEL COEFFICIENTS

const double coeff[5][11] = {
	{ 3.11044e-06,
	8.943665402,    -36.83889529,    92.01697887,    -154.337906,    181.6233289,
	-151.8651235,   89.09614114,    -35.10298511,    8.388101016,    -0.923313471  ///A
	},
	{4.36215e-06,
	8.90438318,    -36.55179099,    91.05750846,    -152.422234,    179.1170248,  ///E
	-149.6496211,87.78352223,    -34.60687431,    8.282228154,    -0.914150747
	},
	{ 3.33819e-06,
	8.893102966,    -36.49532826,    90.96543286,    -152.4545478,    179.4835618,
	-150.315433,    88.43409371,    -34.98612086,    8.407803364,    -0.932568035  ///I
	},
	{1.13572e-06,
	8.994734087,    -37.2084849,    93.22900521,    -156.6929844,    184.596544,   ///O
	-154.3755513,    90.49663749,    -35.58964535,    8.478996281,    -0.929252233
	},
	{4.09431e-07,
	8.997322763,    -37.20218544,    93.11385476,    -156.2530937,    183.7080141,  ///U
	-153.2631681,    89.59539726,    -35.12454591,    8.338655623,    -0.910251753
	}
}; 


synth::synth()
{
	for (int x=0; x<10; ++x)
	{
		memory[x] = 0;
	}
	


	stereo_mode = true;

	synth_params = (params*)malloc(sizeof(params));

	update_counter = 0;
	update_speed = 64;
	out_fm = 0;
	OSC_count_one = 0;
	env_amp_level_db = 0;
	midi_key = -1;

	nixnoise = new noise();

	osc_frequency = 440;
	
	gMinBLEP.lpTable = minBLEP_table;
	gMinBLEP.c = sizeof(minBLEP_table) / sizeof(double);


	buf0_left=0; buf1_left=0;

	f_left=0; pc_left=0; q_left=0;             //filter coefficients
  	bf0_left=0; bf1_left=0; bf2_left=0; bf3_left=0; bf4_left=0;  //filter buffers (beware denormals!)
  	t1_left=0; t2_left=0;              //temporary buffers

	selectivity_left=90, gain1_left=0, gain2_left=0.25, ratio_left=7, cap_left=0; 

	buf0_right=0; buf1_right=0;

	f_right=0; pc_right=0; q_right=0;             //filter coefficients
  	bf0_right=0; bf1_right=0; bf2_right=0; bf3_right=0; bf4_right=0;  //filter buffers (beware denormals!)
  	t1_right=0; t2_right=0;              //temporary buffers

	selectivity_right=90, gain1_right=0, gain2_right=0.25, ratio_right=7, cap_right=0; 

	env_amp_state = env_state_dormant;
	env_filter_state = env_state_dormant;
	env_lfo_state = env_state_dormant;

	env_amp_level = 0;
	env_filter_level = 0;
	env_lfo_level = 0;

}

// DECONSTRUCTOR

synth::~synth()
{

	for (int OSC=0; OSC<6; ++OSC)
	{
		free( lpO[OSC] );
		free( sinewave_osc[OSC] );
	}

	free(synth_params);
	delete(nixnoise);

}

// RUN

void synth::run(float* out_left, float* out_right, uint32_t n_samples)
{
	if (synth_params->TRICERATOPS_ACTIVE_ONE[0]!=1 &&
		synth_params->TRICERATOPS_ACTIVE_TWO[0]!=1 &&
		synth_params->TRICERATOPS_ACTIVE_THREE[0]!=1) return;
		

	//------------------------------ envelope stuff

	float amp_attack = fast_pow(synth_params->TRICERATOPS_ATTACK_ONE[0],10);
	float amp_decay = fast_pow(synth_params->TRICERATOPS_DECAY_ONE[0],15);
	float amp_sustain = 1-synth_params->TRICERATOPS_SUSTAIN_ONE[0];
	float amp_release = fast_pow(synth_params->TRICERATOPS_RELEASE_ONE[0],14);

	if (amp_release < 7.02758e-07) amp_release = 7.02758e-07;
	if (amp_release > 0.00075) amp_release = 0.00075;


	float filter_attack = fast_pow(synth_params->TRICERATOPS_ATTACK_TWO[0],10);
	float filter_decay = fast_pow(synth_params->TRICERATOPS_DECAY_TWO[0],15);
	float filter_sustain = 1-synth_params->TRICERATOPS_SUSTAIN_TWO[0];
	float filter_release = fast_pow(synth_params->TRICERATOPS_RELEASE_TWO[0],14);

	float lfo_attack = fast_pow(synth_params->TRICERATOPS_ATTACK_THREE[0],10);
	float lfo_decay = fast_pow(synth_params->TRICERATOPS_DECAY_THREE[0],15);
	float lfo_sustain = 1-synth_params->TRICERATOPS_SUSTAIN_THREE[0];
	float lfo_release = fast_pow(synth_params->TRICERATOPS_RELEASE_THREE[0],14);


	lpO[0]->type = synth_params->TRICERATOPS_WAVE_ONE[0];
	lpO[3]->type = synth_params->TRICERATOPS_WAVE_ONE[0];
	
	lpO[1]->type = synth_params->TRICERATOPS_WAVE_TWO[0];
	lpO[4]->type = synth_params->TRICERATOPS_WAVE_TWO[0];

	lpO[2]->type = synth_params->TRICERATOPS_WAVE_THREE[0];
	lpO[5]->type = synth_params->TRICERATOPS_WAVE_THREE[0];

	float route_matrix[18];
	bool route_type[18];

	int TRICERATOPS_ADSR1_ROUTE_ONE_DEST = synth_params->TRICERATOPS_ADSR1_ROUTE_ONE_DEST[0];
	int TRICERATOPS_ADSR2_ROUTE_ONE_DEST = synth_params->TRICERATOPS_ADSR2_ROUTE_ONE_DEST[0];
	int TRICERATOPS_ADSR1_ROUTE_TWO_DEST = synth_params->TRICERATOPS_ADSR1_ROUTE_TWO_DEST[0];
	int TRICERATOPS_ADSR2_ROUTE_TWO_DEST = synth_params->TRICERATOPS_ADSR2_ROUTE_TWO_DEST[0];
	int TRICERATOPS_LFO1_ROUTE_ONE_DEST = synth_params->TRICERATOPS_LFO1_ROUTE_ONE_DEST[0];
	int TRICERATOPS_LFO2_ROUTE_ONE_DEST = synth_params->TRICERATOPS_LFO2_ROUTE_ONE_DEST[0];
	int TRICERATOPS_LFO3_ROUTE_ONE_DEST = synth_params->TRICERATOPS_LFO3_ROUTE_ONE_DEST[0];
	int TRICERATOPS_LFO1_ROUTE_TWO_DEST = synth_params->TRICERATOPS_LFO1_ROUTE_TWO_DEST[0];
	int TRICERATOPS_LFO2_ROUTE_TWO_DEST = synth_params->TRICERATOPS_LFO2_ROUTE_TWO_DEST[0];
	int TRICERATOPS_LFO3_ROUTE_TWO_DEST = synth_params->TRICERATOPS_LFO3_ROUTE_TWO_DEST[0];

	float test_osc_one = 0;
	float test_osc_two = 0;
	float test_osc_three = 0;

	for (uint32_t pos = 0; pos < n_samples; ++pos)
	{

		//------------------------------ ENVELOPE STUFF

		++update_counter;
		if (update_counter > update_speed) update_counter = 0;

			//--------- ADSR AMP
			// ATTACK

			switch (env_amp_state)
			{

			case env_state_begin:
				if (env_amp_level > 0) 
				{
					env_amp_level -= 0.01;
					if (env_amp_level <= 0) 
					env_amp_state = env_state_attack;
				}

				if (env_amp_level < 0)
				{
					env_amp_level -= 0.01;
					if (env_amp_level >= 0)
					env_amp_state = env_state_attack;
				}

				if (env_amp_level == 0) env_amp_state = env_state_attack;
	
				break;

			case env_state_attack:
				if (env_amp_level < 1) 
				{
					env_amp_level += amp_attack;
					if (env_amp_level >= 1)
					{
						env_amp_level = 1;
						env_amp_state = env_state_decay;
					}
				}
				break;

			// DECAY / SUSTAIN

			case env_state_decay:
			{	
				if (env_amp_level > amp_sustain) 
				{
					env_amp_level -= amp_decay;
						if (env_amp_level <= amp_sustain)
						{
							env_amp_level = amp_sustain;
							env_amp_state = env_state_wait;
						}
				}

				if (env_amp_level <amp_sustain) 
				{
					env_amp_level += amp_decay;
						if (env_amp_level >=amp_sustain)
						{
							env_amp_level = amp_sustain;
							env_amp_state = env_state_wait;
						}
				}
				break;

			// RELEASE

			case env_state_release:
				if (env_amp_level > 0) 
				{
					env_amp_level -=amp_release;
					if (env_amp_level <= 0)
					{
						env_amp_level = 0;
						env_amp_state = env_state_dormant;
					}
				}
				break;
			}
		}
		
		env_amp_level_db = env_amp_level * env_amp_level * env_amp_level; 
		

			//--------- ADSR FILTER
			// ATTACK

			switch (env_filter_state)
			{

			case env_state_attack:
				if (env_filter_level < 1) 
				{
					env_filter_level += filter_attack;
					if (env_filter_level >= 1)
					{
						env_filter_level = 1;
						env_filter_state = env_state_decay;
					}
				}
				break;

			// DECAY / SUSTAIN

			case env_state_decay:
			{	
				if (env_filter_level > filter_sustain) 
				{
					env_filter_level -= filter_decay;
						if (env_filter_level <= filter_sustain)
						{
							env_filter_level = filter_sustain;
							env_filter_state = env_state_wait;
						}
				}

				if (env_filter_level <filter_sustain) 
				{
					env_filter_level += filter_decay;
						if (env_filter_level >=filter_sustain)
						{
							env_filter_level = filter_sustain;
							env_filter_state = env_state_wait;
						}
				}
				break;

			// RELEASE

			case env_state_release:
				if (env_filter_level > 0) 
				{
					env_filter_level -=filter_release;
					if (env_filter_level <= 0)
					{
						env_filter_level = 0;
						env_filter_state = env_state_dormant;
					}
				}
				break;
			}
		}

			//--------- ADSR LFO
			// ATTACK

			if (env_lfo_state == env_state_attack)
			{
				if (env_lfo_level < 1) 
				{
					env_lfo_level += lfo_attack;
					if (env_lfo_level >= 1)
					{
						env_lfo_level = 1;
						env_lfo_state = env_state_decay;
					}
				}
			}	
		
			// DECAY / SUSTAIN

			if (env_lfo_state == env_state_decay)
			{	
				if (env_lfo_level > lfo_sustain) 
				{
					env_lfo_level -= lfo_decay;
						if (env_lfo_level <= lfo_sustain)
						{
							env_lfo_level = lfo_sustain;
							env_lfo_state = env_state_wait;
						}
				}

				if (env_lfo_level <lfo_sustain) 
				{
					env_lfo_level += lfo_decay;
						if (env_lfo_level >=lfo_sustain)
						{
							env_lfo_level = lfo_sustain;
							env_lfo_state = env_state_wait;
						}
				}
			}

			// RELEASE

			if (env_lfo_state == env_state_release)
			{
				if (env_lfo_level > 0) 
				{
					env_lfo_level -=lfo_release;
					if (env_lfo_level <= 0)
					{
						env_lfo_level = 0;
						env_lfo_state = env_state_dormant;
					}
				}
			}	
		//------------------------------ MATRIX MODULATION STUFF

		float lfo1 = lfo1_out[0][pos];
		float lfo2 = lfo2_out[0][pos];
		float lfo3 = lfo3_out[0][pos];

		if (synth_params->TRICERATOPS_ADSR3_LFO1_AMOUNT[0] != 0) lfo1 *= env_lfo_level;
		if (synth_params->TRICERATOPS_ADSR3_LFO2_AMOUNT[0] != 0) lfo2 *= env_lfo_level;
		if (synth_params->TRICERATOPS_ADSR3_LFO3_AMOUNT[0] != 0) lfo3 *= env_lfo_level;

		route_matrix[route_none] = 0;
		route_matrix[route_cutoff] = 0;
		route_matrix[route_res] = 0;
		route_matrix[route_osc1_pitch] = 0;
		route_matrix[route_osc2_pitch] = 0;
		route_matrix[route_osc3_pitch] = 0;
		route_matrix[route_osc1_pw] = 0;
		route_matrix[route_osc2_pw] = 0;
		route_matrix[route_osc3_pw] = 0;
		route_matrix[route_osc1_pan] = 0;
		route_matrix[route_osc2_pan] = 0;
		route_matrix[route_osc3_pan] = 0;
		route_matrix[route_lfo1_sp] = 0;
		route_matrix[route_lfo2_sp] = 0;
		route_matrix[route_lfo3_sp] = 0;
		route_matrix[route_osc1_amp] = 0;
		route_matrix[route_osc2_amp] = 0;
		route_matrix[route_osc3_amp] = 0;

		route_type[route_none] = false;
		route_type[route_cutoff] = false;
		route_type[route_res] = false;
		route_type[route_osc1_pitch] = false;
		route_type[route_osc2_pitch] = false;
		route_type[route_osc3_pitch] = false;
		route_type[route_osc1_pw] = false;
		route_type[route_osc2_pw] = false;
		route_type[route_osc3_pw] = false;
		route_type[route_osc1_pan] = false;
		route_type[route_osc2_pan] = false;
		route_type[route_osc3_pan] = false;
		route_type[route_lfo1_sp] = false;
		route_type[route_lfo2_sp] = false;
		route_type[route_lfo3_sp] = false;
		route_type[route_osc1_amp] = true;
		route_type[route_osc2_amp] = true;
		route_type[route_osc3_amp] = true;


		route_matrix[TRICERATOPS_ADSR1_ROUTE_ONE_DEST] += 
			synth_params->TRICERATOPS_ADSR1_ROUTE_ONE[0] * env_amp_level;

		route_matrix[TRICERATOPS_ADSR1_ROUTE_TWO_DEST] +=
			synth_params->TRICERATOPS_ADSR1_ROUTE_TWO[0] * env_amp_level;



		route_matrix[TRICERATOPS_ADSR2_ROUTE_ONE_DEST] += 
			synth_params->TRICERATOPS_ADSR2_ROUTE_ONE[0] * env_filter_level;

		route_matrix[TRICERATOPS_ADSR2_ROUTE_TWO_DEST] +=
			synth_params->TRICERATOPS_ADSR2_ROUTE_TWO[0] * env_filter_level;


		if (route_type[TRICERATOPS_LFO1_ROUTE_ONE_DEST] == false)
		{
			route_matrix[TRICERATOPS_LFO1_ROUTE_ONE_DEST] +=
				synth_params->TRICERATOPS_LFO1_ROUTE_ONE[0] * lfo1;
		}
		else
		{
			route_matrix[TRICERATOPS_LFO1_ROUTE_ONE_DEST] +=
				synth_params->TRICERATOPS_LFO1_ROUTE_ONE[0] * (1-((1 + lfo1)/2));
				route_matrix[TRICERATOPS_LFO1_ROUTE_ONE_DEST] + 0.0000002;
		}

		if (route_type[TRICERATOPS_LFO1_ROUTE_TWO_DEST] == false)
		{
			route_matrix[TRICERATOPS_LFO1_ROUTE_TWO_DEST] +=
				synth_params->TRICERATOPS_LFO1_ROUTE_TWO[0] * lfo1;
		}
		else
		{
			route_matrix[TRICERATOPS_LFO1_ROUTE_TWO_DEST] +=
				synth_params->TRICERATOPS_LFO1_ROUTE_TWO[0] * (1-((1 + lfo1)/2));
				route_matrix[TRICERATOPS_LFO1_ROUTE_TWO_DEST] + 0.0000002;
		}




		if (route_type[TRICERATOPS_LFO2_ROUTE_ONE_DEST] == false)
		{
			route_matrix[TRICERATOPS_LFO2_ROUTE_ONE_DEST] +=
				synth_params->TRICERATOPS_LFO2_ROUTE_ONE[0] * lfo2;
		}
		else
		{
			route_matrix[TRICERATOPS_LFO2_ROUTE_ONE_DEST] +=
				synth_params->TRICERATOPS_LFO2_ROUTE_ONE[0] * (1-((1 + lfo2)/2));
				route_matrix[TRICERATOPS_LFO2_ROUTE_ONE_DEST] + 0.0000002;
		}

		if (route_type[TRICERATOPS_LFO2_ROUTE_TWO_DEST] == false)
		{
			route_matrix[TRICERATOPS_LFO2_ROUTE_TWO_DEST] +=
				synth_params->TRICERATOPS_LFO2_ROUTE_TWO[0] * lfo2;
		}
		else
		{
			route_matrix[TRICERATOPS_LFO2_ROUTE_TWO_DEST] +=
				synth_params->TRICERATOPS_LFO2_ROUTE_TWO[0] * (1-((1 + lfo2)/2));
				route_matrix[TRICERATOPS_LFO2_ROUTE_TWO_DEST] + 0.0000002;
		}




		if (route_type[TRICERATOPS_LFO3_ROUTE_ONE_DEST] == false)
		{
			route_matrix[TRICERATOPS_LFO3_ROUTE_ONE_DEST] +=
				synth_params->TRICERATOPS_LFO3_ROUTE_ONE[0] * lfo3;
		}
		else
		{
			route_matrix[TRICERATOPS_LFO3_ROUTE_ONE_DEST] +=
				synth_params->TRICERATOPS_LFO3_ROUTE_ONE[0] * (1-((1 + lfo3)/2));
				route_matrix[TRICERATOPS_LFO3_ROUTE_ONE_DEST] + 0.0000002;
		}

		if (route_type[TRICERATOPS_LFO3_ROUTE_TWO_DEST] == false)
		{
			route_matrix[TRICERATOPS_LFO3_ROUTE_TWO_DEST] +=
				synth_params->TRICERATOPS_LFO3_ROUTE_TWO[0] * lfo3;
		}
		else
		{
			route_matrix[TRICERATOPS_LFO3_ROUTE_TWO_DEST] +=
				synth_params->TRICERATOPS_LFO3_ROUTE_TWO[0] * (1-((1 + lfo3)/2));
				route_matrix[TRICERATOPS_LFO3_ROUTE_TWO_DEST] + 0.0000002;
		}
			
		
	
		if (update_counter == 0 || synth_params->TRICERATOPS_FM[0] == 1)
		{
			float routes = 0;	


			//------------- OSCILLATOR STUFF

			// do oscilator 0 ( and 3)
	
			routes = route_matrix[route_osc1_pitch] * 12;

			if (synth_params->TRICERATOPS_MODIFIER_DIRT[0] > 0)
				routes += (nixnoise->tick()*synth_params->TRICERATOPS_MODIFIER_DIRT[0]);

			// routes += channel_after_touch[0] * (synth_params->TRICERATOPS_LFO1_OSC1_AMOUNT[0] * lfo1 * 12);
			routes += (synth_params->TRICERATOPS_LFO1_OSC1_AMOUNT[0] * lfo1 * 12);
			routes += synth_params->TRICERATOPS_LFO2_OSC1_AMOUNT[0] * lfo2 * 12;
			routes += synth_params->TRICERATOPS_LFO3_OSC1_AMOUNT[0] * lfo3 * 12;

			if (synth_params->TRICERATOPS_FM[0] == 1) { routes += out_fm; } 

			routes += synth_params->TRICERATOPS_MASTER_TUNE[0] + 0.22;
			routes += synth_params->TRICERATOPS_DETUNE_ONE[0];
			routes += (12 * synth_params->TRICERATOPS_OCTAVE_ONE[0]);

			float new_frequency = osc_frequency * fastishP2F(routes + (pitch_bend[0] * synth_params->TRICERATOPS_PITCH_BEND_RANGE[0]) );

			if (new_frequency > 18000) new_frequency = 18000;

			// check if inertia's enabled - slide frequency

			if (synth_params->TRICERATOPS_INERTIA_ONE[0] > 0)
			{
				inertia_one.set_speed( (new_frequency * (1.1-synth_params->TRICERATOPS_INERTIA_ONE[0])) * 0.005 );

				if (synth_params->TRICERATOPS_WAVE_ONE[0] < 2)
				{
					lpO[0]->f = inertia_one.slide(new_frequency);
				}

				if (synth_params->TRICERATOPS_WAVE_ONE[0] == 2)
				{
					sinewave_osc[0]->setRate(inertia_one.slide(new_frequency));
				}
			}


		// no inertia just set frequency straight away

		if (synth_params->TRICERATOPS_INERTIA_ONE[0] == 0)
		{
			if (synth_params->TRICERATOPS_WAVE_ONE[0] < 2) // it's a saw or square
			{
				lpO[0]->f = new_frequency;
			}

			if (synth_params->TRICERATOPS_WAVE_ONE[0] == 2) // it's a sinus
			{
				sinewave_osc[0]->setRate(new_frequency);
			}
		}
			
		// is unison activated? detune one down, one up
		
		if (synth_params->TRICERATOPS_UNISON_ACTIVATE[0] == 1)
		{
			lpO[3]->f = lpO[0]->f + synth_params->TRICERATOPS_UNISON_ONE[0];
			lpO[0]->f -= synth_params->TRICERATOPS_UNISON_ONE[0];

			sinewave_osc[3]->setRate(new_frequency + synth_params->TRICERATOPS_UNISON_ONE[0]);
			sinewave_osc[0]->setRate(new_frequency - synth_params->TRICERATOPS_UNISON_ONE[0]);
		}

		// pulsewidth routes for squarewave
		
		if (synth_params->TRICERATOPS_WAVE_ONE[0] == 1)
		{

			routes = route_matrix[route_osc1_pw];

			routes += synth_params->TRICERATOPS_PULSEWIDTH_ONE[0]+0.5;

			if (routes < 0) routes = 0;
			if (routes > 1) routes = 1;
	
			lpO[0]->fPWM = routes;
			lpO[3]->fPWM = routes;
			
		}



		// do oscilator 1 ( and 4)
	
		routes = route_matrix[route_osc2_pitch] * 12;

		if (synth_params->TRICERATOPS_MODIFIER_DIRT[0] > 0)
			routes += (nixnoise->tick()*synth_params->TRICERATOPS_MODIFIER_DIRT[0]);

		// routes += channel_after_touch[0] * (synth_params->TRICERATOPS_LFO1_OSC2_AMOUNT[0] * lfo1 * 12);
		routes += (synth_params->TRICERATOPS_LFO1_OSC2_AMOUNT[0] * lfo1 * 12);
		routes += synth_params->TRICERATOPS_LFO2_OSC2_AMOUNT[0] * lfo2 * 12;
		routes += synth_params->TRICERATOPS_LFO3_OSC2_AMOUNT[0] * lfo3 * 12;

		if (synth_params->TRICERATOPS_FM[0] == 1) { routes+= out_fm; } 

		routes += synth_params->TRICERATOPS_MASTER_TUNE[0] + 0.22;
		routes += synth_params->TRICERATOPS_DETUNE_TWO[0];
		routes += (12 * synth_params->TRICERATOPS_OCTAVE_TWO[0]);

		new_frequency = osc_frequency * fastishP2F(routes + (pitch_bend[0]  * synth_params->TRICERATOPS_PITCH_BEND_RANGE[0]) );

		if (new_frequency > 18000) new_frequency = 18000;

		// check if inertia's enabled - slide frequency

		if (synth_params->TRICERATOPS_INERTIA_TWO[0] > 0)
		{
			inertia_two.set_speed( (new_frequency * (1.1-synth_params->TRICERATOPS_INERTIA_TWO[0])) * 0.005 );

			if (synth_params->TRICERATOPS_WAVE_TWO[0] < 2)
			{
				lpO[1]->f = inertia_two.slide(new_frequency);
			}

			if (synth_params->TRICERATOPS_WAVE_TWO[0] == 2)
			{
				sinewave_osc[1]->setRate(inertia_two.slide(new_frequency));
			}
		}

		// no inertia just set frequency straight away

		if (synth_params->TRICERATOPS_INERTIA_TWO[0] == 0)
		{
			if (synth_params->TRICERATOPS_WAVE_TWO[0] < 2) // it's a saw or square
			{
				lpO[1]->f = new_frequency;
			}

			if (synth_params->TRICERATOPS_WAVE_TWO[0] == 2) // it's a sinus
			{
				sinewave_osc[1]->setRate(new_frequency);
			}
		}
			
		// is unison activated? detune one down, one up
		
		if (synth_params->TRICERATOPS_UNISON_ACTIVATE[0] == 1)
		{
			lpO[4]->f = lpO[1]->f + synth_params->TRICERATOPS_UNISON_TWO[0];
			lpO[1]->f -= synth_params->TRICERATOPS_UNISON_TWO[0];

			sinewave_osc[4]->setRate(new_frequency + synth_params->TRICERATOPS_UNISON_TWO[0]);
			sinewave_osc[1]->setRate(new_frequency - synth_params->TRICERATOPS_UNISON_TWO[0]);
		}

			// pulsewidth routes for squarewave
		
		if (synth_params->TRICERATOPS_WAVE_TWO[0] == 1)
		{

			routes = route_matrix[route_osc2_pw];

			routes += synth_params->TRICERATOPS_PULSEWIDTH_TWO[0]+0.5;

			if (routes < 0) routes = 0;
			if (routes > 1) routes = 1;

			lpO[1]->fPWM = routes;
			lpO[4]->fPWM = routes;
		}


	
		// do oscillator 2 ( and 5)


		routes = route_matrix[route_osc3_pitch] * 12;

		if (synth_params->TRICERATOPS_MODIFIER_DIRT[0] > 0)
			routes += (nixnoise->tick()*synth_params->TRICERATOPS_MODIFIER_DIRT[0]);


		// routes += channel_after_touch[0] * (synth_params->TRICERATOPS_LFO1_OSC3_AMOUNT[0] * lfo1 * 12);
		routes += (synth_params->TRICERATOPS_LFO1_OSC3_AMOUNT[0] * lfo1 * 12);
		routes += synth_params->TRICERATOPS_LFO2_OSC3_AMOUNT[0] * lfo2 * 12;
		routes += synth_params->TRICERATOPS_LFO3_OSC3_AMOUNT[0] * lfo3 * 12;

		routes += synth_params->TRICERATOPS_MASTER_TUNE[0] + 0.22;
		routes += synth_params->TRICERATOPS_DETUNE_THREE[0];
		routes += (12 * synth_params->TRICERATOPS_OCTAVE_THREE[0]);

		new_frequency = osc_frequency * fastishP2F(routes + (pitch_bend[0]  * synth_params->TRICERATOPS_PITCH_BEND_RANGE[0]) );
		if (new_frequency > 18000) new_frequency = 18000;


		// check if inertia's enabled - slide frequency

		if (synth_params->TRICERATOPS_INERTIA_THREE[0] > 0)
		{
			inertia_three.set_speed( (new_frequency * (1.1-synth_params->TRICERATOPS_INERTIA_THREE[0])) * 0.005 );

			if (synth_params->TRICERATOPS_WAVE_THREE[0] < 2)
			{
				lpO[2]->f = inertia_three.slide(new_frequency);
			}

			if (synth_params->TRICERATOPS_WAVE_THREE[0] == 2)
			{
				sinewave_osc[2]->setRate(inertia_three.slide(new_frequency));
			}
		}

		// no inertia just set frequency straight away

		if (synth_params->TRICERATOPS_INERTIA_THREE[0] == 0)
		{
			if (synth_params->TRICERATOPS_WAVE_THREE[0] < 2) // it's a saw or square
			{
				lpO[2]->f = new_frequency;
			}

			if (synth_params->TRICERATOPS_WAVE_THREE[0] == 2) // it's a sinus
			{
				sinewave_osc[2]->setRate(new_frequency);
			}
		}
			
		// is unison activated? detune one down, one up
		
		if (synth_params->TRICERATOPS_UNISON_ACTIVATE[0] == 1)
		{
			lpO[5]->f = lpO[2]->f + synth_params->TRICERATOPS_UNISON_THREE[0];
			lpO[2]->f -= synth_params->TRICERATOPS_UNISON_THREE[0];

			sinewave_osc[5]->setRate(new_frequency + synth_params->TRICERATOPS_UNISON_THREE[0]);
			sinewave_osc[2]->setRate(new_frequency - synth_params->TRICERATOPS_UNISON_THREE[0]);
		}

			// pulsewidth routes for squarewave
		
		if (synth_params->TRICERATOPS_WAVE_THREE[0] == 1)
		{

			routes = route_matrix[route_osc3_pw];
	
			routes += synth_params->TRICERATOPS_PULSEWIDTH_THREE [0]+0.5;

			if (routes < 0) routes = 0;
			if (routes > 1) routes = 1;
	
			lpO[2]->fPWM = routes;
			lpO[5]->fPWM = routes;
		}


	}

	float out = 0;
	float outleft = 0;
	float outright = 0;

	float out_left_1 = 0;
	float out_right_1 = 0;
	float out_1 = 0;

	float out_left_2 = 0;
	float out_right_2 = 0;
	float out_2 = 0;

	float out_left_3 = 0;
	float out_right_3 = 0;
	float out_3 = 0;


	if (synth_params->TRICERATOPS_ACTIVE_ONE[0]==1) // play oscillator one -------------------------------------------
	{

		float OSC1_pan = synth_params->TRICERATOPS_OSC1_PAN[0];

		float routes = route_matrix[route_osc1_pan];

		OSC1_pan += routes;

		float OSC_out = 0;

		if (synth_params->TRICERATOPS_WAVE_ONE[0]<2) 
		{
			float v=0;
			float fs=lpO[0]->f / lpO[0]->srate;

			// create waveform
			lpO[0]->p=lpO[0]->p+fs;

			// add BLEP at end of waveform
			if (lpO[0]->p>=1)
			{
				lpO[0]->p=lpO[0]->p-1.0;
				lpO[0]->v=0.0f;
				osc_AddBLEP(lpO[0], lpO[0]->p/fs,1.0f);
			}


			// add BLEP in middle of wavefor for squarewave
			if (!lpO[0]->v && lpO[0]->p>lpO[0]->fPWM && lpO[0]->type==OT_SQUARE)
			{
				lpO[0]->v=1.0f;
				osc_AddBLEP(lpO[0], (lpO[0]->p-lpO[0]->fPWM)/fs,-1.0f);
			}

			// sample value
			if (lpO[0]->type==OT_SAW)
				v=lpO[0]->p;
			else
				v=lpO[0]->v;

			// add BLEP buffer contents
			if (lpO[0]->nInit)
			{
				v+=lpO[0]->buffer[lpO[0]->iBuffer];
				lpO[0]->nInit--;
				if (++lpO[0]->iBuffer>=lpO[0]->cBuffer) lpO[0]->iBuffer=0;
			}

			OSC_out = do_3band(eq_left_1, v); 			
		}

		if (synth_params->TRICERATOPS_WAVE_ONE[0]==2) OSC_out = sinewave_osc[0]->tick();
		if (synth_params->TRICERATOPS_WAVE_ONE[0]==3) OSC_out = nixnoise->tick();
		
		if ( route_matrix[route_osc1_amp] > 0.0000001) 
		{
			OSC_out *= ( 1 - route_matrix[route_osc1_amp] );
		}


		if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0]==1)
		{
			out_left_1 = (OSC_out *synth_params->TRICERATOPS_VOLUME_ONE[0]) * (1 - OSC1_pan);	
			out_right_1 = (OSC_out *synth_params->TRICERATOPS_VOLUME_ONE[0]) * OSC1_pan;
		}
		else 
		{
			out_1 = (OSC_out * synth_params->TRICERATOPS_VOLUME_ONE[0]) * 0.75;
		}


		if (synth_params->TRICERATOPS_UNISON_ACTIVATE[0] == 1)  // UNISON ENABLED SO DO BOTH OSCILLATOR 0 and 3
		{

			OSC_out = 0;

			if (synth_params->TRICERATOPS_WAVE_ONE[0]<2) 

			{
				float v=0;
				float fs=lpO[3]->f / lpO[3]->srate;

				// create waveform
				lpO[3]->p=lpO[3]->p+fs;

				// add BLEP at end of waveform
				if (lpO[3]->p>=1)
				{
					lpO[3]->p=lpO[3]->p-1.0;
					lpO[3]->v=0.0f;
					osc_AddBLEP(lpO[3], lpO[3]->p/fs,1.0f);
				}

				// add BLEP in middle of wavefor for squarewave
				if (!lpO[3]->v && lpO[3]->p>lpO[3]->fPWM && lpO[3]->type==OT_SQUARE)
				{
					lpO[3]->v=1.0f;
					osc_AddBLEP(lpO[3], (lpO[3]->p-lpO[3]->fPWM)/fs,-1.0f);
				}

				// sample value
				if (lpO[3]->type==OT_SAW)
					v=lpO[3]->p;
				else
					v=lpO[3]->v;

				// add BLEP buffer contents
				if (lpO[3]->nInit)
				{
					v+=lpO[3]->buffer[lpO[3]->iBuffer];
					lpO[3]->nInit--;
					if (++lpO[3]->iBuffer>=lpO[3]->cBuffer) lpO[3]->iBuffer=0;
				}
	
				OSC_out = do_3band(eq_right_1, v);
			}

			if (synth_params->TRICERATOPS_WAVE_ONE[0]==2) OSC_out = sinewave_osc[3]->tick();
			if (synth_params->TRICERATOPS_WAVE_ONE[0]==3) OSC_out = nixnoise->tick();

			if ( route_matrix[route_osc1_amp] > 0.0000001) 
			{
				OSC_out *= ( 1 - route_matrix[route_osc1_amp] );
			}
			

			if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0]==1)
			{
				out_left_1 += (OSC_out *synth_params->TRICERATOPS_VOLUME_ONE[0]) * (1 - OSC1_pan);
				out_right_1 += (OSC_out *synth_params->TRICERATOPS_VOLUME_ONE[0]) * OSC1_pan;
			}
			else 
			{
				out_1 += (OSC_out * synth_params->TRICERATOPS_VOLUME_ONE[0]) * 0.75;
			}
		}
	}

	if (synth_params->TRICERATOPS_ACTIVE_TWO[0]==1) // play oscillator two -----------------------------------------------
	{

		float OSC2_pan = synth_params->TRICERATOPS_OSC2_PAN[0];

		float routes = route_matrix[route_osc2_pan];

		OSC2_pan += routes;

		float OSC_out = 0;

		if (synth_params->TRICERATOPS_WAVE_TWO[0]<2)
		{
			float v=0;
			float fs=lpO[1]->f / lpO[1]->srate;

			// create waveform
			lpO[1]->p=lpO[1]->p+fs;

			// add BLEP at end of waveform
			if (lpO[1]->p>=1)
			{
				lpO[1]->p=lpO[1]->p-1.0;
				lpO[1]->v=0.0f;
				osc_AddBLEP(lpO[1], lpO[1]->p/fs,1.0f);
			}

			// add BLEP in middle of wavefor for squarewave
			if (!lpO[1]->v && lpO[1]->p>lpO[1]->fPWM && lpO[1]->type==OT_SQUARE)
			{
				lpO[1]->v=1.0f;
				osc_AddBLEP(lpO[1], (lpO[1]->p-lpO[1]->fPWM)/fs,-1.0f);
			}

			// sample value
			if (lpO[1]->type==OT_SAW)
				v=lpO[1]->p;
			else
				v=lpO[1]->v;

			// add BLEP buffer contents
			if (lpO[1]->nInit)
			{
				v+=lpO[1]->buffer[lpO[1]->iBuffer];
				lpO[1]->nInit--;
				if (++lpO[1]->iBuffer>=lpO[1]->cBuffer) lpO[1]->iBuffer=0;
			}

			OSC_out = do_3band(eq_left_2, v);
		}

		if (synth_params->TRICERATOPS_WAVE_TWO[0]==2) OSC_out = sinewave_osc[1]->tick();
		if (synth_params->TRICERATOPS_WAVE_TWO[0]==3) OSC_out = nixnoise->tick();

		if ( route_matrix[route_osc2_amp] > 0.0000001) 
		{
			OSC_out *= ( 1 - route_matrix[route_osc2_amp] );
		}

		if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0]==1)
		{
			if (synth_params->TRICERATOPS_MODIFIER_RING[0]==1)
			{
				out_left_2 = 4*(OSC_out *synth_params->TRICERATOPS_VOLUME_TWO[0]) * (1 - OSC2_pan);
				out_right_2 = 4*(OSC_out *synth_params->TRICERATOPS_VOLUME_TWO[0]) * OSC2_pan;
			}
			else
			{
				out_left_2 = (OSC_out *synth_params->TRICERATOPS_VOLUME_TWO[0]) * (1 - OSC2_pan);
				out_right_2 = (OSC_out *synth_params->TRICERATOPS_VOLUME_TWO[0]) * OSC2_pan;
			}
		}
		else 
		{
			if (synth_params->TRICERATOPS_MODIFIER_RING[0]==1)
			{
				out_2 = 4*(OSC_out * synth_params->TRICERATOPS_VOLUME_TWO[0]) * 0.75;
			}
			else
			{
				out_2 = (OSC_out * synth_params->TRICERATOPS_VOLUME_TWO[0]) * 0.75;
			}
		}


		if (synth_params->TRICERATOPS_UNISON_ACTIVATE[0] == 1)  // UNISON ENABLED SO DO BOTH OSCILLATOR 1 and 4
		{
			OSC_out = 0;

			if (synth_params->TRICERATOPS_WAVE_TWO[0]<2)
			{
				float v=0;
				float fs=lpO[4]->f / lpO[4]->srate;

				// create waveform
				lpO[4]->p=lpO[4]->p+fs;

				// add BLEP at end of waveform
				if (lpO[4]->p>=1)
				{
					lpO[4]->p=lpO[4]->p-1.0;
					lpO[4]->v=0.0f;
					osc_AddBLEP(lpO[4], lpO[4]->p/fs,1.0f);
				}

				// add BLEP in middle of wavefor for squarewave
				if (!lpO[4]->v && lpO[4]->p>lpO[4]->fPWM && lpO[4]->type==OT_SQUARE)
				{
					lpO[4]->v=1.0f;
					osc_AddBLEP(lpO[4], (lpO[4]->p-lpO[4]->fPWM)/fs,-1.0f);
				}

				// sample value
				if (lpO[4]->type==OT_SAW)
					v=lpO[4]->p;
				else
					v=lpO[4]->v;

				// add BLEP buffer contents
				if (lpO[4]->nInit)
				{
					v+=lpO[4]->buffer[lpO[4]->iBuffer];
					lpO[4]->nInit--;
					if (++lpO[4]->iBuffer>=lpO[4]->cBuffer) lpO[4]->iBuffer=0;
				}
	
				OSC_out = do_3band(eq_right_2, v);
			}

			if (synth_params->TRICERATOPS_WAVE_TWO[0]==2) OSC_out = sinewave_osc[4]->tick();
			if (synth_params->TRICERATOPS_WAVE_TWO[0]==3) OSC_out = nixnoise->tick();

			if ( route_matrix[route_osc2_amp] > 0.0000001) 
			{
				OSC_out *= ( 1 - route_matrix[route_osc2_amp] );
			}
	
			if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0]==1)
			{
				out_left_2 += (OSC_out *synth_params->TRICERATOPS_VOLUME_TWO[0]) * (1 - OSC2_pan);
				out_right_2 += (OSC_out *synth_params->TRICERATOPS_VOLUME_TWO[0]) * OSC2_pan;
			}
			else 
			{
				out_2 += (OSC_out * synth_params->TRICERATOPS_VOLUME_TWO[0]) * 0.75;
			}
		}
	}


	if (synth_params->TRICERATOPS_ACTIVE_THREE[0]==1) // play oscillator three
	{
		float OSC_out = 0;

		float OSC3_pan = synth_params->TRICERATOPS_OSC3_PAN[0];

		float routes = route_matrix[route_osc3_pan];	

		OSC3_pan += routes;

		OSC_out = 0;
		if (synth_params->TRICERATOPS_WAVE_THREE[0]<2)
		{
			float v=0;
			float fs=lpO[2]->f / lpO[2]->srate;

			// create waveform
			lpO[2]->p=lpO[2]->p+fs;

			// add BLEP at end of waveform
			if (lpO[2]->p>=1)
			{
				lpO[2]->p=lpO[2]->p-1.0;
				lpO[2]->v=0.0f;
				osc_AddBLEP(lpO[2], lpO[2]->p/fs,1.0f);
			}

			// add BLEP in middle of wavefor for squarewave
			if (!lpO[2]->v && lpO[2]->p>lpO[2]->fPWM && lpO[2]->type==OT_SQUARE)
			{
				lpO[2]->v=1.0f;
				osc_AddBLEP(lpO[2], (lpO[2]->p-lpO[2]->fPWM)/fs,-1.0f);
			}

			// sample value
			if (lpO[2]->type==OT_SAW)
				v=lpO[2]->p;
			else
				v=lpO[2]->v;

			// add BLEP buffer contents
			if (lpO[2]->nInit)
			{
				v+=lpO[2]->buffer[lpO[2]->iBuffer];
				lpO[2]->nInit--;
				if (++lpO[2]->iBuffer>=lpO[2]->cBuffer) lpO[2]->iBuffer=0;
			}

			OSC_out += do_3band(eq_left_3, v);
		}

		if (synth_params->TRICERATOPS_WAVE_THREE[0]==2) OSC_out = sinewave_osc[2]->tick();
		if (synth_params->TRICERATOPS_WAVE_THREE[0]==3) OSC_out = nixnoise->tick();

		if ( route_matrix[route_osc3_amp] > 0.0000001) 
		{
			OSC_out *= ( 1 - route_matrix[route_osc3_amp] );
		}

		if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0]==1)
		{
			if (synth_params->TRICERATOPS_FM[0] == 1)
			{
				out_fm = (OSC_out * synth_params->TRICERATOPS_VOLUME_THREE[0] * 16) * env_filter_level;
			}
			if (synth_params->TRICERATOPS_FM[0] == 0)
			{
				out_left_3 = (OSC_out *synth_params->TRICERATOPS_VOLUME_THREE[0]) * (1 - OSC3_pan);
				out_right_3 = (OSC_out *synth_params->TRICERATOPS_VOLUME_THREE[0]) * OSC3_pan;;
			}
		}
		else 
		{
			if (synth_params->TRICERATOPS_FM[0] == 1)
			{
				out_fm = (OSC_out * synth_params->TRICERATOPS_VOLUME_THREE[0] * 16) * env_filter_level;
			} 
			if (synth_params->TRICERATOPS_FM[0] == 0)
			{
				out_3 =  (OSC_out * synth_params->TRICERATOPS_VOLUME_THREE[0]) * 0.75;
			}
		}


		if (synth_params->TRICERATOPS_UNISON_ACTIVATE[0] == 1)  // UNISON ENABLED SO DO BOTH OSCILLATOR 2 and 5
		{
			OSC_out = 0;

			if (synth_params->TRICERATOPS_WAVE_THREE[0]<2)
			{
				float v=0;
				float fs=lpO[5]->f / lpO[5]->srate;

				// create waveform
				lpO[5]->p=lpO[5]->p+fs;

				// add BLEP at end of waveform
				if (lpO[5]->p>=1)
				{
					lpO[5]->p=lpO[5]->p-1.0;
					lpO[5]->v=0.0f;
					osc_AddBLEP(lpO[5], lpO[5]->p/fs,1.0f);
				}

				// add BLEP in middle of wavefor for squarewave
				if (!lpO[5]->v && lpO[5]->p>lpO[5]->fPWM && lpO[5]->type==OT_SQUARE)
				{
					lpO[5]->v=1.0f;
					osc_AddBLEP(lpO[5], (lpO[5]->p-lpO[5]->fPWM)/fs,-1.0f);
				}

				// sample value
				if (lpO[5]->type==OT_SAW)
					v=lpO[5]->p;
				else
					v=lpO[5]->v;

				// add BLEP buffer contents
				if (lpO[5]->nInit)
				{
					v+=lpO[5]->buffer[lpO[5]->iBuffer];
					lpO[5]->nInit--;
					if (++lpO[5]->iBuffer>=lpO[5]->cBuffer) lpO[5]->iBuffer=0;
				}
	
				OSC_out += do_3band(eq_right_3, v);
			}

			if (synth_params->TRICERATOPS_WAVE_THREE[0]==2) OSC_out = sinewave_osc[5]->tick();
			if (synth_params->TRICERATOPS_WAVE_THREE[0]==3) OSC_out = nixnoise->tick();

			if ( route_matrix[route_osc3_amp] > 0.0000001) 
			{
				OSC_out *= ( 1 - route_matrix[route_osc3_amp] );
			}

			if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0]==1)
			{
				if (synth_params->TRICERATOPS_FM[0] == 1)
				{
					out_fm = (OSC_out * synth_params->TRICERATOPS_VOLUME_THREE[0] * 16) * env_filter_level;
				}
				if (synth_params->TRICERATOPS_FM[0] == 0)
				{
					out_left_3 += (OSC_out * synth_params->TRICERATOPS_VOLUME_THREE[0]) * (1 - OSC3_pan);
					out_right_3 += (OSC_out * synth_params->TRICERATOPS_VOLUME_THREE[0]) * OSC3_pan;;
				}
			}
			else 
			{
				if (synth_params->TRICERATOPS_FM[0] == 1)
				{
					out_fm = (OSC_out * synth_params->TRICERATOPS_VOLUME_THREE[0] * 16) * env_filter_level;
				} 
				if (synth_params->TRICERATOPS_FM[0] == 0)
				{
					out_3 +=  (OSC_out * synth_params->TRICERATOPS_VOLUME_THREE[0]) * 0.75;
				}
			}
		}

	}

	outleft = out_left_1 + out_left_2 + out_left_3;
	outright = out_right_1 + out_right_2 + out_right_3;
	out = out_1 + out_2 + out_3;

	if (synth_params->TRICERATOPS_SYNC[0] == 1) 
	{
		if (lpO[0]->p<0.1 ) { lpO[1]->p = 0; sinewave_osc[1]->phase = 0; }
		if (lpO[3]->p<0.1 ) { lpO[4]->p = 0; sinewave_osc[4]->phase = 0; }
	}
	
	
	//------------ filter

		if (synth_params->TRICERATOPS_FILTER_MODE != 0)
		{
			float routes = 0;

			if (synth_params->TRICERATOPS_LFO1_ROUTE_ONE_DEST[0]==1)
				routes += synth_params->TRICERATOPS_LFO1_ROUTE_ONE[0]* lfo1;
			if (synth_params->TRICERATOPS_LFO2_ROUTE_ONE_DEST[0]==1)
				routes += synth_params->TRICERATOPS_LFO2_ROUTE_ONE[0]* lfo2;
			if (synth_params->TRICERATOPS_LFO3_ROUTE_ONE_DEST[0]==1)
				routes += synth_params->TRICERATOPS_LFO3_ROUTE_ONE[0]* lfo3;
			if (synth_params->TRICERATOPS_LFO1_ROUTE_TWO_DEST[0]==1)
				routes += synth_params->TRICERATOPS_LFO1_ROUTE_TWO[0]* lfo1;
			if (synth_params->TRICERATOPS_LFO2_ROUTE_TWO_DEST[0]==1)
				routes += synth_params->TRICERATOPS_LFO2_ROUTE_TWO[0]* lfo2;
			if (synth_params->TRICERATOPS_LFO3_ROUTE_TWO_DEST[0]==1)
				routes += synth_params->TRICERATOPS_LFO3_ROUTE_TWO[0]* lfo3;	

			routes += synth_params->TRICERATOPS_LFO1_FILTER[0] * lfo1 * 12;
			routes += synth_params->TRICERATOPS_LFO2_FILTER[0] * lfo2 * 12;
			routes += synth_params->TRICERATOPS_LFO3_FILTER[0] * lfo3 * 12;

			routes *= 0.25;

			routes += (synth_params->TRICERATOPS_FILTER_KEY_FOLLOW[0] * (osc_frequency * 0.001));

			if (synth_params->TRICERATOPS_ADSR1_ROUTE_ONE_DEST[0]==1)
				routes += synth_params->TRICERATOPS_ADSR1_ROUTE_ONE[0] * env_amp_level * (velocity * 0.01);

			if (synth_params->TRICERATOPS_ADSR2_ROUTE_ONE_DEST[0]==1)
				routes += synth_params->TRICERATOPS_ADSR2_ROUTE_ONE[0] * env_filter_level * (velocity * 0.01);

			if (synth_params->TRICERATOPS_ADSR1_ROUTE_TWO_DEST[0]==1)
				routes += synth_params->TRICERATOPS_ADSR1_ROUTE_TWO[0] * env_amp_level * (velocity * 0.01);
	
			if (synth_params->TRICERATOPS_ADSR2_ROUTE_TWO_DEST[0]==1)
				routes += synth_params->TRICERATOPS_ADSR2_ROUTE_TWO[0] * env_filter_level * (velocity * 0.01);

			routes += synth_params->TRICERATOPS_FILTER_FREQUENCY[0];
			if (synth_params->TRICERATOPS_MODIFIER_DIRT[0] > 0)
				routes += (nixnoise->tick()*synth_params->TRICERATOPS_MODIFIER_DIRT[0] * 0.001);

			float frequency = routes*routes*routes*routes; // fast_pow(routes,4); // not fast enough meh
			cutoff_modded = frequency;

			routes = 0;

			if (synth_params->TRICERATOPS_LFO1_ROUTE_ONE_DEST[0]==2)
				routes += synth_params->TRICERATOPS_LFO1_ROUTE_ONE[0]* lfo1;

			if (synth_params->TRICERATOPS_LFO2_ROUTE_ONE_DEST[0]==2)
				routes += synth_params->TRICERATOPS_LFO2_ROUTE_ONE[0]* lfo2;

			if (synth_params->TRICERATOPS_LFO3_ROUTE_ONE_DEST[0]==2)
				routes += synth_params->TRICERATOPS_LFO3_ROUTE_ONE[0]* lfo3;

			if (synth_params->TRICERATOPS_LFO1_ROUTE_TWO_DEST[0]==2)
				routes += synth_params->TRICERATOPS_LFO1_ROUTE_TWO[0]* lfo1;
	
			if (synth_params->TRICERATOPS_LFO2_ROUTE_TWO_DEST[0]==2)
				routes += synth_params->TRICERATOPS_LFO2_ROUTE_TWO[0]* lfo2;
	
			if (synth_params->TRICERATOPS_LFO3_ROUTE_TWO_DEST[0]==2)
				routes += synth_params->TRICERATOPS_LFO3_ROUTE_TWO[0]* lfo3;

			routes *= 0.25;

			if (synth_params->TRICERATOPS_ADSR1_ROUTE_ONE_DEST[0]==2)
				routes += synth_params->TRICERATOPS_ADSR1_ROUTE_ONE[0] * env_amp_level;
	
			if (synth_params->TRICERATOPS_ADSR2_ROUTE_ONE_DEST[0]==2)
				routes += synth_params->TRICERATOPS_ADSR2_ROUTE_ONE[0] * env_filter_level;
	
			if (synth_params->TRICERATOPS_ADSR1_ROUTE_TWO_DEST[0]==2)
				routes += synth_params->TRICERATOPS_ADSR1_ROUTE_TWO[0] * env_amp_level;

			if (synth_params->TRICERATOPS_ADSR2_ROUTE_TWO_DEST[0]==2)
				routes += synth_params->TRICERATOPS_ADSR2_ROUTE_TWO[0] * env_filter_level;
	
			routes += synth_params->TRICERATOPS_FILTER_RESONANCE[0];
	
			float resonance = routes;
			float in = 0;

			if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0] == 0)
			{	
				in = (out * 0.8);
			}
			else 
			{
				in = (outleft * 0.5);
			}

			if (frequency > 0.6) { frequency = 0.6; }
				
			
			float in_left = in;
			float in_right = 0;

			if ( synth_params->TRICERATOPS_FILTER_MODE[0] > 0 && synth_params->TRICERATOPS_FILTER_MODE[0] < 4)
			{

			// IN MONO MODE WE ONLY NEED ONE FILTER SO THIS SERVES AS BOTH MONO AND LEFT CHANNEL FOR STEREO MODE

			// Set coefficients given frequency & resonance [0.0...1.0]
	
			q_left = 1.0f - frequency;
			pc_left = frequency + 0.8f * frequency * q_left;
			f_left= pc_left + pc_left - 1.0f;
			q_left = resonance * (1.0f + 0.5f * q_left * (1.0f - q_left + 5.6f * q_left * q_left));
	

			// Filter (in [-1.0...+1.0])

			in -= q_left * bf4_left;                          //feedback
			t1_left = bf1_left;  bf1_left = (in + bf0_left) * pc_left - bf1_left * f_left;
			t2_left = bf2_left;  bf2_left = (bf1_left + t1_left) * pc_left - bf2_left * f_left;
			t1_left = bf3_left;  bf3_left = (bf2_left + t2_left) * pc_left - bf3_left * f_left;
		        bf4_left = (bf3_left + t1_left) * pc_left - bf4_left * f_left;
			bf4_left = bf4_left - bf4_left * bf4_left * bf4_left * 0.166667f;    //clipping
			bf0_left = in;

			// Lowpass  output:  bf4_left
			// Highpass output:  in - bf4_left;
			// Bandpass output:  3.0f * (bf3_left - bf4_left);

			if (  isnan(bf4_left) ) {bf1_left = 0; bf2_left = 0; bf3_left = 0; bf4_left = 0; in = 0; out = 0;}
			

			// STEREO MODE ENABLED - NEED TWO FILTERS, HERES HE RIGHT CHANNEL ONE

			if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0] == 1)
			{

				in = (outright * 0.5);
				in_right = in;
	
				// Set coefficients given frequency & resonance [0.0...1.0]
	
				q_right = 1.0f - frequency;
				pc_right = frequency + 0.4f * frequency * q_right;
				f_right= pc_right + pc_right - 1.0f;
				q_right = resonance * (1.0f + 0.5f * q_right * (1.0f - q_right + 5.6f * q_right * q_right));
	

				// Filter (in [-1.0...+1.0])

				in -= q_right * bf4_right;                          //feedback
				t1_right = bf1_right;  bf1_right = (in + bf0_right) * pc_right - bf1_right * f_right;
				t2_right = bf2_right;  bf2_right = (bf1_right + t1_right) * pc_right - bf2_right * f_right;
				t1_right = bf3_right;  bf3_right = (bf2_right + t2_right) * pc_right - bf3_right * f_right;
			        bf4_right = (bf3_right + t1_right) * pc_right - bf4_right * f_right;
				bf4_right = bf4_right - bf4_right * bf4_right * bf4_right * 0.166667f;    //clipping
				bf0_right = in;

		
				// Lowpass  output:  bf4_right
				// Highpass output:  in - bf4_right;
				// Bandpass output:  3.0f * (bf3_right - bf4_right);

				if (  isnan(bf4_right) ) {bf1_right = 0; bf2_right = 0; bf3_right = 0; bf4_right = 0; in = 0; out = 0;}
			}
		

		}


	//------------ return audio

			if (synth_params->TRICERATOPS_FILTER_MODE[0] == 1) // low pass
			{
				out_left[pos] += (bf4_left * env_amp_level_db) * synth_params->TRICERATOPS_MASTER_VOLUME[0];

				if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0] == 0)
					out_right[pos] = out_left[pos];

					else out_right[pos] += (bf4_right * env_amp_level_db)
						* synth_params->TRICERATOPS_MASTER_VOLUME[0];
			}

			if (synth_params->TRICERATOPS_FILTER_MODE[0] == 2) // high pass
			{
				out_left[pos] += ((in_left-bf4_left) * env_amp_level_db) * synth_params->TRICERATOPS_MASTER_VOLUME[0];

				if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0] == 0) 
				{
					out_right[pos] = out_left[pos];
				}
				else
				{
					out_right[pos] += ((in_right-bf4_right) * env_amp_level_db)
						* synth_params->TRICERATOPS_MASTER_VOLUME[0];
				}
			}

			if (synth_params->TRICERATOPS_FILTER_MODE[0] == 3) // band pass
			{
				out_left[pos] += ((3.0F * (bf3_left-bf4_left)) * env_amp_level_db)
					* synth_params->TRICERATOPS_MASTER_VOLUME[0];

				if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0] == 0)
				out_right[pos] = out_left[pos];

					else out_right[pos] +=  ((3.0F * (bf3_right-bf4_right)) * env_amp_level_db)
						* synth_params->TRICERATOPS_MASTER_VOLUME[0];
			}

			if (synth_params->TRICERATOPS_FILTER_MODE[0] == 4)
			{

				float crossfade = (cutoff_modded * 6.5);

				if (crossfade < 0) crossfade=0;
				if (crossfade > 4) crossfade=4;

				float fade = 0;

				int vowel1 = 0;
				int vowel2 = 0;
	
				if (crossfade >= 0 &&
				crossfade < 1)
				{
					fade = crossfade;
					vowel1 = 1;
					vowel2 = 0;	
				}	

				if (crossfade >= 1 &&
				crossfade < 2)
				{
					fade = 1 - (crossfade - 1);
					vowel1 = 1;
					vowel2 = 2;	
				}	

				if (crossfade >= 2 &&
					crossfade < 3)
				{
					fade = crossfade - 2;
					vowel1 = 3;
					vowel2 = 2;	
				}

				if (crossfade >= 3 &&
				crossfade <= 4)
				{
					fade = 1 - (crossfade - 3);
					vowel1 = 3;
					vowel2 = 4;	
				}


				morph_coeff[0] = (float)( (coeff[vowel1][0]
					-coeff[vowel2][0])*fade)
					+coeff[vowel2][0];

				morph_coeff[1] = (float)( (coeff[vowel1][1]
					-coeff[vowel2][1])*fade)
					+coeff[vowel2][1];

				morph_coeff[2] = (float)( (coeff[vowel1][2]
					-coeff[vowel2][2])*fade)
					+coeff[vowel2][2];

				morph_coeff[3] = (float)( (coeff[vowel1][3]
					-coeff[vowel2][3])*fade)
					+coeff[vowel2][3];

				morph_coeff[4] = (float)( (coeff[vowel1][4]
					-coeff[vowel2][4])*fade)
					+coeff[vowel2][4];

				morph_coeff[5] = (float)( (coeff[vowel1][5]
					-coeff[vowel2][5])*fade)
					+coeff[vowel2][5];

				morph_coeff[6] = (float)( (coeff[vowel1][6]
					-coeff[vowel2][6])*fade)
					+coeff[vowel2][6];

				morph_coeff[7] = (float)( (coeff[vowel1][7]
					-coeff[vowel2][7])*fade)
					+coeff[vowel2][7];

				morph_coeff[8] = (float)( (coeff[vowel1][8]
					-coeff[vowel2][8])*fade)
					+coeff[vowel2][8];
	
				morph_coeff[9] = (float)( (coeff[vowel1][9]
					-coeff[vowel2][9])*fade)
					+coeff[vowel2][9];

				morph_coeff[10] = (float)( (coeff[vowel1][10]
					-coeff[vowel2][10])*fade)
					+coeff[vowel2][10];

				if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0] == 1)  out = (outleft + outright) * 0.5;

				float res = (float) ( morph_coeff[0]  * out +
				morph_coeff[1]  *memory[0] +  
				morph_coeff[2]  *memory[1] +
				morph_coeff[3]  *memory[2] +
				morph_coeff[4]  *memory[3] +
				morph_coeff[5]  *memory[4] +
				morph_coeff[6]  *memory[5] +
				morph_coeff[7]  *memory[6] +
				morph_coeff[8]  *memory[7] +
				morph_coeff[9]  *memory[8] +
				morph_coeff[10] *memory[9] );

				if (std::isnan(in))
				{
					res = 0;
					for (int x=0; x<11; x++) memory[x] = 0;
				}
	
				memory[9] = memory[8];
				memory[8] = memory[7];
				memory[7] = memory[6];
				memory[6] = memory[5];
				memory[5] = memory[4];
				memory[4] = memory[3];
				memory[3] = memory[2];
				memory[2] = memory[1];                     
				memory[1] = memory[0];
				memory[0] = (float) res;

				out_left[pos] += res * env_amp_level_db
					* synth_params->TRICERATOPS_MASTER_VOLUME[0];
				out_right[pos] = out_left[pos];
			}

		}

		if (synth_params->TRICERATOPS_FILTER_MODE[0] == 0) // BYPASS THE FILTER
		{
			if (synth_params->TRICERATOPS_MODIFIER_STEREO_MODE[0] == 0)  
			{
				out_left[pos] += ((out * env_amp_level_db) * synth_params->TRICERATOPS_MASTER_VOLUME[0])*0.3;
				out_right[pos] = out_left[pos];
			}
			else
			{
				out_left[pos] = ((outleft * env_amp_level_db) * synth_params->TRICERATOPS_MASTER_VOLUME[0])*0.3;
				out_right[pos] = ((outright * env_amp_level_db) * synth_params->TRICERATOPS_MASTER_VOLUME[0])*0.3;
			}

		}
		
		if (synth_params->TRICERATOPS_AMP_DRIVE[0] > 0) // Saturated Clipping Distortion Enabled
		{
			float out_left_atanClip = (2/M_PI) * atan(out_left[pos] * ((0.1+synth_params->TRICERATOPS_AMP_DRIVE[0])*100)); 
			float out_right_atanClip = (2/M_PI) * atan(out_right[pos] * ((0.1+synth_params->TRICERATOPS_AMP_DRIVE[0])*100));

			out_left[pos] = out_left_atanClip * 0.25;
			out_right[pos] = out_right_atanClip * 0.25;
		}
	}	
	


	return;
}

//----------------------------------------------------------

osc_t* synth::osc_Create(double rate)
{
	osc_t *lpO=(osc_t*)malloc(sizeof(osc_t));

	// frequency
	lpO->f=440;
	lpO->p=0;
	lpO->v=0;

	// stuff
	lpO->bSync=false;
	lpO->fPWM=0.5;
	lpO->type=OT_SAW;
	lpO->srate = rate;

	// buffer
	lpO->cBuffer = 1024; //gMinBLEP.c/KTABLE;
	lpO->buffer=(double*)malloc(sizeof(double)*lpO->cBuffer);
	lpO->iBuffer=0;
	lpO->nInit=0;

	return lpO;
}

#define LERP(A,B,F) ((B-A)*F+A)

//----------------------------------------------------------

// add impulse into buffer
void synth::osc_AddBLEP(osc_t *lpO, double offset, double amp)
{
	if (offset > 0.99) offset=0.99;

	int i;
	double *lpOut=lpO->buffer+lpO->iBuffer;
	double *lpIn=gMinBLEP.lpTable+(uint)(KTABLE*offset);
	double frac=fmod(KTABLE*offset,1.0);
	int cBLEP=(gMinBLEP.c/KTABLE)-1;
	double *lpBufferEnd=lpO->buffer+lpO->cBuffer;
	double f;

	if (frac > 0.99) frac = 0.99;

	// add
	for (i=0; i<lpO->nInit; i++, lpIn+=KTABLE, lpOut++)
	{
		if (lpOut>=lpBufferEnd) lpOut=lpO->buffer;
		f=LERP(lpIn[0],lpIn[1],frac);
		*lpOut+=amp*(1-f);
	}

	// copy
	for (; i<cBLEP; i++, lpIn+=KTABLE, lpOut++)
	{
		if (lpOut>=lpBufferEnd) lpOut=lpO->buffer;
		f=LERP(lpIn[0],lpIn[1],frac);
		*lpOut=amp*(1-f);
	}

	lpO->nInit=cBLEP;
}

//----------------------------------------------------------

// play waveform
float synth::osc_Play(osc_t *lpO)
{

double v;
double fs=lpO->f / lpO->srate;

	// create waveform
	lpO->p=lpO->p+fs;

	// add BLEP at end of waveform
	if (lpO->p>=1)
	{
		lpO->p=lpO->p-1.0;
		lpO->v=0.0f;
		osc_AddBLEP(lpO, lpO->p/fs,1.0f);
	}


	// add BLEP in middle of wavefor for squarewave
	if (!lpO->v && lpO->p>lpO->fPWM && lpO->type==OT_SQUARE)
	{
		lpO->v=1.0f;
		osc_AddBLEP(lpO, (lpO->p-lpO->fPWM)/fs,-1.0f);
	}
		

	// sample value
	if (lpO->type==OT_SAW)
		v=lpO->p;
	else
		v=lpO->v;

	// add BLEP buffer contents
	if (lpO->nInit)
	{
		v+=lpO->buffer[lpO->iBuffer];
		lpO->nInit--;
		if (++lpO->iBuffer>=lpO->cBuffer) lpO->iBuffer=0;
	}
	
	return v;
}


//----------------------------------

void synth::minBLEP_Free()
{
	free(gMinBLEP.lpTable);
}

//----------------------------------

void synth::init_oscillators(double sample_rate)
{
	for (int OSC=0; OSC<6; ++OSC)
	{
		rate = sample_rate;
		lpO[OSC] = osc_Create(rate);
		lpO[OSC]->type = OT_SAW;
		lpO[OSC]->bSync = false;

		sinewave_osc[OSC] = new LFO(rate);
		sinewave_osc[OSC]->setWaveform(LFO::sinus);
	}
	
	// Initialize EQs
				
	eq_left_1 = new EQSTATE();
	init_3band_state(eq_left_1,220,5000,sample_rate);	
	eq_left_1->lg = 0.0; // BASS
	eq_left_1->mg = 1.0; // MIDS
	eq_left_1->hg = 1.0; // HIGHS
		
	eq_right_1 = new EQSTATE();
	init_3band_state(eq_right_1,220,5000,sample_rate);		
	eq_right_1->lg = 0.0; // BASS
	eq_right_1->mg = 1.0; // MIDS
	eq_right_1->hg = 1.0; // HIGHS 

	eq_left_2 = new EQSTATE();
	init_3band_state(eq_left_2,220,5000,sample_rate);	
	eq_left_2->lg = 0.0; // BASS
	eq_left_2->mg = 1.0; // MIDS
	eq_left_2->hg = 1.0; // HIGHS
		
	eq_right_2 = new EQSTATE();
	init_3band_state(eq_right_2,220,5000,sample_rate);		
	eq_right_2->lg = 0.0; // BASS
	eq_right_2->mg = 1.0; // MIDS
	eq_right_2->hg = 1.0; // HIGHS 

	eq_left_3 = new EQSTATE();
	init_3band_state(eq_left_3,220,5000,sample_rate);	
	eq_left_3->lg = 0.0; // BASS
	eq_left_3->mg = 1.0; // MIDS
	eq_left_3->hg = 1.0; // HIGHS
		
	eq_right_3 = new EQSTATE();
	init_3band_state(eq_right_3,220,5000,sample_rate);		
	eq_right_3->lg = 0.0; // BASS
	eq_right_3->mg = 1.0; // MIDS
	eq_right_3->hg = 1.0; // HIGHS 
	 
}




