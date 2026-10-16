

import os
from tkinter import Tk
from tkinter.filedialog import askopenfilename

Tk().withdraw() # we don't want a full GUI, so keep the root window from appearing
filename = askopenfilename() # show an "Open" dialog box and return the path to the selected file

f = open(filename)
w = open(filename+".new", "w")

ln = f.read()

#----- OSCILLATORS

ln = ln.replace("active_one", "TRICERATOPS_OSC1_ACTIVE")
ln = ln.replace("active_two", "TRICERATOPS_OSC2_ACTIVE")
ln = ln.replace("active_three", "TRICERATOPS_OSC3_ACTIVE")

ln = ln.replace("detune_centre_one", "TRICERATOPS_OSC1_DETUNE_CENTRE")
ln = ln.replace("detune_one", "TRICERATOPS_OSC1_DETUNE")
ln = ln.replace("detune_centre_two", "TRICERATOPS_OSC2_DETUNE_CENTRE")
ln = ln.replace("detune_two", "TRICERATOPS_OSC2_DETUNE")
ln = ln.replace("detune_centre_three", "TRICERATOPS_OSC3_DETUNE_CENTRE")
ln = ln.replace("detune_three", "TRICERATOPS_OSC3_DETUNE")

ln = ln.replace("wave_one", "TRICERATOPS_OSC1_WAVE")
ln = ln.replace("wave_two", "TRICERATOPS_OSC2_WAVE")
ln = ln.replace("wave_three", "TRICERATOPS_OSC3_WAVE")

ln = ln.replace("pan_one", "TRICERATOPS_OSC1_PAN")
ln = ln.replace("pan_two", "TRICERATOPS_OSC2_PAN")
ln = ln.replace("pan_three", "TRICERATOPS_OSC3_PAN")

ln = ln.replace("volume_one", "TRICERATOPS_OSC1_VOLUME")
ln = ln.replace("volume_two", "TRICERATOPS_OSC2_VOLUME")
ln = ln.replace("volume_three", "TRICERATOPS_OSC3_VOLUME")

ln = ln.replace("octave_one", "TRICERATOPS_OSC1_OCTAVE")
ln = ln.replace("octave_two", "TRICERATOPS_OSC2_OCTAVE")
ln = ln.replace("octave_three", "TRICERATOPS_OSC3_OCTAVE")

ln = ln.replace("inertia_one", "TRICERATOPS_OSC1_INERTIA")
ln = ln.replace("inertia_two", "TRICERATOPS_OSC2_INERTIA")
ln = ln.replace("inertia_three", "TRICERATOPS_OSC3_INERTIA")

ln = ln.replace("pulsewidth_one", "TRICERATOPS_OSC1_PULSEWIDTH")
ln = ln.replace("pulsewidth_two", "TRICERATOPS_OSC2_PULSEWIDTH")
ln = ln.replace("pulsewidth_three", "TRICERATOPS_OSC3_PULSEWIDTH")

ln = ln.replace("unison_activate", "TRICERATOPS_UNISON_ACTIVATE")
ln = ln.replace("unison_one", "TRICERATOPS_UNISON_ONE")
ln = ln.replace("unison_two", "TRICERATOPS_UNISON_TWO")
ln = ln.replace("unison_three", "TRICERATOPS_UNISON_THREE")

#---------- ENVELOPES

ln = ln.replace("attack_one", "TRICERATOPS_ADSR1_ATTACK")
ln = ln.replace("attack_two", "TRICERATOPS_ADSR2_ATTACK")
ln = ln.replace("attack_three", "TRICERATOPS_ADSR3_ATTACK")

ln = ln.replace("decay_one", "TRICERATOPS_ADSR1_DECAY")
ln = ln.replace("decay_two", "TRICERATOPS_ADSR2_DECAY")
ln = ln.replace("decay_three", "TRICERATOPS_ADSR3_DECAY")

ln = ln.replace("sustain_one", "TRICERATOPS_ADSR1_SUSTAIN")
ln = ln.replace("sustain_two", "TRICERATOPS_ADSR2_SUSTAIN")
ln = ln.replace("sustain_three", "TRICERATOPS_ADSR3_SUSTAIN")

ln = ln.replace("release_one", "TRICERATOPS_ADSR1_RELEASE")
ln = ln.replace("release_two", "TRICERATOPS_ADSR2_RELEASE")
ln = ln.replace("release_three", "TRICERATOPS_ADSR3_RELEASE")

ln = ln.replace("adsr1_route_one_dest", "TRICERATOPS_ADSR1_ROUTE_ONE_DEST")
ln = ln.replace("adsr1_route_two_dest", "TRICERATOPS_ADSR1_ROUTE_TWO_DEST")
ln = ln.replace("adsr1_route_three_dest", "TRICERATOPS_ADSR1_ROUTE_THREE_DEST")

ln = ln.replace("adsr1_route_one", "TRICERATOPS_ADSR1_ROUTE_ONE")
ln = ln.replace("adsr1_route_two", "TRICERATOPS_ADSR1_ROUTE_TWO")
ln = ln.replace("adsr1_route_three", "TRICERATOPS_ADSR1_ROUTE_THREE")

ln = ln.replace("adsr1_route_one", "TRICERATOPS_ADSR1_ROUTE_ONE")
ln = ln.replace("adsr2_route_one", "TRICERATOPS_ADSR2_ROUTE_ONE")
ln = ln.replace("adsr3_route_one", "TRICERATOPS_ADSR3_ROUTE_ONE")

ln = ln.replace("adsr1_route_two", "TRICERATOPS_ADSR1_ROUTE_TWO")
ln = ln.replace("adsr2_route_two", "TRICERATOPS_ADSR2_ROUTE_TWO")
ln = ln.replace("adsr3_route_two", "TRICERATOPS_ADSR3_ROUTE_TWO")

ln = ln.replace("adsr3_lfo1_amount", "TRICERATOPS_ADSR3_LFO1_AMOUNT")
ln = ln.replace("adsr3_lfo2_amount", "TRICERATOPS_ADSR3_LFO2_AMOUNT")
ln = ln.replace("adsr3_lfo3_amount", "TRICERATOPS_ADSR3_LFO3_AMOUNT")

#--------- FILTER

ln = ln.replace("cutoff", "TRICERATOPS_FILTER_FREQUENCY")
ln = ln.replace("resonance", "TRICERATOPS_FILTER_RESONANCE")
ln = ln.replace("filter_mode", "TRICERATOPS_FILTER_MODE")
ln = ln.replace("filter_key_follow", "TRICERATOPS_FILTER_KEY_FOLLOW")

# -------- LFOs

ln = ln.replace("lfo1_speed", "TRICERATOPS_LFO1_SPEED")
ln = ln.replace("lfo2_speed", "TRICERATOPS_LFO2_SPEED")
ln = ln.replace("lfo3_speed", "TRICERATOPS_LFO3_SPEED")

ln = ln.replace("lfo1_wave", "TRICERATOPS_LFO1_WAVE")
ln = ln.replace("lfo2_wave", "TRICERATOPS_LFO2_WAVE")
ln = ln.replace("lfo3_wave", "TRICERATOPS_LFO3_WAVE")

ln = ln.replace("lfo1_dco1_pitch", "TRICERATOPS_LFO1_OSC1_AMOUNT")
ln = ln.replace("lfo1_dco2_pitch", "TRICERATOPS_LFO1_OSC2_AMOUNT")
ln = ln.replace("lfo1_dco3_pitch", "TRICERATOPS_LFO1_OSC3_AMOUNT")

ln = ln.replace("lfo2_dco1_pitch", "TRICERATOPS_LFO2_OSC1_AMOUNT")
ln = ln.replace("lfo2_dco2_pitch", "TRICERATOPS_LFO2_OSC2_AMOUNT")
ln = ln.replace("lfo2_dco3_pitch", "TRICERATOPS_LFO2_OSC3_AMOUNT")

ln = ln.replace("lfo3_dco1_pitch", "TRICERATOPS_LFO3_OSC1_AMOUNT")
ln = ln.replace("lfo3_dco2_pitch", "TRICERATOPS_LFO3_OSC2_AMOUNT")
ln = ln.replace("lfo3_dco3_pitch", "TRICERATOPS_LFO3_OSC3_AMOUNT")

ln = ln.replace("lfo1_filter", "TRICERATOPS_LFO1_FILTER")
ln = ln.replace("lfo2_filter", "TRICERATOPS_LFO2_FILTER")
ln = ln.replace("lfo3_filter", "TRICERATOPS_LFO3_FILTER")

ln = ln.replace("lfo1_retrig",  "TRICERATOPS_LFO1_RETRIG")
ln = ln.replace("lfo2_retrig",  "TRICERATOPS_LFO2_RETRIG")
ln = ln.replace("lfo3_retrig",  "TRICERATOPS_LFO3_RETRIG")

ln = ln.replace("lfo1_route_one_dest",  "TRICERATOPS_LFO1_ROUTE_ONE_DEST")
ln = ln.replace("lfo1_route_one",  "TRICERATOPS_LFO1_ROUTE_ONE")

ln = ln.replace("lfo1_route_two_dest",  "TRICERATOPS_LFO1_ROUTE_TWO_DEST")
ln = ln.replace("lfo1_route_two",  "TRICERATOPS_LFO1_ROUTE_TWO")

ln = ln.replace("lfo2_route_one_dest",  "TRICERATOPS_LFO2_ROUTE_ONE_DEST")
ln = ln.replace("lfo2_route_one",  "TRICERATOPS_LFO2_ROUTE_ONE")

ln = ln.replace("lfo2_route_two_dest",  "TRICERATOPS_LFO2_ROUTE_TWO_DEST")
ln = ln.replace("lfo2_route_two",  "TRICERATOPS_LFO2_ROUTE_TWO")

ln = ln.replace("lfo3_route_one_dest",  "TRICERATOPS_LFO3_ROUTE_ONE_DEST")
ln = ln.replace("lfo3_route_one",  "TRICERATOPS_LFO3_ROUTE_ONE")

ln = ln.replace("lfo3_route_two_dest",  "TRICERATOPS_LFO3_ROUTE_TWO_DEST")
ln = ln.replace("lfo3_route_two",  "TRICERATOPS_LFO3_ROUTE_TWO")

#------- fx

ln = ln.replace("amp_drive", "TRICERATOPS_AMP_DRIVE")

ln = ln.replace("bend_range", "TRICERATOPS_PITCH_BEND_RANGE")

ln = ln.replace("fm", "TRICERATOPS_FM")

ln = ln.replace("legato", "TRICERATOPS_LEGATO")

ln = ln.replace("fx_echo_active", "TRICERATOPS_FX_ECHO_ACTIVE")
ln = ln.replace("fx_echo_speed", "TRICERATOPS_FX_ECHO_SPEED")
ln = ln.replace("fx_echo_decay", "TRICERATOPS_FX_ECHO_DECAY")
ln = ln.replace("fx_echo_eq_low", "TRICERATOPS_FX_ECHO_EQ_LOW")
ln = ln.replace("fx_echo_eq_mid", "TRICERATOPS_FX_ECHO_EQ_MID")
ln = ln.replace("fx_echo_eq_high", "TRICERATOPS_FX_ECHO_EQ_HIGH")

ln = ln.replace("fx_reverb_active", "TRICERATOPS_FX_REVERB_ACTIVE")
ln = ln.replace("fx_reverb_decay", "TRICERATOPS_FX_REVERB_DECAY")
ln = ln.replace("fx_reverb_mix", "TRICERATOPS_FX_REVERB_MIX")

ln = ln.replace("master_tune", "TRICERATOPS_MASTER_TUNE")
ln = ln.replace("master_volume", "TRICERATOPS_MASTER_VOLUME")
ln = ln.replace("midi_channel", "TRICERATOPS_MIDI_CHANNEL")
ln = ln.replace("modifier_dirt", "TRICERATOPS_MODIFIER_DIRT")
ln = ln.replace("modifier_ring", "TRICERATOPS_MODIFIER_RING")
ln = ln.replace("stereo_mode", "TRICERATOPS_MODIFIER_STEREO_MODE")
ln = ln.replace("warmth", "TRICERATOPS_WARMTH")
ln = ln.replace("sync", "TRICERATOPS_SYNC")


w.write(ln)

print (ln)


f.close()
w.close()

os.remove(filename) 
os.rename ( filename + ".new", filename)


