

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "Window.hpp"
#include <iostream>
#include <sstream> 
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

using namespace std;

#include "../../delirium_ui/delirium_ui.hpp"

START_NAMESPACE_DISTRHO

class triceratopsUI : public UI
{
	public:

		//------------------------------------------------------------------------------------------------------

		triceratopsUI() : UI(960, 460)
		{

			memset(fParameters, 0, sizeof(float)*kParameterCount);
			memset(fParameters_widget_number, -1, sizeof(int)*kParameterCount);

			GUI = Delirium_UI_Init(static_cast<float>(getWidth()),static_cast<float>(getHeight()), 40,30);
			
			// OSCILATOR PANEL
			
			float panelX = 0.5;
			float panelY = 0.5;

			GUI->group_visible[0] = true;
			
			//------------------------------------------------------------------------------------------------------------------------------------------
			
			// OSCILATOR ONE ------------------------------------------------------------------------------------------
			
			int osc_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19,6.5, "OSCILLATORS", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, osc_panel, "global", "");
			
			int widget_logo = Delirium_UI_Create_Widget(GUI, deliriumUI_Logo, 0, panelX + 13.5, panelY + 1.25, 3.5, 6, "", -1);
			GUI->Widgets[widget_logo]->logo_image = cairo_image_surface_create_from_png ("/usr/lib/lv2/thunderox_triceratops.lv2/logo.png");
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_logo, "global", "");
						
			// OSCILATOR NAVIGATION ONE TO THREE -----------------------------------------------------------------------
			
			int oscillators_group = Delirium_UI_Group_Create(GUI, "oscillators");
			Delirium_UI_Group_Add_Member(GUI, "oscillators", "osc1");
			Delirium_UI_Group_Add_Member(GUI, "oscillators", "osc2");
			Delirium_UI_Group_Add_Member(GUI, "oscillators", "osc3");
			Delirium_UI_Group_Add_Member(GUI, "oscillators", "unison");
			
			int nav_oscillators = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator,
				0, panelX + 0.5, panelY + 0.25, 19,0.6,"",-1);			
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_oscillators, "OSC-1", "oscillators", "osc1");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_oscillators, "OSC-2", "oscillators", "osc2");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_oscillators, "OSC-3", "oscillators", "osc3");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_oscillators, "UNISON", "oscillators", "unison");
			Delirium_UI_Group_Set_Visible_member(GUI, "oscillators", "osc1");
			
			Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_oscillators, "global", "");
			
			
			// OSCILATOR ONE ------------------------------------------------------------------------------------------
			
			//------- Oscilator One - Active

			int widget_osc1_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "OSC-1", TRICERATOPS_OSC1_ACTIVE);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_active, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_active, "oscillators", "osc1");			
			fParameters_widget_number[TRICERATOPS_OSC1_ACTIVE] = widget_osc1_active; 
			
			//------- Oscilator One - Volume
	
			int widget_osc1_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 0.5, panelY + 4, 2, 2.5, "VOL", TRICERATOPS_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_volume, 0.75);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc1_volume, 0.75);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_volume, "oscillators", "osc1");			
			fParameters_widget_number[TRICERATOPS_OSC1_VOLUME] = widget_osc1_volume; 
			
			//------- Oscilator One - Pan

			int widget_osc1_pan = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 3, panelY + 4, 2, 2.5, "PAN", TRICERATOPS_OSC1_PAN);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_pan, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_pan, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc1_pan, 0.5);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_pan, "oscillators", "osc1");
			fParameters_widget_number[TRICERATOPS_OSC1_PAN] = widget_osc1_pan; 
			
			//------- Oscilator One - Detune

			int widget_osc1_detune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 6, panelY + 1.25, 1, 5.25, "DETUNE", TRICERATOPS_OSC1_DETUNE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_detune, 7,-7);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_detune, 0);
			fParameters_widget_number[TRICERATOPS_OSC1_DETUNE] = widget_osc1_detune; 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_detune, "oscillators", "osc1");
			fParameters_widget_number[TRICERATOPS_OSC1_DETUNE] = widget_osc1_detune; 
			
			//------- Oscilator One - Wave
			
			int widget_osc1_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX + 8, panelY + 1.25, 2, 2.5,"WAVE", TRICERATOPS_OSC1_WAVE);
			Delirium_UI_Widget_Set_Selector_Type(GUI, widget_osc1_wave, WAVE_MODE_OSC);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_wave, "oscillators", "osc1");
			fParameters_widget_number[TRICERATOPS_OSC1_WAVE] = widget_osc1_wave; 

			//------- Oscilator One - Inertia

			int widget_osc1_glide = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 8, panelY + 4, 2, 2.5, "GLIDE", TRICERATOPS_OSC1_INERTIA);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_glide, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_glide, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_glide, "oscillators", "osc1");
			fParameters_widget_number[TRICERATOPS_OSC1_INERTIA] = widget_osc1_glide; 

			//------- Oscilator One - Octave

			int widget_osc1_octave = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 10.5, panelY + 1.25, 2, 2.5, "OCTAVE", TRICERATOPS_OSC1_OCTAVE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_octave, -5,5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_octave, -2);
			Delirium_UI_Widget_Set_Increment(GUI, widget_osc1_octave, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_osc1_octave, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_octave, "oscillators", "osc1");
			fParameters_widget_number[TRICERATOPS_OSC1_OCTAVE] = widget_osc1_octave; 

			//------- Oscilator One - Pulsewidth

			int widget_osc1_pulsewidth = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 10.5, panelY + 4, 2, 2.5, "PW", TRICERATOPS_OSC1_PULSEWIDTH);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_pulsewidth, -0.5,0.5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_pulsewidth, 0);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc1_pulsewidth, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_pulsewidth, "oscillators", "osc1");
			fParameters_widget_number[TRICERATOPS_OSC1_PULSEWIDTH] = widget_osc1_pulsewidth; 
			
			
			
			// OSCILATOR TWO ------------------------------------------------------------------------------------------

			//------- Oscilator Two - Active

			int widget_osc2_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "OSC-2", TRICERATOPS_OSC2_ACTIVE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_active, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_active, "oscillators", "osc2");			
			fParameters_widget_number[TRICERATOPS_OSC2_ACTIVE] = widget_osc2_active; 
			
			//------- Oscilator Two - Volume
	
			int widget_osc2_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 0.5, panelY + 4, 2, 2.5, "VOL", TRICERATOPS_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_volume, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc2_volume, 0.5);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_volume, "oscillators", "osc2");
			fParameters_widget_number[TRICERATOPS_OSC2_VOLUME] = widget_osc2_volume; 

			//------- Oscilator Two - Pan

			int widget_osc2_pan = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 3, panelY + 4, 2, 2.5, "PAN", TRICERATOPS_OSC2_PAN);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_pan, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_pan, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc2_pan, 0.5);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_pan, "oscillators", "osc2");
			fParameters_widget_number[TRICERATOPS_OSC2_PAN] = widget_osc2_pan; 

			//------- Oscilator Two - Detune
			
			int widget_osc2_detune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 6, panelY + 1.25, 1, 5.25, "DETUNE", TRICERATOPS_OSC2_DETUNE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_detune, 7,-7);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_detune, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_detune, "oscillators", "osc2");
			fParameters_widget_number[TRICERATOPS_OSC2_DETUNE] = widget_osc2_detune; 

			//------- Oscilator Two - Wave			
			
			int widget_osc2_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX + 8, panelY + 1.25, 2, 2.5,"WAVE", TRICERATOPS_OSC2_WAVE);
			Delirium_UI_Widget_Set_Selector_Type(GUI, widget_osc2_wave, WAVE_MODE_OSC);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_wave, "oscillators", "osc2");
			fParameters_widget_number[TRICERATOPS_OSC2_WAVE] = widget_osc2_wave; 
			
			//------- Oscilator Two - Inertia

			int widget_osc2_glide = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 8, panelY + 4, 2, 2.5, "GLIDE", TRICERATOPS_OSC2_INERTIA);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_glide, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_glide, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_glide, "oscillators", "osc2");
			fParameters_widget_number[TRICERATOPS_OSC2_INERTIA] = widget_osc2_glide; 

			//------- Oscilator Two Octave

			int widget_osc2_octave = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 10.5, panelY + 1.25, 2, 2.5, "OCTAVE", TRICERATOPS_OSC2_OCTAVE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_octave, -5,5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_octave, -2);
			Delirium_UI_Widget_Set_Increment(GUI, widget_osc2_octave, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_osc2_octave, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_octave, "oscillators", "osc2");
			fParameters_widget_number[TRICERATOPS_OSC2_OCTAVE] = widget_osc2_octave; 

			//------- Oscilator Two - Pulsewidth

			int widget_osc2_pulsewidth = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 10.5, panelY + 4, 2, 2.5, "PW", TRICERATOPS_OSC2_PULSEWIDTH);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_pulsewidth, -0.5,0.5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_pulsewidth, 0);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc2_pulsewidth, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_pulsewidth, "oscillators", "osc2");
			fParameters_widget_number[TRICERATOPS_OSC2_PULSEWIDTH] = widget_osc2_pulsewidth;
			

			// OSCILATOR THREE ------------------------------------------------------------------------------------------
			
			//------- Oscilator Three - Active

			int widget_osc3_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "OSC-3", TRICERATOPS_OSC3_ACTIVE);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_active, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_active, "oscillators", "osc3");
			fParameters_widget_number[TRICERATOPS_OSC3_ACTIVE] = widget_osc3_active; 
						
			//------- Oscilator Three - Volume
	
			int widget_osc3_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 0.5, panelY + 4, 2, 2.5, "VOL", TRICERATOPS_OSC3_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_volume, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc3_volume, 0.5);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_volume, "oscillators", "osc3");
			fParameters_widget_number[TRICERATOPS_OSC3_VOLUME] = widget_osc3_volume; 
			
			//------- Oscilator Three - Pan

			int widget_osc3_pan = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 3, panelY + 4, 2, 2.5, "PAN", TRICERATOPS_OSC3_PAN);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_pan, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_pan, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc3_pan, 0.5);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_pan, "oscillators", "osc3");
			fParameters_widget_number[TRICERATOPS_OSC3_PAN] = widget_osc3_pan; 
			
			//------- Oscilator Three - Detune

			int widget_osc3_detune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 6, panelY + 1.25, 1, 5.25, "DETUNE", TRICERATOPS_OSC3_DETUNE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_detune, 7,-7);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_detune, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_detune, "oscillators", "osc3");
			fParameters_widget_number[TRICERATOPS_OSC3_DETUNE] = widget_osc3_detune; 
			
			
			//------- Oscilator Three - Wave
			
			int widget_osc3_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX + 8, panelY + 1.25, 2, 2.5,"WAVE", TRICERATOPS_OSC3_WAVE);
			Delirium_UI_Widget_Set_Selector_Type(GUI, widget_osc3_wave, WAVE_MODE_OSC);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_wave, "oscillators", "osc3");
			fParameters_widget_number[TRICERATOPS_OSC3_WAVE] = widget_osc3_wave; 
			
						
			//------- Oscilator Three - Inertia

			int widget_osc3_glide = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 8, panelY + 4, 2, 2.5, "GLIDE", TRICERATOPS_OSC3_INERTIA);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_glide, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_glide, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_glide, "oscillators", "osc3");
			fParameters_widget_number[TRICERATOPS_OSC3_INERTIA] = widget_osc3_glide; 


			//------- Oscilator Two Octave
			
			int widget_osc3_octave = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 10.5, panelY + 1.25, 2, 2.5, "OCTAVE", TRICERATOPS_OSC3_OCTAVE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_octave, -5,5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_octave, 0);
			Delirium_UI_Widget_Set_Increment(GUI, widget_osc3_octave, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_osc3_octave, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_octave, "oscillators", "osc3");
			fParameters_widget_number[TRICERATOPS_OSC3_OCTAVE] = widget_osc3_octave; 
			
			
			//------- Oscilator Two - Pulsewidth

			int widget_osc3_pulsewidth = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 10.5, panelY + 4, 2, 2.5, "PW", TRICERATOPS_OSC3_PULSEWIDTH);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc3_pulsewidth, -0.5,0.5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc3_pulsewidth, 0);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc3_pulsewidth, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc3_pulsewidth, "oscillators", "osc3");
			fParameters_widget_number[TRICERATOPS_OSC3_PULSEWIDTH] = widget_osc3_pulsewidth;


			// UNISON ------------------------------------------------------------------------------------------
			
			//------- ACTIVE

			int widget_unison_activate = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "UNISON", TRICERATOPS_UNISON_ACTIVATE);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_unison_activate, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_unison_activate, "oscillators", "unison");
			fParameters_widget_number[TRICERATOPS_UNISON_ACTIVATE] = widget_unison_activate;
			
			int widget_unison_one = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 3, panelY + 1.25, 1, 5.25, "OSC-4", TRICERATOPS_UNISON_ONE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_unison_one, 4,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_unison_one, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_unison_one, "oscillators", "unison");
			fParameters_widget_number[TRICERATOPS_UNISON_ONE] = widget_unison_one;
			
			int widget_unison_two = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5.5, panelY + 1.25, 1, 5.25, "OSC-5", TRICERATOPS_UNISON_TWO);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_unison_two, 4,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_unison_two, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_unison_two, "oscillators", "unison");
			fParameters_widget_number[TRICERATOPS_UNISON_TWO] = widget_unison_two;
			
			int widget_unison_three = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 8, panelY + 1.25, 1, 5.25, "OSC-6", TRICERATOPS_UNISON_THREE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_unison_three, 4,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_unison_three, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_unison_three, "oscillators", "unison");
			fParameters_widget_number[TRICERATOPS_UNISON_THREE] = widget_unison_three;

			//------------------------------------------------------------------------------------------------------------------------------------------
			//------------------------------------------------------------------------------------------------------------------------------------------

			// LFO PANEL
						
			panelX = 20;
			panelY = 0.5;
			
			int lfo_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19.5,6.5, "LFO", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, lfo_panel, "global", "");

			int lfos_group = Delirium_UI_Group_Create(GUI, "lfos");
			Delirium_UI_Group_Add_Member(GUI, "lfos", "lfo1");
			Delirium_UI_Group_Add_Member(GUI, "lfos", "lfo2");
			Delirium_UI_Group_Add_Member(GUI, "lfos", "lfo3");
			
			int nav_lfos = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator, 0, panelX + 0.5, panelY + 0.25, 19,0.6,"",-1);
			
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_lfos, "LFO-1", "lfos", "lfo1");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_lfos, "LFO-2", "lfos", "lfo2");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_lfos, "LFO-3", "lfos", "lfo3");
			Delirium_UI_Group_Set_Visible_member(GUI, "lfos", "lfo1");
			
			Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_lfos, "global", "");	
			
			// LFO ONE ------------------------------------------------------------------------------------------

			int widget_lfo1_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "RETRIG", TRICERATOPS_LFO1_RETRIG);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_retrig, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_retrig, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_RETRIG] = widget_lfo1_retrig; 

			int widget_lfo1_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 0.5, panelY + 4, 2, 2.5, "BPM", TRICERATOPS_LFO1_SPEED);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_speed, 1,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo1_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo1_speed, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_speed, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_SPEED] = widget_lfo1_speed; 
			
			int widget_lfo1_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX + 2.5, panelY + 1.25, 2, 2.5,"WAVE", TRICERATOPS_LFO1_WAVE);
			Delirium_UI_Widget_Set_Selector_Type(GUI, widget_lfo1_wave, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_wave, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_WAVE] = widget_lfo1_wave;

			int widget_lfo1_osc1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5, panelY + 1.25, 1, 5.25, "OSC-1", TRICERATOPS_LFO1_OSC1_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc1, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_osc1, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_OSC1_AMOUNT] = widget_lfo1_osc1;

			int widget_lfo1_osc2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 7, panelY + 1.25, 1, 5.25, "OSC-2", TRICERATOPS_LFO1_OSC2_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc2, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_osc2, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_OSC2_AMOUNT] = widget_lfo1_osc2;

			int widget_lfo1_osc3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 9, panelY + 1.25, 1, 5.25, "OSC-3", TRICERATOPS_LFO1_OSC3_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc3, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_osc3, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_OSC3_AMOUNT] = widget_lfo1_osc3;

			int widget_lfo1_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 11, panelY + 1.25, 1, 5.25, "FILTER", TRICERATOPS_LFO1_FILTER);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route1, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_route1, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_FILTER] = widget_lfo1_route1;

			int widget_lfo1_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader_Route, 0, panelX + 13.5, panelY + 1.25, 1, 5.25, "OUT-1", TRICERATOPS_LFO1_ROUTE_ONE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route2, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_route2, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_ROUTE_ONE] = widget_lfo1_route2;
			fParameters_widget_number[TRICERATOPS_LFO1_ROUTE_ONE_DEST] = widget_lfo1_route2;
			
			int widget_lfo1_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader_Route, 0, panelX + 15.5, panelY + 1.25, 1, 5.25, "OUT-2", TRICERATOPS_LFO1_ROUTE_TWO);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route3, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_route3, "lfos", "lfo1");
			fParameters_widget_number[TRICERATOPS_LFO1_ROUTE_TWO] = widget_lfo1_route3;
			fParameters_widget_number[TRICERATOPS_LFO1_ROUTE_TWO_DEST] = widget_lfo1_route3;			

			// LFO TWO ------------------------------------------------------------------------------------------

			int widget_lfo2_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "RETRIG", TRICERATOPS_LFO2_RETRIG);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_retrig, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_retrig, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_RETRIG] = widget_lfo2_retrig; 

			int widget_lfo2_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 0.5, panelY + 4, 2, 2.5, "BPM", TRICERATOPS_LFO2_SPEED);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_speed, 0,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo2_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo2_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo2_speed, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_speed, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_SPEED] = widget_lfo2_speed; 
			
			int widget_lfo2_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX + 2.5, panelY + 1.25, 2, 2.5,"WAVE", TRICERATOPS_LFO2_WAVE);
			Delirium_UI_Widget_Set_Selector_Type(GUI, widget_lfo2_wave, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_wave, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_WAVE] = widget_lfo2_wave;

			int widget_lfo2_osc1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5, panelY + 1.25, 1, 5.25, "OSC-1", TRICERATOPS_LFO2_OSC1_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_osc1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_osc1, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_osc1, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_OSC1_AMOUNT] = widget_lfo2_osc1;

			int widget_lfo2_osc2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 7, panelY + 1.25, 1, 5.25, "OSC-2", TRICERATOPS_LFO2_OSC2_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_osc2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_osc2, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_osc2, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_OSC2_AMOUNT] = widget_lfo2_osc2;

			int widget_lfo2_osc3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 9, panelY + 1.25, 1, 5.25, "OSC-3", TRICERATOPS_LFO2_OSC3_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_osc3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_osc3, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_osc3, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_OSC3_AMOUNT] = widget_lfo2_osc3;

			int widget_lfo2_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 11, panelY + 1.25, 1, 5.25, "FILTER", TRICERATOPS_LFO2_FILTER);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_route1, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_route1, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_FILTER] = widget_lfo2_route1;

			int widget_lfo2_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader_Route, 0, panelX + 13.5, panelY + 1.25, 1, 5.25, "OUT-1", TRICERATOPS_LFO2_ROUTE_ONE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_route2, 0);
			fParameters_widget_number[TRICERATOPS_LFO2_ROUTE_ONE] = widget_lfo2_route2;
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_route2, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_ROUTE_ONE_DEST] = widget_lfo2_route2;


			int widget_lfo2_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader_Route, 0, panelX + 15.5, panelY + 1.25, 1, 5.25, "OUT-2", TRICERATOPS_LFO2_ROUTE_TWO);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_route3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_route3, 0);
			fParameters_widget_number[TRICERATOPS_LFO2_ROUTE_TWO] = widget_lfo2_route3;
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_route3, "lfos", "lfo2");
			fParameters_widget_number[TRICERATOPS_LFO2_ROUTE_TWO_DEST] = widget_lfo2_route3;
			
			
			// LFO THREE ------------------------------------------------------------------------------------------

			int widget_lfo3_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "RETRIG", TRICERATOPS_LFO3_RETRIG);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_retrig, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_retrig, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_RETRIG] = widget_lfo3_retrig; 

			int widget_lfo3_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 0.5, panelY + 4, 2, 2.5, "BPM", TRICERATOPS_LFO3_SPEED);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_speed, 0,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo3_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo3_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo3_speed, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_speed, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_SPEED] = widget_lfo3_speed; 
			
			int widget_lfo3_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX + 2.5, panelY + 1.25, 2, 2.5,"WAVE", TRICERATOPS_LFO3_WAVE);
			Delirium_UI_Widget_Set_Selector_Type(GUI, widget_lfo3_wave, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_wave, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_WAVE] = widget_lfo3_wave;
			
			int widget_lfo3_osc1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5, panelY + 1.25, 1, 5.25, "OSC-1", TRICERATOPS_LFO3_OSC1_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_osc1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_osc1, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_osc1, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_OSC1_AMOUNT] = widget_lfo3_osc1;

			int widget_lfo3_osc2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 7, panelY + 1.25, 1, 5.25, "OSC-2", TRICERATOPS_LFO3_OSC2_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_osc2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_osc2, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_osc2, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_OSC2_AMOUNT] = widget_lfo3_osc2;

			int widget_lfo3_osc3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 9, panelY + 1.25, 1, 5.25, "OSC-3", TRICERATOPS_LFO3_OSC3_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_osc3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_osc3, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_osc3, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_OSC3_AMOUNT] = widget_lfo3_osc3;

			int widget_lfo3_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 11, panelY + 1.25, 1, 5.25, "FILTER", TRICERATOPS_LFO3_FILTER);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_route1, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_route1, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_FILTER] = widget_lfo3_route1;

			int widget_lfo3_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader_Route, 0, panelX + 13.5, panelY + 1.25, 1, 5.25, "OUT-1", TRICERATOPS_LFO3_ROUTE_ONE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_route2, 0);
			fParameters_widget_number[TRICERATOPS_LFO3_ROUTE_ONE] = widget_lfo3_route2;
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_route2, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_ROUTE_ONE_DEST] = widget_lfo3_route2;

			int widget_lfo3_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader_Route, 0, panelX + 15.5, panelY + 1.25, 1, 5.25, "OUT-2", TRICERATOPS_LFO3_ROUTE_TWO);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo3_route3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo3_route3, 0);
			fParameters_widget_number[TRICERATOPS_LFO3_ROUTE_TWO] = widget_lfo3_route3;
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo3_route3, "lfos", "lfo3");
			fParameters_widget_number[TRICERATOPS_LFO3_ROUTE_TWO_DEST] = widget_lfo3_route3;			

			//--------------------------------------------------------------------------------------------
			// ENVELOPE PANEL
			
			panelX = 0.5;
			panelY = 7.75;
			
			int envelopes_group = Delirium_UI_Group_Create(GUI, "envelopes");
			Delirium_UI_Group_Add_Member(GUI, "envelopes", "adsr");
			
			int nav_envelopes = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator, 0, panelX + 0.25, panelY + 0.6, 19,0.75,"",-1);			
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_envelopes, "ENVELOPES", "envelopes", "adsr");
			Delirium_UI_Group_Set_Visible_member(GUI, "envelopes", "adsr");
			
			// Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_envelopes, "global", "");
			
			// --------------
			
			int envelope_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19, 6.25, "ENVELOPES", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, envelope_panel, "global", "");

			int widget_amp_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 0.25, panelY + 0.75, 5, 2.5,"AMP",TRICERATOPS_ADSR1_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_amp_env, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR1_ATTACK] = widget_amp_env; 
			fParameters_widget_number[TRICERATOPS_ADSR1_ATTACK+1] = widget_amp_env; 
			fParameters_widget_number[TRICERATOPS_ADSR1_ATTACK+2] = widget_amp_env; 
			fParameters_widget_number[TRICERATOPS_ADSR1_ATTACK+3] = widget_amp_env; 
		
						
			Delirium_UI_Widget_ADSR* wdg_amp_env = (Delirium_UI_Widget_ADSR*)GUI->Widgets[widget_amp_env];
			wdg_amp_env->default_values[0] = 1;
			wdg_amp_env->values[0] = 1;

			int widget_filter_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 0.25, panelY + 3.75, 5, 2.5,"FILTER",TRICERATOPS_ADSR2_ATTACK);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_env, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR2_ATTACK] = widget_filter_env;
			fParameters_widget_number[TRICERATOPS_ADSR2_ATTACK+1] = widget_filter_env; 
			fParameters_widget_number[TRICERATOPS_ADSR2_ATTACK+2] = widget_filter_env; 
			fParameters_widget_number[TRICERATOPS_ADSR2_ATTACK+3] = widget_filter_env; 
			
			int widget_mod_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 13.75, panelY + 0.75, 5, 2.5,"MOD",TRICERATOPS_ADSR3_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_mod_env, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR3_ATTACK] = widget_mod_env;
			fParameters_widget_number[TRICERATOPS_ADSR3_ATTACK+1] = widget_mod_env; 
			fParameters_widget_number[TRICERATOPS_ADSR3_ATTACK+2] = widget_mod_env; 
			fParameters_widget_number[TRICERATOPS_ADSR3_ATTACK+3] = widget_mod_env; 


			
			int widget_env2_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 6, panelY + 1.25, 1, 5.25, "FILTER", TRICERATOPS_ADSR2_ROUTE_ONE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env2_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env2_route1, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_env2_route1, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR2_ROUTE_ONE] = widget_env2_route1; 

			int widget_env1_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader_Route, 0, panelX + 8, panelY + 1.25, 1, 5.25, "OSC-1", TRICERATOPS_ADSR1_ROUTE_TWO);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env1_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env1_route2, 0);
			Delirium_UI_Widget_Set_Route_Destination(GUI, widget_env1_route2, 3);
			Delirium_UI_Widget_Set_Integer(GUI, widget_env1_route2, false);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_env1_route2, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR1_ROUTE_TWO] = widget_env1_route2; 
			fParameters_widget_number[TRICERATOPS_ADSR1_ROUTE_TWO_DEST] = widget_env1_route2; 
						
			int widget_env2_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader_Route, 0, panelX + 10, panelY + 1.25, 1, 5.25, "OSC-2", TRICERATOPS_ADSR2_ROUTE_TWO);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env2_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env2_route2, 0);
			Delirium_UI_Widget_Set_Route_Destination(GUI, widget_env2_route2, 4);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_env2_route2, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR2_ROUTE_TWO] = widget_env2_route2; 
			fParameters_widget_number[TRICERATOPS_ADSR2_ROUTE_TWO_DEST] = widget_env2_route2; 
			
			int widget_env3_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 12, panelY + 4, 2, 2, "LFO-1", TRICERATOPS_ADSR3_LFO1_AMOUNT);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_env3_route1, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR3_LFO1_AMOUNT] = widget_env3_route1; 

			int widget_env3_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 14.5, panelY + 4, 2, 2, "LFO-2", TRICERATOPS_ADSR3_LFO2_AMOUNT);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_env3_route2, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR3_LFO2_AMOUNT] = widget_env3_route2; 
			
			int widget_env3_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 17, panelY + 4, 2, 2, "LFO-3", TRICERATOPS_ADSR3_LFO2_AMOUNT);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_env3_route3, "envelopes", "adsr");
			fParameters_widget_number[TRICERATOPS_ADSR3_LFO3_AMOUNT] = widget_env3_route3; 


			//--------------------------------------------------------------------------------------------
			// GENERAL PANEL - INCLUDING VOLUME
			
			
			panelX = 20;
			panelY = 7.75;
			
			int general_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 19.5,6.25, "AMP, FILTER & FX", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, general_panel, "global", "");

			int general_group = Delirium_UI_Group_Create(GUI, "general");
			Delirium_UI_Group_Add_Member(GUI, "general", "vol_flt");
			Delirium_UI_Group_Add_Member(GUI, "general", "fx");
			Delirium_UI_Group_Add_Member(GUI, "general", "mod");
			Delirium_UI_Group_Add_Member(GUI, "general", "presets");
			
			int nav_general = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator, 0, panelX + 0.5, panelY + 0.25, 19,0.6,"",-1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_general, "global", "");
			
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_general, "VOL / FLT", "general", "vol_flt");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_general, "FX", "general", "fx");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_general, "MOD", "general", "mod");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_general, "PRESETS", "general", "presets");
			Delirium_UI_Group_Set_Visible_member(GUI, "general", "VOL/FLT+");
			
			// FILTER CUTOFF FREQUENCY
			
			int widget_filter_cutoff = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 4, panelY + 1.25, 1, 5.25, "CUTOFF",TRICERATOPS_FILTER_FREQUENCY);
				
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_cutoff, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_cutoff, "general", "vol_flt");
			fParameters_widget_number[TRICERATOPS_FILTER_FREQUENCY] = widget_filter_cutoff;
			
			// FILTER RESONANCE AMOUNT
			
			int widget_filter_resonance = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 6, panelY + 1.25, 1, 5.25, "RES", TRICERATOPS_FILTER_RESONANCE);
				
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_resonance, 1,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_resonance, "general", "vol_flt");
			fParameters_widget_number[TRICERATOPS_FILTER_RESONANCE] = widget_filter_resonance;
			
			// FILTER TYPE SELECTOR
			
			int widget_filter_type = Delirium_UI_Create_Widget(GUI, deliriumUI_Filter, 0, panelX + 0.5, panelY + 1.6, 2.5, 4.5, "FILTER", TRICERATOPS_FILTER_MODE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_type, "general", "vol_flt");
			fParameters_widget_number[TRICERATOPS_FILTER_MODE] = widget_filter_type;	
			
			// FILTER FOLLOW KEYBOARD
			
			int widget_filter_key_follow = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 11.5, panelY + 1.25, 1, 5.25, "KEYF", TRICERATOPS_FILTER_KEY_FOLLOW);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_key_follow, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_key_follow, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_key_follow, "general", "vol_flt");
			fParameters_widget_number[TRICERATOPS_FILTER_KEY_FOLLOW] = widget_filter_key_follow;
			
			int widget_master_tune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 13.5, panelY + 1.25, 1, 5.25, "TUNE", TRICERATOPS_MASTER_TUNE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_master_tune, -7,7);
			Delirium_UI_Widget_Set_Value(GUI, widget_master_tune, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_master_tune, "general", "vol_flt");
			fParameters_widget_number[TRICERATOPS_MASTER_TUNE] = widget_master_tune;

			int widget_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 15, panelY + 1.75, 4, 4, "VOLUME", TRICERATOPS_MASTER_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_volume, 0,2);
			Delirium_UI_Widget_Set_Value(GUI, widget_volume, 0.6);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_volume, 0.6);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_volume, "global", "");
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_volume, "general", "vol_flt");
			fParameters_widget_number[TRICERATOPS_MASTER_VOLUME] = widget_volume; 
			
			//------- FX PANEL
			
			// ECHO
			
			int widget_echo_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "ECHO", TRICERATOPS_FX_ECHO_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_echo_active, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_ECHO_ACTIVE] = widget_echo_active; 

			int widget_echo_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 0.5, panelY + 4, 2, 2.5, "SPEED", TRICERATOPS_FX_ECHO_SPEED);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_echo_speed, 512,65536);
			Delirium_UI_Widget_Set_Value(GUI, widget_echo_speed, 4096);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_echo_speed, 4096);
			Delirium_UI_Widget_Set_Increment(GUI, widget_echo_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_echo_speed, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_echo_speed, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_ECHO_SPEED] = widget_echo_speed; 
			
			int widget_echo_decay = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 3, panelY + 4, 2, 2.5, "DECAY", TRICERATOPS_FX_ECHO_DECAY);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_echo_decay, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_echo_decay, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_echo_decay, 0.5);
			Delirium_UI_Widget_Set_Increment(GUI, widget_echo_decay, 0.1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_echo_decay, false);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_echo_decay, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_ECHO_DECAY] = widget_echo_decay; 
			
			int widget_fx_echo_eq_low = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5.5, panelY + 1.25, 1, 5.25, "EQ-LOW", TRICERATOPS_FX_ECHO_EQ_LOW);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_fx_echo_eq_low, 2,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_fx_echo_eq_low, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_fx_echo_eq_low, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_ECHO_EQ_LOW] = widget_fx_echo_eq_low;

			int widget_fx_echo_eq_mid = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 7.75, panelY + 1.25, 1, 5.25, "EQ-MID", TRICERATOPS_FX_ECHO_EQ_MID);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_fx_echo_eq_mid, 2,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_fx_echo_eq_mid, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_fx_echo_eq_mid, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_ECHO_EQ_MID] = widget_fx_echo_eq_mid;

			int widget_fx_echo_eq_high = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 10, panelY + 1.25, 1, 5.25, "EQ-HI", TRICERATOPS_FX_ECHO_EQ_HIGH);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_fx_echo_eq_high, 2,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_fx_echo_eq_high, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_fx_echo_eq_high, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_ECHO_EQ_HIGH] = widget_fx_echo_eq_high;
			
			// REVERB
	
			int widget_fx_reverb_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 13, panelY + 1.25, 2, 2, "REVERB", TRICERATOPS_FX_REVERB_ACTIVE);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_fx_reverb_active, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_REVERB_ACTIVE] = widget_fx_reverb_active;	
			
			int widget_fx_reverb_decay = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 15.5, panelY + 1.25, 1, 5.25, "DECAY", TRICERATOPS_FX_REVERB_DECAY);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_fx_reverb_decay, 0,15);
			Delirium_UI_Widget_Set_Value(GUI, widget_fx_reverb_decay, 4);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_fx_reverb_decay, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_REVERB_DECAY] = widget_fx_reverb_decay;			
			
			int widget_fx_reverb_mix = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 17.5, panelY + 1.25, 1, 5.25, "MIX", TRICERATOPS_FX_REVERB_MIX);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_fx_reverb_mix, 0,0.8);
			Delirium_UI_Widget_Set_Value(GUI, widget_fx_reverb_mix, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_fx_reverb_mix, "general", "fx");
			fParameters_widget_number[TRICERATOPS_FX_REVERB_MIX] = widget_fx_reverb_mix;	

			

			//------- MOD PANEL

			panelX = 20;
			panelY = 7.75;
			
			// MIDI CHANNEL
			
			int widget_midi_channel = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 4, panelY + 1.25, 1, 5.25, "MIDI-CH",TRICERATOPS_MIDI_CHANNEL);				
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_midi_channel, 1,14);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_midi_channel, "general", "mod");
			Delirium_UI_Widget_Set_Integer(GUI, widget_midi_channel, true);
			Delirium_UI_Widget_Set_Value(GUI, widget_midi_channel,1);
			fParameters_widget_number[TRICERATOPS_MIDI_CHANNEL] = widget_midi_channel;
						
			int widget_dirt = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 6, panelY + 1.25, 1, 5.25, "DIRT",TRICERATOPS_MODIFIER_DIRT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_dirt, 1,0);
			Delirium_UI_Widget_Set_Integer(GUI, widget_dirt, false);
			Delirium_UI_Widget_Set_Value(GUI, widget_dirt,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_dirt, "general", "mod");
			fParameters_widget_number[TRICERATOPS_MODIFIER_DIRT] = widget_dirt;
			
			int widget_warmth = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 8, panelY + 1.25, 1, 5.25, "WARM",TRICERATOPS_WARMTH);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_warmth, 1,0);
			Delirium_UI_Widget_Set_Integer(GUI, widget_warmth, false);
			Delirium_UI_Widget_Set_Value(GUI, widget_warmth,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_warmth, "general", "mod");
			fParameters_widget_number[TRICERATOPS_WARMTH] = widget_warmth;
			
			

			// LEGATO, SYNC, FM, RING
			
			int widget_mod_legato = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2, "LEGATO", TRICERATOPS_LEGATO);
			Delirium_UI_Widget_Set_Value(GUI, widget_mod_legato,false);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_mod_legato, "general", "mod");
			fParameters_widget_number[TRICERATOPS_LEGATO] = widget_mod_legato;
			
			
			int widget_mod_stereo = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 4, 2, 2, "STEREO", TRICERATOPS_MODIFIER_STEREO_MODE);
			Delirium_UI_Widget_Set_Value(GUI, widget_mod_stereo,true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_mod_stereo, "general", "mod");
			fParameters_widget_number[TRICERATOPS_MODIFIER_STEREO_MODE] = widget_mod_stereo;
			
			int widget_mod_sync = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 17, panelY + 1.25, 2, 2, "SYNC", TRICERATOPS_SYNC);
			Delirium_UI_Widget_Set_Value(GUI, widget_mod_sync,false);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_mod_sync, "general", "mod");	
			fParameters_widget_number[TRICERATOPS_SYNC] = widget_mod_sync;
			
			int widget_mod_fm = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 17, panelY + 4, 2, 2, "FM", TRICERATOPS_FM);
			Delirium_UI_Widget_Set_Value(GUI, widget_mod_fm,false);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_mod_fm, "general", "mod");	
			fParameters_widget_number[TRICERATOPS_FM] = widget_mod_fm;
			
			int widget_mod_ring = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 10, panelY + 1.25, 2, 2, "RING", TRICERATOPS_MODIFIER_RING);
			
			Delirium_UI_Widget_Set_Value(GUI, widget_mod_ring,false);	
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_mod_ring, "general", "mod");
			fParameters_widget_number[TRICERATOPS_MODIFIER_RING] = widget_mod_ring;
			
			int widget_amp_drive = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0,
				panelX + 15, panelY + 1.25, 1, 5.25, "DRIVE",TRICERATOPS_AMP_DRIVE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_amp_drive, 1,0);
			Delirium_UI_Widget_Set_Integer(GUI, widget_amp_drive, false);
			Delirium_UI_Widget_Set_Value(GUI, widget_amp_drive,0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_amp_drive, "general", "mod");
			fParameters_widget_number[TRICERATOPS_AMP_DRIVE] = widget_amp_drive;
			
			// PRESETS

			widget_categories_list = Delirium_UI_Create_Widget(GUI, deliriumUI_List, 0, panelX + 0.5,panelY + 1.25, 7,5, "CATEGORIES", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_categories_list, "general", "presets");
					
			widget_presets_list = Delirium_UI_Create_Widget(GUI, deliriumUI_List, 0, panelX + 8.5,panelY + 1.25, 10, 5, "PRESETS", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_presets_list, "general", "presets");
								
			loadSymbols();
			searchPresets();
					
			
			//-----------------------------

			GUI->draw_flag = true;					
			GUI->drag = 0;
			GUI->current_widget = -1;	
			
			Delirium_UI_Group_Set_Active_Widgets(GUI);

		}

		//------------------------------------------------------------------------------------------------------

		~triceratopsUI()
		{
			Delirium_UI_Cleanup(GUI);
		}
		

		//------------------------------------------------------------------------------------------------------

		vector<string> split (const string &s, char delim) {
			    vector <string> result;
			    stringstream ss (s);
			    string item;
	
			    while (getline (ss, item, delim)) {
			        result.push_back (item);
				    }
		
		    return result;
		}
		
		//------------------------------------------------------------------------------------------------------
		void loadSymbols()
		{
			string line;

			string lv2_path = "/usr/lib/lv2"; // getenv("LV2_PATH");				
			struct dirent *d;
			struct stat st;
			DIR *dr;
			stringstream triceratops_ttl_file_name;
			number_of_symbols = 0;
					
			for (int x=0; x<200; x++) symbols.push_back("");
			
			vector<string> v = split (lv2_path, ':');
			
			for (int z=0; z<v.size(); z++)
			{
				triceratops_ttl_file_name.str("");
				triceratops_ttl_file_name << v[z] << "/thunderox_triceratops.lv2/thunderox_triceratops_dsp.ttl";


				ifstream triceratops_ttl_file(triceratops_ttl_file_name.str().c_str(), ios::in);
				
				while (getline(triceratops_ttl_file,line))  
				{
				
					stringstream symbol_name;
					symbol_name.str("");
					int symbol_index;
					bool symbol_found = false;
						
					int search_pos = line.rfind("lv2:symbol");
					if (search_pos > 0)
					{
						symbol_found = true;
						int add_this_char = 0;
						
						for (int char_number = 0; char_number < line.length(); char_number++)
						{
							if (line[char_number] == 34) add_this_char = 1 - add_this_char;
							else if (add_this_char) symbol_name << line[char_number];
						}
						
						

					}					
					
					search_pos = line.rfind("lv2:index");
					
					if (search_pos > 0)
					{
						istringstream (line.substr(search_pos+10)) >> symbol_index;
					}
					
					if (symbol_found)
					{
						symbols[symbol_index] = symbol_name.str();
						if (symbol_index > number_of_symbols) number_of_symbols = symbol_index;
					}	
				}
				
				triceratops_ttl_file.close();
			}
		}


		
		//------------------------------------------------------------------------------------------------------
		
		void searchPresets()
		{
			string lv2_path = "/usr/lib/lv2"; // getenv("LV2_PATH");			
			stringstream ss;
			struct dirent *d, *pr_d;
			struct stat st;
			DIR *dr, *prDr;
			stringstream file_name_and_path;
			stringstream path_name;
			
			/*
		
			ifstream categories_file("/usr/lib/lv2/thunderox_triceratops.lv2/triceratops_presets.lv2/categories.txt");
			string line;
		
			int category_number = 0;
			

			while (getline(categories_file,line))
			{
				category new_category;
				new_category.number = category_number++;
				new_category.name = line;
				categories.push_back(new_category);
				Delirium_UI_Widget_List_Add_Item(GUI, widget_categories_list, line);
			}  
			categories_file.close();	

			*/

			// preset_category_file.open("test.txt");
			vector<string> v = split (lv2_path, ':');
			    
			for (int z=0; z<v.size(); z++)
			{
				dr = opendir(v[z].c_str()); // search through LV2 folders in LV2_PATH 
				
				if (dr!=NULL)
				{
					for( d=readdir(dr); d!=NULL; d=readdir(dr)) // List all files here
					{
						string file_name = d->d_name;
							if (file_name == "thunderox_triceratops.lv2")
							{
								file_name = "thunderox_triceratops.lv2/triceratops_presets.lv2";
							}
							
							if (stat(file_name.c_str(),&st)) // Look in each folder
							{
								path_name.str("");
								path_name << v[z] << "/" << file_name;
								prDr = opendir(path_name.str().c_str()); // Read file see if it applies to our plugin

								if (prDr!=NULL)
								{
									for( pr_d=readdir(prDr); pr_d!=NULL; pr_d=readdir(prDr))
									{
										file_name_and_path.str("");
										file_name_and_path <<  v[z] << "/" << file_name << "/" << pr_d->d_name;
										
										int file_is_ttl = file_name_and_path.str().rfind(".ttl");
										int file_is_manifest = file_name_and_path.str().rfind("manifest.ttl");
										int file_is_triceratops = file_name_and_path.str().rfind("triceratops.ttl");
										
										if (file_is_ttl > 0 && file_is_manifest < 0 && file_is_triceratops < 0)
										{
																
											bool is_triceratops_preset = false;
											ifstream preset_file(file_name_and_path.str().c_str(), ios::in);
											string line;
									
											while (getline(preset_file,line))  
											{
												int search_pos = line.rfind("triceratops");
												if (search_pos > 0)
												{
													is_triceratops_preset = true;

												}
												
												/*
												
												search_pos = line.rfind("preset_category");
												if (search_pos > 0)
												{
													getline(preset_file,line); 
													search_pos = line.rfind("pset:value");
													istringstream value_str;
													istringstream ( line.substr(search_pos + 11, line.length()+1)
														) >> category_number;
												}
												
												*/
										
											}
																			
											preset_file.close();
											if (is_triceratops_preset)
											{
												preset new_preset;
												new_preset.file = file_name_and_path.str().c_str();
												string preset_name = pr_d->d_name;
												new_preset.name = preset_name.substr(0,preset_name.length()-4);
												
												string category_name = Find_Preset_Category(new_preset.file);
												bool category_found = false;
												
												for (int x=0; x<categories.size(); x++)
												{
													if (categories[x].name == category_name)
													{
														categories[x].presets.push_back(new_preset);
														category_found = true;
													}
												}
												
												if (!category_found)
												{
													category new_category;
													new_category.name = category_name;
													new_category.presets.push_back(new_preset);			
													categories.push_back(new_category);																																					
												}
												
												
												
												
												// Find_Preset_Category(new_preset.file);
											}	
										}			
								}
								closedir(prDr);
							}
						}
					}
					closedir(dr);
				}				
			}		

			/*

			for (int pr=0; pr<categories[0].presets.size(); pr++)
			{			
				Delirium_UI_Widget_List_Add_Item(GUI, widget_presets_list, categories[0].presets[pr].name);
			}
			
			*/
			
			sort(categories.begin(),categories.end(),alphasort_category());
			
			for (int x=0; x<categories.size(); x++)
			{
				Delirium_UI_Widget_List_Add_Item(GUI, widget_categories_list, categories[x].name);
			}
			
			for (int x=0; x<categories.size(); x++)
			{
				sort(categories[x].presets.begin(),categories[x].presets.end(),alphasort_preset());	
			}
			
		}
		
		//--------------------------------------------------------------------------------------
		// GET SYMBOL INDEX NUMBER (PORT)
		
		int getSymbolIndex(string symbol_name)
		{
			int symbol_index = -1;
			
			for (int x=0; x<number_of_symbols; x++)
			{
				if (symbols[x] == symbol_name)
					symbol_index = x;
			}	
			return symbol_index;
		}
		
		//--------------------------------------------------------------------------------------
		// LOAD PRESET

		void loadPreset()
		{ 	
			int category_number = current_category;

			int preset_number = GUI->Widgets[widget_presets_list]->list_position
				+ GUI->Widgets[widget_presets_list]->list_scroll;
				
			if ( preset_number > categories[category_number].presets.size()-1
				|| category_number > categories.size()-1
				|| categories[category_number].presets.size() == 0) return;
				
			string preset_path_and_file_name = categories[category_number].presets[preset_number].file;			

			ifstream preset_file;
	
			cout << preset_path_and_file_name << endl;
			preset_file.open(preset_path_and_file_name );

			string preset_symbol;

			string temp_str;
			string line;
			double preset_value;

			parameterChanged(TRICERATOPS_DSP_RUN, false);
			
			while (getline(preset_file,line))  
			{
				int preset_index = line.rfind("lv2:symbol ");
				if (preset_index > 0) preset_symbol = line.substr(preset_index + 12, line.length() - preset_index - 15);

				preset_index = line.rfind("pset:value");
				if (preset_index > 0) istringstream ( line.substr(preset_index + 11, line.length()+1 ) ) >> preset_value;

				if (preset_index > 0)
				{
				
					int symbol_index = getSymbolIndex(preset_symbol) - 3;
					
					if (symbol_index > -1) 
					{
						/*
					
						if (symbol_index >= 36 && symbol_index <=40 ) 
						 {
						 	preset_value = 1 - preset_value;
						 }
						 
						if (symbol_index >= 44 && symbol_index <=47 ) 
						 {
						 	preset_value = 1 - preset_value;
						 }
						 
						if (symbol_index >= 52 && symbol_index <=55 ) 
						 {
						 	preset_value = 1 - preset_value;
						 }
						 
						 */
						 
						parameterChanged(symbol_index, preset_value);
						 
					}

				} 
			}
			
		
						
			preset_file.close();
			
			parameterChanged(TRICERATOPS_DSP_RUN, true);

		}

		//------------------------------------------------------------------------------------------------------
		string Find_Preset_Category(string file_name)
		{
		
			ifstream preset_file;
			preset_file.open(file_name );

			
			string category_name = "Unsorted";
			string line;
			
			while (getline(preset_file,line))  
			{

				int preset_index = line.rfind("pset:bank");

				if (preset_index > 0)
				{
				
					int start_index = line.rfind("<");
					int end_index = line.rfind(">") - start_index - 1;
					category_name =  line.substr(start_index+1, end_index);
					// cout << category_name << endl;
					

				} 
			}
			
		
						
			preset_file.close();
		
			return category_name;
		}

		//------------------------------------------------------------------------------------------------------

		 void onCairoDisplay(const CairoGraphicsContext& context)
		{
			cr = context.handle;

			int widget_filter_type = fParameters_widget_number[TRICERATOPS_FILTER_MODE];
			int widget_filter_cutoff = fParameters_widget_number[TRICERATOPS_FILTER_FREQUENCY];
			int widget_filter_resonance = fParameters_widget_number[TRICERATOPS_FILTER_RESONANCE];
		
			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[widget_filter_type];
			Delirium_UI_Widget_Filter* wdg_freq = (Delirium_UI_Widget_Filter*)GUI->Widgets[widget_filter_cutoff];
			Delirium_UI_Widget_Filter* wdg_res = (Delirium_UI_Widget_Filter*)GUI->Widgets[widget_filter_resonance];
			
			wdg->filter_frequency = wdg_freq->values[0];
			wdg->filter_resonance = wdg_res->values[0];	
			
			Delirium_UI_Display_All(GUI, cr);
			
			
		}
		
		//------------------------------------------------------------------------------------------------------

		bool onMouse(const MouseEvent& ev) override
		{
			

			if (ev.button == 1)
			{
				Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());
				Delirium_UI_Left_Button_Press(GUI, cr, ev.pos.getX(), ev.pos.getY());
				repaint();
				
				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
	
				
				if ( GUI->current_widget == widget_categories_list)
				{
					int category_number = GUI->Widgets[widget_categories_list]->list_position
						+ GUI->Widgets[widget_categories_list]->list_scroll;
				
					GUI->Widgets[widget_presets_list]->list_items.clear();
					GUI->Widgets[widget_presets_list]->list_scroll = 0;
					current_category = category_number;

					for (int pr=0; pr<categories[category_number].presets.size(); pr++)
					{			
						Delirium_UI_Widget_List_Add_Item(GUI, widget_presets_list, categories[category_number].presets[pr].name);
					}
					
				}
				
				if ( GUI->current_widget == widget_presets_list )
				{
					loadPreset();
				}
				
				if (parameter_number > -1)
				{	
				
					float value = Delirium_UI_Widget_Get_Value(GUI);
					setParameterValue(parameter_number, value);
			
					
					if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Fader_Route)
					{
						int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_FREQUENCY];
			 			Delirium_UI_Widget_Base* wdg = (Delirium_UI_Widget_Fader_Route*)GUI->Widgets[wdg_number];
						setParameterValue(parameter_number+1, Delirium_UI_Widget_Get_Route_Number(GUI));
					}
					 
					 if (parameter_number == TRICERATOPS_FILTER_FREQUENCY)
					 {
					 	int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_FREQUENCY];
			 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
						wdg->filter_frequency = value;
						GUI->Widgets[wdg_number]->Draw(cr);
					 }

					if (parameter_number == TRICERATOPS_FILTER_RESONANCE)
					 {
					 	int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_RESONANCE];
			 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
						wdg->filter_resonance = value;
						GUI->Widgets[wdg_number]->Draw(cr);
					 }
				}
				return true;

			}


			if (ev.button == 2)
			{

				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				if (parameter_number > -1)
				{
	
					setParameterValue(parameter_number, 0);
					Delirium_UI_Middle_Button_Press(GUI);	
					repaint();	
					
					if (parameter_number == TRICERATOPS_FILTER_FREQUENCY)
					{
						int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_MODE];
						Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
						wdg->filter_frequency = 0;
						GUI->Widgets[wdg_number]->Draw(cr);	
					}
					
					if (parameter_number == TRICERATOPS_FILTER_RESONANCE)
					 {
					 	int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_MODE];
			 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
						wdg->filter_resonance = 0;
						GUI->Widgets[wdg_number]->Draw(cr);
					 }			
				}
			}


			return true;
		}

		//------------------------------------------------------------------------------------------------------

		bool onMotion(const MotionEvent& ev)
		{

			Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());

			repaint();	

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
	

			if (parameter_number > -1)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);
				
				if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Fader_Route)
				{
					setParameterValue(parameter_number+1, Delirium_UI_Widget_Get_Route_Number(GUI));
				}
				
				 if (parameter_number == TRICERATOPS_FILTER_FREQUENCY)
				 {
				 	int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_MODE];
					Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
					wdg->filter_frequency = value;
					GUI->Widgets[wdg_number]->Draw(cr);
				 }
				 
				if (parameter_number == TRICERATOPS_FILTER_RESONANCE)
				 {
				 	int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_MODE];
		 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
					wdg->filter_resonance = value;
					GUI->Widgets[wdg_number]->Draw(cr);
				 }
					 
			}
			return true;

		}

		//------------------------------------------------------------------------------------------------------

		bool onScroll(const ScrollEvent& ev)
		{
			float delta = ev.delta.getY();

			int current_widget = GUI->current_widget;

			if (current_widget > 0)
			{	
				if (GUI->Widgets[current_widget]->type == deliriumUI_Panel) return true;
			
				GUI->Widgets[current_widget]->Mouse_Scroll(ev.pos.getX(), ev.pos.getY(), delta);
				Delirium_UI_Convert_Value_To_Range(GUI, current_widget);
				GUI->Widgets[current_widget]->Draw(cr);
				repaint();
			}

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);

			if (parameter_number > -1)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);

				if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Fader_Route)
				{
					setParameterValue(parameter_number+1, Delirium_UI_Widget_Get_Route_Number(GUI));
				}
				
				if (parameter_number == TRICERATOPS_FILTER_FREQUENCY)
				{
					int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_MODE];
					Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
					wdg->filter_frequency = value;
					GUI->Widgets[wdg_number]->Draw(cr);	
				}
				
				if (parameter_number == TRICERATOPS_FILTER_RESONANCE)
				 {
				 	int wdg_number = GUI->parameter_widget_number[TRICERATOPS_FILTER_MODE];
		 			Delirium_UI_Widget_Filter* wdg = (Delirium_UI_Widget_Filter*)GUI->Widgets[wdg_number];
					wdg->filter_resonance = value;
					GUI->Widgets[wdg_number]->Draw(cr);
				 }
			}

			return true;
		}
		



	protected:
	
		//--------------------------------------------------------------------------------------------------------
		void programLoaded(uint32_t index) override
		{
			GUI->draw_flag = true;
			Delirium_UI_Display_All(GUI, cr);
		}
		
		
		/* --------------------------------------------------------------------------------------------------------
		* DSP/Plugin Callbacks 
		A parameter has changed on the plugin side.
		This is called by the host to inform the UI about parameter changes.
		*/

		void parameterChanged(uint32_t index, float value) override
		{					

			// get widget number connected to this parameter
			int widget_number = fParameters_widget_number[index];

			if (widget_number > -1)
			{
				Delirium_UI_Widget_Base* wdg = (Delirium_UI_Widget_Base*)GUI->Widgets[widget_number];
				
				if (index == wdg->parameter_number && wdg->type == deliriumUI_ADSR) wdg->current_value = 0;
				if (index > wdg->parameter_number && wdg->type == deliriumUI_ADSR)
				{
					wdg->current_value = ( index - wdg->parameter_number);
				}
				
				if ((int)index > wdg->parameter_number && wdg->type == deliriumUI_Fader_Route)
					wdg->route_number = value;

				if ((int)index == wdg->parameter_number && wdg->type == deliriumUI_Fader_Route)
					Delirium_UI_Widget_Set_Value(GUI, widget_number, value);
		
				
				if (wdg->type != deliriumUI_Fader_Route) Delirium_UI_Widget_Set_Value(GUI, widget_number, value);
				
			
			
			}
			// trigger repaint

			fParameters[index] = value;
			setParameterValue(index, value);
			
			GUI->draw_flag = true;		
			repaint();
			
			
		}

	private:

		Delirium_UI_Surface* GUI;
		cairo_t* cr;
		float fParameters[kParameterCount+3];
		int fParameters_widget_number[kParameterCount+3];
		
		vector <string> symbols;
		int number_of_symbols;
		
		struct preset
		{
			string name;
			string file;
		};
		
		struct category
		{
			string name;
			vector <preset> presets;
		};
		
		struct alphasort_preset
		{
			inline bool operator() (const preset& struct1, const preset& struct2)
			{
				return (struct1.name < struct2.name);
			}
		};

		vector <category> categories;
		
		int widget_categories_list;
		int widget_presets_list;
		int current_category;


		struct alphasort_category
		{
			inline bool operator() (const category& struct1, const category& struct2)
			{
				return (struct1.name < struct2.name);
			}
		};
		
		/*  Set our UI class as non-copyable and add a leak detector just in case. */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(triceratopsUI)

};

UI* createUI()
{
    return new triceratopsUI;
}

END_NAMESPACE_DISTRHO


