/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2019 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "Window.hpp"

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

			GUI = Delirium_UI_Init(static_cast<float>(getWidth()),static_cast<float>(getHeight()), 40,28);

			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, 1, 1, 18,5.75, "OSCILLATORS", 0);

			int widget_logo = Delirium_UI_Create_Widget(GUI, deliriumUI_Logo, 0, 14.5, 1, 3.5, 6, "", kParametertriceratopsTime);

			int widget_osc1_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, 1.5, 1.5, 2, 2, "ON", kParametertriceratopsTime);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_active, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 
	
			int widget_osc1_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, 1.5, 4.5, 2, 2, "VOL", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_volume, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc1_volume, 0.5);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 

			int widget_osc1_pan = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, 4, 4.5, 2, 2, "PAN", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_pan, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_pan, 0);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_pan; 

			int widget_triceratops_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 7.5, 1.5, 1, 5, "DETUNE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_triceratops_amount, -1,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_triceratops_amount, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc1_glide = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, 9.5, 4.5, 2, 2, "GLIDE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_glide, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_glide, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc1_octave = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, 12, 1.5, 2, 2, "OCTAVE", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_octave, 0,5);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_octave, 0);
			Delirium_UI_Widget_Set_Increment(GUI, widget_osc1_octave, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_osc1_octave, true);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_osc1_pulsewidth = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, 12, 4.5, 2, 2, "PW", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_pulsewidth, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_pulsewidth, 0.5);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_osc1_pulsewidth, 0.5);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 


			//-----------------------------------------------------------------------------------

			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, 24, 1, 18,5.75, "LFO", 0);

			int widget_lfo1_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, 24.75, 1.5, 2, 2, "ON", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_retrig, 1);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 

			int widget_lfo1_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, 25, 4.5, 2, 2, "BPM", kParametertriceratopsTime);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_speed, 0,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo1_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo1_speed, true);
			// fParameters_widget_number[kParametertriceratopsTime] = widget_osc1_volume; 

			int widget_lfo1_osc1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 28, 1.5, 1, 5, "OSC-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_osc2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 30, 1.5, 1, 5, "OSC-2", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_osc3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 32, 1.5, 1, 5, "OSC-3", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 34, 1.5, 1, 5, "FILTER", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 36, 1.5, 1, 5, "OSC1-PW", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			int widget_lfo1_route3 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 38, 1.5, 1, 5, "AMP", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_route3, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_route3, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount;

			//--------------------------------------------------------------------------------------------

			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, 1, 7.5, 16,5.75, "ENVELOPES", 0);



			int widget_amp_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, 2, 8.5, 6, 2,"Amp",0); 

			int widget_filter_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, 2, 11, 6, 2,"Filter",0);
 
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_env1_route1 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 12, 8, 1, 5, "AMP", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env1_route1, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env1_route1, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			int widget_env1_route2 = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, 14.25, 8, 1, 5, "OSC-1", kParametertriceratopsAmount);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_env1_route2, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_env1_route2, 0);
			// fParameters_widget_number[kParametertriceratopsAmount] = widget_triceratops_amount; 

			//--------------------------------------------------------------------------------------------

			Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, 18, 7.5, 15.5,5.75, "AMP/FX", 0);

			int widget_selector = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0,20,8.5,2,4,"WAVE", 0);

			int widget_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, 27, 9, 3, 3, "VOLUME", kParametertriceratopsVolume);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_volume, 0.6);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_volume, 0.6);
			fParameters_widget_number[kParametertriceratopsVolume] = widget_volume; 

			GUI->draw_flag = true;

			cout << "CAIRO WINDOW = " << getParentWindow().getGraphicsContext().cairo << endl;
		}

		//------------------------------------------------------------------------------------------------------

		~triceratopsUI()
		{
			Delirium_UI_Cleanup(GUI);
		}

		//------------------------------------------------------------------------------------------------------

		void onDisplay()
		{
			cairo_t* cr = getParentWindow().getGraphicsContext().cairo;
			Delirium_UI_Display_All(GUI, cr);
		}

		bool onMouse(const MouseEvent& ev) override
		{
			cairo_t* cr = getParentWindow().getGraphicsContext().cairo;	

			if (ev.button == 1)
			{
				Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());
				Delirium_UI_Left_Button_Press(GUI, cr, ev.pos.getX(), ev.pos.getY());
				repaint();
				
				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				if (parameter_number > 0)
				{	
					float value = Delirium_UI_Widget_Get_Value(GUI);
					 setParameterValue(parameter_number, value);
				}
				return true;

			}


			if (ev.button == 2)
			{

				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				if (parameter_number > 0)
				{
	
					setParameterValue(parameter_number, 0);
					Delirium_UI_Middle_Button_Press(GUI);	
					repaint();	
				}
			}


			return true;
		}

		//------------------------------------------------------------------------------------------------------

		bool onMotion(const MotionEvent& ev)
		{
			cairo_t* cr = getParentWindow().getGraphicsContext().cairo;

			Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());

			repaint();	

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
	

			if (parameter_number > 0)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);
			}
			return true;

		}

		//------------------------------------------------------------------------------------------------------

		bool onScroll(const ScrollEvent& ev)
		{
			float delta = ev.delta.getY();
			cairo_t* cr = getParentWindow().getGraphicsContext().cairo;

			int current_widget = GUI->current_widget;

			if (current_widget > 0)
			{	
				GUI->Widgets[current_widget]->Mouse_Scroll(delta);
				Delirium_UI_Convert_Value_To_Range(GUI, current_widget);
				GUI->Widgets[current_widget]->Draw(cr);
				repaint();
			}

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);

			if (parameter_number > 0)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);
			}

			return true;
		}


	protected:

		/* --------------------------------------------------------------------------------------------------------
		* DSP/Plugin Callbacks 
		A parameter has changed on the plugin side.
		This is called by the host to inform the UI about parameter changes.
		*/

		void parameterChanged(uint32_t index, float value) override
		{
			fParameters[index] = value;

			// get widget number connected to this parameter
			int widget_number = fParameters_widget_number[index];

			if (widget_number > 0)
				Delirium_UI_Widget_Set_Value(GUI, widget_number, value);

			// trigger repaint
			repaint();
		}

	private:

		Delirium_UI_Surface* GUI;
		float fParameters[kParameterCount];
		int fParameters_widget_number[kParameterCount];

		/*  Set our UI class as non-copyable and add a leak detector just in case. */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(triceratopsUI)

};

UI* createUI()
{
    return new triceratopsUI;
}

END_NAMESPACE_DISTRHO


