
#include "delirium_ui.hpp"
#include <sstream>
#include <bits/stdc++.h> 

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Fader::Draw(cairo_t* cr)
{
	if (integer) values[current_value] = int(values[current_value]);
	Convert_Scaled_To_Value();

	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;
	
	float fader_top = wY + (font_size * 2.5);
	float fader_height = wH * 0.625;
	float value_to_ypixel = (normalised_values[0] * fader_height);


	cairo_set_source(cr, theme_background_grad);
				
	cairo_rectangle(cr, wX - (font_size), wY - font_size, wW + (font_size*3), wH+font_size);
	cairo_fill(cr);

	// ------------------------------------------------------------------

	// draw vertical scale bars

	cairo_set_line_width(cr, 0.4);
	cairo_set_source_rgba(cr, 0.9,0.9,0.9,1.0);


	for (int yl=0; yl<fader_height + font_size; yl+=(fader_height/12))
	{
		cairo_move_to(cr, wX + (wW/6), yl + fader_top);
	 	cairo_line_to(cr, wX + wW - (wW/6), yl + fader_top);
	    	cairo_stroke(cr);
	}

	// draw vertical grey line down the middle
	cairo_set_line_width(cr, 4);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_move_to(cr, wX + (wW / 2), fader_top);
	cairo_line_to(cr, wX + (wW / 2), fader_top + fader_height + font_size);
	cairo_stroke(cr);


	// draw horizontal thin black line at fader_value height
	cairo_set_line_width(cr, fader_height/6);
	cairo_set_source_rgba(cr, 0.0,0.0,0.0,1.0);
	cairo_move_to(cr, wX, fader_top + value_to_ypixel);
	cairo_line_to(cr, wX + wW, fader_top+value_to_ypixel);
	cairo_stroke(cr);


	// draw horizontal thick black line at fader_value height
	cairo_set_line_width(cr, fader_height/4);
	cairo_set_source_rgba(cr, 0.0,0.0,0.0,0.3);
	cairo_move_to(cr, wX + wW, fader_top+value_to_ypixel);
	cairo_line_to(cr, wX + wW, fader_top+value_to_ypixel + 1.1);
	cairo_stroke(cr);

	// set up grad
	cairo_pattern_t* pat;

	pat = cairo_pattern_create_linear(wX, fader_top + value_to_ypixel, wX + wW, fader_top + value_to_ypixel );
	cairo_pattern_add_color_stop_rgba(pat, 0.0,0.1,0.1,0.1,1);
        cairo_pattern_add_color_stop_rgba(pat, 0.4,0.7,0.7,0.7,1);
        cairo_pattern_add_color_stop_rgba(pat, 1.0,0.3,0.3,0.3,1);

	// draw horizontal blue line at fader_value height
	cairo_set_line_width(cr, wH / 28);
	cairo_set_source(cr, pat);
	cairo_move_to(cr, wX, fader_top + value_to_ypixel);
	cairo_line_to(cr, wX + wW, fader_top+value_to_ypixel);
	cairo_stroke(cr);

	cairo_set_line_width(cr, 2);

	// -- text

	if (hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);

	}

	cairo_text_extents_t extents;
	cairo_set_font_size(cr, font_size);

	stringstream number;
	number << fixed << setprecision(4) << values[current_value];

	cairo_text_extents(cr, number.str().c_str(), &extents);
	float x_text_centred = (wX + wW / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, wY + wH - font_size);
	cairo_show_text(cr, number.str().c_str());

	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (wX + wW / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, wY + font_size);
	cairo_show_text(cr, label.c_str());

	cairo_pattern_destroy(pat);
}

//-------------------------------------------------------------------------------------------
// USER PRESSED LEFT MOUSE BUTTON

void Delirium_UI_Widget_Fader::Left_Button_Press(int xm, int ym)
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;
	
	float fader_top = wY + font_size;
	float fader_height = wH - (font_size*3);

	float ypixel = (ym - wY) - font_size;
	float value = (float)ypixel / (fader_height);
	if (value < 0) value = 0;
	if (value > 1) value = 1;
	normalised_values[0] = value;
}

//-------------------------------------------------------------------------------------------
// USER PRESSED MIDDLE MOUSE BUTTON - RESET WIDGET TO DEFAULT VALUE

void Delirium_UI_Widget_Fader::Middle_Button_Press()
{
	values[current_value] = default_values[current_value];
}





