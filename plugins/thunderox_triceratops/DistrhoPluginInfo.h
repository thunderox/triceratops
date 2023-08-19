
#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_NAME "thunderox_triceratops"
#define DISTRHO_PLUGIN_NUM_INPUTS 0
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2
#define DISTRHO_PLUGIN_URI "http://thunderox.com/thunderox_triceratops"
#define DISTRHO_PLUGIN_HAS_UI 1
#define DISTRHO_UI_USE_CAIRO 1
#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_IS_SYNTH 1
#define DISTRHO_PLUGIN_WANT_PROGRAMS 1
#define DISTRHO_PLUGIN_WANT_TIMEPOS 1

#define DISTRHO_UI_URI DISTRHO_PLUGIN_URI "#UI"

enum Parameters {
    

	TRICERATOPS_MASTER_VOLUME,
	TRICERATOPS_MASTER_TUNE,
	TRICERATOPS_AMP_DRIVE,
	TRICERATOPS_FILTER_MODE,
	TRICERATOPS_FILTER_FREQUENCY,
	TRICERATOPS_FILTER_RESONANCE,
	TRICERATOPS_FILTER_KEY_FOLLOW,
	TRICERATOPS_LEGATO,
	TRICERATOPS_SYNC,
	TRICERATOPS_WARMTH,
	TRICERATOPS_FM,
	TRICERATOPS_PANIC,

	TRICERATOPS_OSC1_ACTIVE,
	TRICERATOPS_OSC1_VOLUME,
	TRICERATOPS_OSC1_PULSEWIDTH,
	TRICERATOPS_OSC1_WAVE,
	TRICERATOPS_OSC1_OCTAVE,
	TRICERATOPS_OSC1_DETUNE,
	TRICERATOPS_OSC1_DETUNE_CENTRE,
	TRICERATOPS_OSC1_INERTIA,

	TRICERATOPS_OSC2_ACTIVE,
	TRICERATOPS_OSC2_VOLUME,
	TRICERATOPS_OSC2_PULSEWIDTH,
	TRICERATOPS_OSC2_WAVE,
	TRICERATOPS_OSC2_OCTAVE,
	TRICERATOPS_OSC2_DETUNE,
	TRICERATOPS_OSC2_DETUNE_CENTRE,
	TRICERATOPS_OSC2_INERTIA,

	TRICERATOPS_OSC3_ACTIVE,
	TRICERATOPS_OSC3_VOLUME,
	TRICERATOPS_OSC3_PULSEWIDTH,
	TRICERATOPS_OSC3_WAVE,
	TRICERATOPS_OSC3_OCTAVE,
	TRICERATOPS_OSC3_DETUNE,
	TRICERATOPS_OSC3_DETUNE_CENTRE,
	TRICERATOPS_OSC3_INERTIA,

	TRICERATOPS_ADSR1_ATTACK,
	TRICERATOPS_ADSR1_DECAY,
	TRICERATOPS_ADSR1_SUSTAIN,
	TRICERATOPS_ADSR1_RELEASE,
	TRICERATOPS_ADSR1_ROUTE_ONE,
	TRICERATOPS_ADSR1_ROUTE_ONE_DEST,
	TRICERATOPS_ADSR1_ROUTE_TWO,
	TRICERATOPS_ADSR1_ROUTE_TWO_DEST,

	TRICERATOPS_ADSR2_ATTACK,
	TRICERATOPS_ADSR2_DECAY,
	TRICERATOPS_ADSR2_SUSTAIN,
	TRICERATOPS_ADSR2_RELEASE,
	TRICERATOPS_ADSR2_ROUTE_ONE,
	TRICERATOPS_ADSR2_ROUTE_ONE_DEST,
	TRICERATOPS_ADSR2_ROUTE_TWO,
	TRICERATOPS_ADSR2_ROUTE_TWO_DEST,

	TRICERATOPS_ADSR3_ATTACK,
	TRICERATOPS_ADSR3_DECAY,
	TRICERATOPS_ADSR3_SUSTAIN,
	TRICERATOPS_ADSR3_RELEASE,
	TRICERATOPS_ADSR3_LFO1_AMOUNT,
	TRICERATOPS_ADSR3_LFO2_AMOUNT,
	TRICERATOPS_ADSR3_LFO3_AMOUNT,

	TRICERATOPS_LFO1_RETRIG,
	TRICERATOPS_LFO1_SPEED,
	TRICERATOPS_LFO1_WAVE,
	TRICERATOPS_LFO1_OSC1_AMOUNT,
	TRICERATOPS_LFO1_OSC2_AMOUNT,
	TRICERATOPS_LFO1_OSC3_AMOUNT,
	TRICERATOPS_LFO1_FILTER,
	TRICERATOPS_LFO1_ROUTE_ONE,
	TRICERATOPS_LFO1_ROUTE_ONE_DEST,
	TRICERATOPS_LFO1_ROUTE_TWO,
	TRICERATOPS_LFO1_ROUTE_TWO_DEST,

	TRICERATOPS_LFO2_RETRIG,
	TRICERATOPS_LFO2_SPEED,
	TRICERATOPS_LFO2_WAVE,
	TRICERATOPS_LFO2_OSC1_AMOUNT,
	TRICERATOPS_LFO2_OSC2_AMOUNT,
	TRICERATOPS_LFO2_OSC3_AMOUNT,
	TRICERATOPS_LFO2_FILTER,
	TRICERATOPS_LFO2_ROUTE_ONE,
	TRICERATOPS_LFO2_ROUTE_ONE_DEST,
	TRICERATOPS_LFO2_ROUTE_TWO,
	TRICERATOPS_LFO2_ROUTE_TWO_DEST,

	TRICERATOPS_LFO3_RETRIG,
	TRICERATOPS_LFO3_SPEED,
	TRICERATOPS_LFO3_WAVE,
	TRICERATOPS_LFO3_OSC1_AMOUNT,
	TRICERATOPS_LFO3_OSC2_AMOUNT,
	TRICERATOPS_LFO3_OSC3_AMOUNT,
	TRICERATOPS_LFO3_FILTER,
	TRICERATOPS_LFO3_ROUTE_ONE,
	TRICERATOPS_LFO3_ROUTE_ONE_DEST,
	TRICERATOPS_LFO3_ROUTE_TWO,
	TRICERATOPS_LFO3_ROUTE_TWO_DEST,

	TRICERATOPS_FX_ECHO_ACTIVE,
	TRICERATOPS_FX_ECHO_SPEED,
	TRICERATOPS_FX_ECHO_DECAY,
	TRICERATOPS_FX_ECHO_EQ_LOW,
	TRICERATOPS_FX_ECHO_EQ_MID,
	TRICERATOPS_FX_ECHO_EQ_HIGH,

	TRICERATOPS_UNISON_ACTIVATE,
	TRICERATOPS_UNISON_ONE,
	TRICERATOPS_UNISON_TWO,
	TRICERATOPS_UNISON_THREE,

	TRICERATOPS_MODIFIER_DIRT,

	TRICERATOPS_FX_REVERB_ACTIVE,
	TRICERATOPS_FX_REVERB_DECAY,
	TRICERATOPS_FX_REVERB_MIX,

	TRICERATOPS_MODIFIER_STEREO_MODE,
	TRICERATOPS_OSC1_PAN,
	TRICERATOPS_OSC2_PAN,
	TRICERATOPS_OSC3_PAN,
	TRICERATOPS_MODIFIER_RING,

	TRICERATOPS_PRESET_CATEGORY,
	TRICERATOPS_PITCH_BEND_RANGE,
	TRICERATOPS_MIDI_CHANNEL,
	
	TRICERATOPS_DSP_RUN,

	kParameterBufferSize,
	kParametertriceratopsTime,        
	kParameterCount
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED