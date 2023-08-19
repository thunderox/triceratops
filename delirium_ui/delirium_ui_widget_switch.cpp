
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Switch::Draw(cairo_t* cr)
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;

	cairo_set_source(cr, theme_background_grad);
				
	cairo_rectangle(cr, wX - font_size, wY - font_size, wW + (font_size * 1.5), wH + font_size);
	cairo_fill(cr);

	float value = normalised_values[0];

	cairo_text_extents_t extents;
	cairo_set_font_size(cr, font_size);
	float x_text_centred;

	string txt;
	float switch_yp = wY + (wH / 2.0);

	if (value)
	{
		cairo_arc(cr, wX + (wW / 1.9), switch_yp, wW / 2.8, 0.0, 2 * M_PI);

		cairo_set_source_rgba(cr, 0,0,0,0.5);
		cairo_fill(cr);

		cairo_pattern_t* grad1;
		grad1 = cairo_pattern_create_linear(wX + (wW / 1.9), switch_yp, wX + (wW / 3), wY + (wH / 3) );

		cairo_pattern_add_color_stop_rgba(grad1, 0,0.2,0.2,0.2,1);
		cairo_pattern_add_color_stop_rgba(grad1, 0.3,0.4,0.4,0.4,1);
		cairo_pattern_add_color_stop_rgba(grad1, 1,0.0,0.0,0.0,1);

		cairo_arc(cr, wX + (wW / 1.9), switch_yp, wW / 2.75, 0.0, 2 * M_PI);

		cairo_set_source(cr, grad1);
		cairo_fill(cr);

		cairo_arc(cr, wX + (wW / 1.9), switch_yp, wW / 4.75, 0.0, 2 * M_PI);

		cairo_set_source_rgba(cr, 0,0,0,0.5);
		cairo_fill(cr);

		cairo_pattern_t* grad_led;
		grad_led = cairo_pattern_create_radial(wX + (wW / 1.9), switch_yp, 0, wX + (wW / 3), wY + (wH / 3), wH / 2);

		cairo_pattern_add_color_stop_rgba(grad_led, 0,0.0,0.9,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 0.3,0.0,0.1,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 0.9,0.0,0.10,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 1.0,0.0,0.0,0.0,1);

		cairo_arc(cr, wX + (wW / 1.7), switch_yp, wW / 6, 0.0, 2 * M_PI);
		cairo_set_source(cr, grad_led);
		cairo_fill(cr);

		txt = "ON";
		cairo_text_extents(cr, txt.c_str(), &extents);
		x_text_centred = (wX + (wW / 2)) - extents.width / 2.5;

		cairo_pattern_destroy(grad1);
		cairo_pattern_destroy(grad_led);

		if (hover) {
			cairo_set_source_rgba(cr, 1, 1, 1, 1);
		} else {
			cairo_set_source_rgba(cr, 1, 1, 1, 0.6);

		}
	}
	else
	{
		cairo_arc(cr, wX + (wW / 1.9), switch_yp, (wW / 2.8), 0.0, 2 * M_PI);

		cairo_set_source_rgba(cr, 0,0,0,0.5);
		cairo_fill(cr);
	
		cairo_pattern_t* grad1;

		grad1 = cairo_pattern_create_linear(wX + (wW / 1.9), switch_yp, wX + (wW / 3), wY + (wH / 3) );

		cairo_pattern_add_color_stop_rgba(grad1, 0,0.2,0.2,0.2,1);
		cairo_pattern_add_color_stop_rgba(grad1, 0.5,0.4,0.4,0.4,1);
		cairo_pattern_add_color_stop_rgba(grad1, 1,0.0,0.0,0.0,1);

		cairo_arc(cr, wX + (wW / 1.9), switch_yp, (wW / 2.75), 0.0, 2 * M_PI);

		cairo_set_source(cr, grad1);
		cairo_fill(cr);

		cairo_arc(cr, wX + (wW / 1.9), switch_yp, (wW / 4.75), 0.0, 2 * M_PI);

		cairo_set_source_rgba(cr, 0,0,0,0.5);
		cairo_fill(cr);

		cairo_pattern_t* grad_led;

		grad_led = cairo_pattern_create_radial(wX + (wW / 2), switch_yp, 0, wX + (wW / 2), wY + (wH / 2), (wW / 2.5) );

		cairo_pattern_add_color_stop_rgba(grad_led, 0,0.0,0.2,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 0.3,0.0,0.01,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 0.8,0.0,0.01,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 1.0,0.0,0.0,0.0,1);

		cairo_arc(cr, wX + (wW / 1.9), switch_yp, (wW / 5.75), 0.0, 2*M_PI);
		cairo_set_source(cr, grad_led);
		cairo_fill(cr);

		txt = "OFF";
		cairo_text_extents(cr, txt.c_str(), &extents);
		x_text_centred = (wX + (wW / 2)) - extents.width / 2.5;

		cairo_pattern_destroy(grad1);
		cairo_pattern_destroy(grad_led);

		if (hover) {
			cairo_set_source_rgba(cr, 1, 1, 1, 1);
		} else {
			cairo_set_source_rgba(cr, 1, 1, 1, 0.6);

		}		
	}


	cairo_move_to(cr,x_text_centred, wY + font_size);
	cairo_show_text(cr, txt.c_str());
	
	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (wX + (wW / 2)) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, wY + wH);
	cairo_show_text(cr, label.c_str());
	
}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Switch::Left_Button_Press(int xm, int ym)
{
	if (pressed) normalised_values[0] = 1 - normalised_values[0];
}


//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE SCROLL WHEEL

void Delirium_UI_Widget_Switch::Mouse_Scroll(int xm, int ym, float delta)
{
	if (delta == -1) normalised_values[0] = false;
	if (delta == 1) normalised_values[0] = true;

}

