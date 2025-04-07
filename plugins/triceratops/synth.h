
#include <iostream>
#include <sstream>
#include <cstdint>
#include "fastmaths.hpp"
#include "lfo.h"
#include "eq.h"

using namespace std;

const int route_none = 0;
const int route_cutoff = 1;
const int route_res = 2;
const int route_OSC1 = 3;
const int route_OSC2 = 4;
const int route_OSC3 = 5;
const int route_pw1 = 6;
const int route_pw2 = 7;
const int route_pw3 = 8;
const int route_pan1 = 9;
const int route_pan2 = 10;
const int route_pan3 = 11;
const int route_lfo1_sp = 12;
const int route_lfo2_sp = 13;
const int route_lfo3_sp = 14;

// parameters

typedef struct
{
	float* TRICERATOPS_MASTER_VOLUME;
	float* TRICERATOPS_MASTER_TUNE;
	float* TRICERATOPS_AMP_DRIVE;
	float* TRICERATOPS_FILTER_MODE;
	float* TRICERATOPS_FILTER_FREQUENCY;
	float* TRICERATOPS_FILTER_RESONANCE;
	float* TRICERATOPS_FILTER_KEY_FOLLOW;
	float* TRICERATOPS_LEGATO;
	float* TRICERATOPS_SYNC;
	float* TRICERATOPS_WARMTH;
	float* TRICERATOPS_FM;
	float* TRICERATOPS_PANIC;

	float* TRICERATOPS_ACTIVE_ONE;
	float* TRICERATOPS_VOLUME_ONE;
	float* TRICERATOPS_PULSEWIDTH_ONE;
	float* TRICERATOPS_WAVE_ONE;
	float* TRICERATOPS_OCTAVE_ONE;
	float* TRICERATOPS_DETUNE_ONE;
	float* TRICERATOPS_DETUNE_CENTRE_ONE;
	float* TRICERATOPS_INERTIA_ONE;

	float* TRICERATOPS_ACTIVE_TWO;
	float* TRICERATOPS_VOLUME_TWO;
	float* TRICERATOPS_PULSEWIDTH_TWO;
	float* TRICERATOPS_WAVE_TWO;
	float* TRICERATOPS_OCTAVE_TWO;
	float* TRICERATOPS_DETUNE_TWO;
	float* TRICERATOPS_DETUNE_CENTRE_TWO;
	float* TRICERATOPS_INERTIA_TWO;

	float* TRICERATOPS_ACTIVE_THREE;
	float* TRICERATOPS_VOLUME_THREE;
	float* TRICERATOPS_PULSEWIDTH_THREE;
	float* TRICERATOPS_WAVE_THREE;
	float* TRICERATOPS_OCTAVE_THREE;
	float* TRICERATOPS_DETUNE_THREE;
	float* TRICERATOPS_DETUNE_CENTRE_THREE;
	float* TRICERATOPS_INERTIA_THREE;

	float* TRICERATOPS_ATTACK_ONE;
	float* TRICERATOPS_DECAY_ONE;
	float* TRICERATOPS_SUSTAIN_ONE;
	float* TRICERATOPS_RELEASE_ONE;
	float* TRICERATOPS_ADSR1_ROUTE_ONE;
	float* TRICERATOPS_ADSR1_ROUTE_ONE_DEST;
	float* TRICERATOPS_ADSR1_ROUTE_TWO;
	float* TRICERATOPS_ADSR1_ROUTE_TWO_DEST;

	float* TRICERATOPS_ATTACK_TWO;
	float* TRICERATOPS_DECAY_TWO;
	float* TRICERATOPS_SUSTAIN_TWO;
	float* TRICERATOPS_RELEASE_TWO;
	float* TRICERATOPS_ADSR2_ROUTE_ONE;
	float* TRICERATOPS_ADSR2_ROUTE_ONE_DEST;
	float* TRICERATOPS_ADSR2_ROUTE_TWO;
	float* TRICERATOPS_ADSR2_ROUTE_TWO_DEST;

	float* TRICERATOPS_ATTACK_THREE;
	float* TRICERATOPS_DECAY_THREE;
	float* TRICERATOPS_SUSTAIN_THREE;
	float* TRICERATOPS_RELEASE_THREE;
	float* TRICERATOPS_ADSR3_LFO1_AMOUNT;
	float* TRICERATOPS_ADSR3_LFO2_AMOUNT;
	float* TRICERATOPS_ADSR3_LFO3_AMOUNT;

	float* TRICERATOPS_LFO1_RETRIG;
	float* TRICERATOPS_LFO1_SPEED;
	float* TRICERATOPS_LFO1_WAVE;
	float* TRICERATOPS_LFO1_OSC1_AMOUNT;
	float* TRICERATOPS_LFO1_OSC2_AMOUNT;
	float* TRICERATOPS_LFO1_OSC3_AMOUNT;
	float* TRICERATOPS_LFO1_FILTER;
	float* TRICERATOPS_LFO1_ROUTE_ONE;
	float* TRICERATOPS_LFO1_ROUTE_ONE_DEST;
	float* TRICERATOPS_LFO1_ROUTE_TWO;
	float* TRICERATOPS_LFO1_ROUTE_TWO_DEST;

	float* TRICERATOPS_LFO2_RETRIG;
	float* TRICERATOPS_LFO2_SPEED;
	float* TRICERATOPS_LFO2_WAVE;
	float* TRICERATOPS_LFO2_OSC1_AMOUNT;
	float* TRICERATOPS_LFO2_OSC2_AMOUNT;
	float* TRICERATOPS_LFO2_OSC3_AMOUNT;
	float* TRICERATOPS_LFO2_FILTER;
	float* TRICERATOPS_LFO2_ROUTE_ONE;
	float* TRICERATOPS_LFO2_ROUTE_ONE_DEST;
	float* TRICERATOPS_LFO2_ROUTE_TWO;
	float* TRICERATOPS_LFO2_ROUTE_TWO_DEST;

	float* TRICERATOPS_LFO3_RETRIG;
	float* TRICERATOPS_LFO3_SPEED;
	float* TRICERATOPS_LFO3_WAVE;
	float* TRICERATOPS_LFO3_OSC1_AMOUNT;
	float* TRICERATOPS_LFO3_OSC2_AMOUNT;
	float* TRICERATOPS_LFO3_OSC3_AMOUNT;
	float* TRICERATOPS_LFO3_FILTER;
	float* TRICERATOPS_LFO3_ROUTE_ONE;
	float* TRICERATOPS_LFO3_ROUTE_ONE_DEST;
	float* TRICERATOPS_LFO3_ROUTE_TWO;
	float* TRICERATOPS_LFO3_ROUTE_TWO_DEST;

	float* TRICERATOPS_FX_ECHO_ACTIVE;
	float* TRICERATOPS_FX_ECHO_SPEED;
	float* TRICERATOPS_FX_ECHO_DECAY;
	float* TRICERATOPS_FX_ECHO_EQ_LOW;
	float* TRICERATOPS_FX_ECHO_EQ_MID;
	float* TRICERATOPS_FX_ECHO_EQ_HIGH;

	float* TRICERATOPS_UNISON_ACTIVATE;
	float* TRICERATOPS_UNISON_ONE;
	float* TRICERATOPS_UNISON_TWO;
	float* TRICERATOPS_UNISON_THREE;

	float* TRICERATOPS_MODIFIER_DIRT;

	float* TRICERATOPS_FX_REVERB_ACTIVE;
	float* TRICERATOPS_FX_REVERB_DECAY;
	float* TRICERATOPS_FX_REVERB_MIX;

	float* TRICERATOPS_MODIFIER_STEREO_MODE;
	float* TRICERATOPS_OSC1_PAN;
	float* TRICERATOPS_OSC2_PAN;
	float* TRICERATOPS_OSC3_PAN;
	float* TRICERATOPS_MODIFIER_RING;

	float* TRICERATOPS_CATEGORY;
	float* TRICERATOPS_PITCH_BEND_RANGE;
	float* TRICERATOPS_MIDI_CHANNEL;
	
} params;

///------------ oscillator  structs

typedef enum
{
	OT_SAW,
	OT_SQUARE,
} oscwave_t;

typedef struct
{
	double f; // frequency
	double p; // phase
	double v; // output

	bool bSync;		// hardsync on/off
	float fSync;	// hardsync frequency
	float fPWM;		// pulse width
	float srate;

	int type; // square or saw
	
	double *buffer; // circular output buffer
	int cBuffer;	// buffer size
	int iBuffer;	// current buffer position
	int nInit;		// amount of initialized entries
} osc_t;

#define KTABLE 64 // BLEP table oversampling factor

typedef struct
{
	double *lpTable;
	int c;
} minbleptable_t;

//----------------- main synth class

class synth
{
	// public stuff


	public:

	params* synth_params;

	string bundle_path;
	
	EQSTATE* eq_left_1;
	EQSTATE* eq_right_1;
	EQSTATE* eq_left_2;
	EQSTATE* eq_right_2;
	EQSTATE* eq_left_3;
	EQSTATE* eq_right_3;

	static const int env_state_attack=1; 
	static const int env_state_decay=2;
	static const int env_state_sustain=3;
	static const int env_state_release=4;
	static const int env_state_wait=5;
	static const int env_state_dormant=6; 
	static const int env_state_kill=7; 
	static const int env_state_begin=8; 

	synth();
	~synth();
	void run(float*, float*, uint32_t);

	float** lfo1_out;
	float** lfo2_out;
	float** lfo3_out;

	float out_fm;

	float env_amp_level_db;
	int midi_key;
	int velocity;

	float env_amp_level;
	int env_amp_state;
	float env_filter_level;
	int env_filter_state;
	float env_lfo_level;
	int env_lfo_state;

	// amp stuff

	float* master_volume;
	float* filter_frequency;
	float* filter_resonance;

	// oscillator stuff

	osc_t* lpO[6];
	LFO* sinewave_osc[6];
	noise* nixnoise;

	int OSC_count_one;

	float osc_frequency;
	float dc1,dc2,dc3;

	inertia inertia_one;
	inertia inertia_two;
	inertia inertia_three;

	int update_counter;
	int update_speed;

	float* pitch_bend;
	float* channel_after_touch;

	bool stereo_mode;

	// resonant filter stuff - left

 	float buf0_left,buf1_left;
  	float f_left, pc_left, q_left;             //filter coefficients
  	float bf0_left, bf1_left, bf2_left, bf3_left, bf4_left;  //filter buffers (beware denormals!)
  	float t1_left, t2_left;              //temporary buffers

  	float selectivity_left;
	float gain1_left;
	float gain2_left;
	float  ratio_left;
	float cap_left;  

	// resonant filter stuff - right

 	float buf0_right,buf1_right;
  	float f_right, pc_right, q_right;             //filter coefficients
  	float bf0_right, bf1_right, bf2_right, bf3_right, bf4_right;  //filter buffers (beware denormals!)
  	float t1_right, t2_right;              //temporary buffers

  	float selectivity_right;
	float gain1_right;
	float gain2_right;
	float  ratio_right;
	float cap_right;  
	
	float rate;
	float s_rate;

	void init_oscillators(double);

	// private stuff

	private:

	double memory[11];
	double cutoff_modded;
	double morph_coeff[11];


	osc_t *osc_Create(double);
	void osc_AddBLEP(osc_t *lpO, double offset, double amp);
	float osc_Play(osc_t *lpO);
	void osc_Free();
	void minBLEP_Free();
	minbleptable_t gMinBLEP;



};







