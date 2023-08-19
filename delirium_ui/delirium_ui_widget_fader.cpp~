
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 


//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Fader::Draw(cairo_t* cr)
{
	if (integer) values[current_value] = int(values[current_value]);
	Convert_Scaled_To_Value();

	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	float sx = widget_x_position;
	float sy = widget_y_position;
	float sw = widget_width;
	float sh = widget_height;
	
	float fader_top = sy + (font_size * 1.5);
	float fader_height = sh - (font_size*4);
	float value_to_ypixel = (normalised_values[0] * fader_height);

	cairo_set_source_rgb(cr, 0.2,0,0);
	cairo_rectangle(cr, widget_x_position-(font_size),widget_y_position-font_size,widget_width+(font_size*3),widget_height+font_size);
	cairo_fill(cr);

	// ------------------------------------------------------------------

	// draw vertical scale bars

	cairo_set_line_width(cr, 0.4);
	cairo_set_source_rgba(cr, 0.9,0.9,0.9,1.0);


	for (int yl=0; yl<fader_height; yl+=(fader_height/11))
	{
		cairo_move_to(cr, sx+(sw/6), yl+fader_top);
	 	cairo_line_to(cr, sx+sw-(sw/6), yl+fader_top);
	    	cairo_stroke(cr);
	}

	// draw vertical grey line down the middle
	cairo_set_line_width(cr, 4);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_move_to(cr, sx+(sw/2), fader_top);
	cairo_line_to(cr, sx+(sw/2), fader_top + fader_height);
	cairo_stroke(cr);


	// draw horizontal thin black line at fader_value height
	cairo_set_line_width(cr, fader_height/6);
	cairo_set_source_rgba(cr, 0.0,0.0,0.0,1.0);
	cairo_move_to(cr, sx, fader_top+value_to_ypixel);
	cairo_line_to(cr, sx+sw, fader_top+value_to_ypixel);
	cairo_stroke(cr);


	// draw horizontal thick black line at fader_value height
	cairo_set_line_width(cr, fader_height/4);
	cairo_set_source_rgba(cr, 0.0,0.0,0.0,0.3);
	cairo_move_to(cr, sx+sw, fader_top+value_to_ypixel);
	cairo_line_to(cr, sx+sw, fader_top+value_to_ypixel + 1.1);
	cairo_stroke(cr);

	// set up grad
	cairo_pattern_t* pat;

	pat = cairo_pattern_create_linear(sx, fader_top+value_to_ypixel, sx+sw, fader_top+value_to_ypixel );
	cairo_pattern_add_color_stop_rgba(pat, 0.0,0.1,0.1,0.1,1);
        cairo_pattern_add_color_stop_rgba(pat, 0.4,0.7,0.7,0.7,1);
        cairo_pattern_add_color_stop_rgba(pat, 1.0,0.3,0.3,0.3,1);

	// draw horizontal blue line at fader_value height
	cairo_set_line_width(cr, sh/28);
	cairo_set_source(cr, pat);
	cairo_move_to(cr, sx, fader_top+value_to_ypixel);
	cairo_line_to(cr, sx+sw, fader_top+value_to_ypixel);
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
	float x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, widget_height+widget_y_position);
	cairo_show_text(cr, number.str().c_str());

	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, sy);
	cairo_show_text(cr, label.c_str());

	cairo_pattern_destroy(pat);
}

//-------------------------------------------------------------------------------------------
// USER PRESSED LEFT MOUSE BUTTON

void Delirium_UI_Widget_Fader::Left_Button_Press(int xm, int ym)
{
	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	float sx = widget_x_position;
	float sy = widget_y_position;
	float sw = widget_width;
	float sh = widget_height;
	
	float fader_top = sy + font_size;
	float fader_height = sh - (font_size*3);

	float ypixel = (ym - widget_y_position) - font_size;
	float value = (float)ypixel/ (fader_height);
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



