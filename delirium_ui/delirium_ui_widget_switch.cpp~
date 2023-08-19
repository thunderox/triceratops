
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Switch::Draw(cairo_t* cr)
{
	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	float sx = widget_x_position;
	float sy = widget_y_position;
	float sw = widget_width;
	float sh = widget_height;

	float x = widget_x_position;
	float y = widget_y_position;
	float w = widget_width;
	float h = widget_height;

	float value = normalised_values[0];

	cairo_text_extents_t extents;
	cairo_set_font_size(cr, font_size);
	cairo_text_extents(cr, label.c_str(), &extents);
	float x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;

	string pig;
	float switch_yp = sy+(sh/3.0);

	if (value)
	{
		cairo_arc(cr, sx+(sw/1.9), switch_yp, sw/2.8, 0.0, 2 * M_PI);

		cairo_set_source_rgba(cr, 0,0,0,0.5);
		cairo_fill(cr);

		cairo_pattern_t* grad1;
		grad1 = cairo_pattern_create_linear(sx+(sw/1.9), switch_yp, sx+(sw/3), sy+(sh/3) );

		cairo_pattern_add_color_stop_rgba(grad1, 0,0.2,0.2,0.2,1);
		cairo_pattern_add_color_stop_rgba(grad1, 0.3,0.4,0.4,0.4,1);
		cairo_pattern_add_color_stop_rgba(grad1, 1,0.0,0.0,0.0,1);

		cairo_arc(cr, sx+(sw/1.9), switch_yp, sw/2.75, 0.0, 2 * M_PI);

		cairo_set_source(cr, grad1);
		cairo_fill(cr);

		cairo_arc(cr, sx+(sw/1.9), switch_yp, sw/4.75, 0.0, 2 * M_PI);

		cairo_set_source_rgba(cr, 0,0,0,0.5);
		cairo_fill(cr);

		cairo_pattern_t* grad_led;
		grad_led = cairo_pattern_create_radial(sx+(w/1.9), switch_yp, 0, sx+(sw/6), sy+(sh/6), h/2);

		cairo_pattern_add_color_stop_rgba(grad_led, 0,0.0,0.9,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 0.3,0.0,0.1,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 0.9,0.0,0.10,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 1.0,0.0,0.0,0.0,1);

		cairo_arc(cr, sx+(sw/1.7), switch_yp, sw/6, 0.0, 2 * M_PI);
		cairo_set_source(cr, grad_led);
		cairo_fill(cr);

		pig = "ON";
		cairo_text_extents(cr, pig.c_str(), &extents);
		x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;

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
		cairo_arc(cr, sx+(sw/1.9), switch_yp, (sw/2.8), 0.0, 2 * M_PI);

		cairo_set_source_rgba(cr, 0,0,0,0.5);
		cairo_fill(cr);
	
		cairo_pattern_t* grad1;

		grad1 = cairo_pattern_create_linear(sx+(sw/1.9), switch_yp, sx+(sw/3), sy+(sh/3) );

		cairo_pattern_add_color_stop_rgba(grad1, 0,0.2,0.2,0.2,1);
		cairo_pattern_add_color_stop_rgba(grad1, 0.5,0.4,0.4,0.4,1);
		cairo_pattern_add_color_stop_rgba(grad1, 1,0.0,0.0,0.0,1);

		cairo_arc(cr, sx+(sw/1.9), switch_yp, (sw/2.75), 0.0, 2 * M_PI);

		cairo_set_source(cr, grad1);
		cairo_fill(cr);

		cairo_arc(cr, sx+(sw/1.9), switch_yp, (sw/4.75), 0.0, 2 * M_PI);

		cairo_set_source_rgba(cr, 0,0,0,0.5);
		cairo_fill(cr);

		cairo_pattern_t* grad_led;

		grad_led = cairo_pattern_create_radial(sx+(sw/2), switch_yp, 0, sx+(sw/2), sy+(sh/2), (sw/2.5) );

		cairo_pattern_add_color_stop_rgba(grad_led, 0,0.0,0.2,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 0.3,0.0,0.01,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 0.8,0.0,0.01,0.0,1);
	        cairo_pattern_add_color_stop_rgba(grad_led, 1.0,0.0,0.0,0.0,1);

		cairo_arc(cr, sx+(sw/1.9), switch_yp, (sw/5.75), 0.0, 2*M_PI);
		cairo_set_source(cr, grad_led);
		cairo_fill(cr);

		pig = "OFF";
		cairo_text_extents(cr, pig.c_str(), &extents);
		x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;

		cairo_pattern_destroy(grad1);
		cairo_pattern_destroy(grad_led);

		if (hover) {
			cairo_set_source_rgba(cr, 1, 1, 1, 1);
		} else {
			cairo_set_source_rgba(cr, 1, 1, 1, 0.6);

		}		
	}

	cairo_move_to(cr,x_text_centred, sy);
	cairo_show_text(cr, pig.c_str());
}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Switch::Left_Button_Press(int xm, int ym)
{
	if (pressed) normalised_values[0] = 1 - normalised_values[0];
}


//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE SCROLL WHEEL

void Delirium_UI_Widget_Switch::Mouse_Scroll(float delta)
{
	if (delta == -1) normalised_values[0] = false;
	if (delta == 1) normalised_values[0] = true;

}

