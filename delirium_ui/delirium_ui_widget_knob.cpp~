
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Knob::Draw(cairo_t* cr)
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

	float x = widget_x_position;
	float y = widget_y_position;
	float w = widget_width;
	float h = widget_height;

	cairo_set_source_rgb(cr, 0.2,0,0);
	cairo_rectangle(cr, widget_x_position-font_size,widget_y_position-font_size,widget_width+(font_size*1.5),widget_height+font_size);
	cairo_fill(cr);

	cairo_set_line_width(cr, 2);
	
	float value = 1-normalised_values[0];

	float cos_x,sin_y,cos_x2,sin_y2;

	cos_x =  (w/7) * (cos(((((1-value)*0.75)-0.3)*2) * M_PI));
	sin_y =  (w/7) * (sin(((((1-value)*0.75)-0.3)*2) * M_PI));

	cairo_set_source_rgba(cr, 1.0,1.0,1.0,0.2);

	for (float scale=-.05; scale<1; scale+=0.05 )
	{
 		cos_x =  (h/3.5) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		sin_y =  (h/3.5) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));
 		cos_x2 =  (h/2.75) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		sin_y2 =  (h/2.75) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));

		if (scale > 0.95-value && value != 0) { cairo_set_source_rgba(cr, 1.0,1.0,1.0,0.6); }

		cairo_move_to(cr,  x + (w/2) + cos_x, y + (h/2.2) + sin_y);
		cairo_line_to(cr,  x + (w/2) + cos_x2, y + (h/2.2) + sin_y2);
		cairo_stroke(cr);
	} 

	cos_x =  (h/10) * (cos((((value*0.75)-0.61)*2) * M_PI));
	sin_y =  (h/10) * (sin((((value*0.75)-0.61)*2) * M_PI));

	cairo_set_source_rgb(cr, 0,0,0);
	cairo_arc(cr, x + (w/2), y + (h/2.2), (h/4), 0.0, 2 * M_PI);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

	cairo_set_source_rgb(cr, 0.2,0.2,0.2);

	if (hover) {
		cairo_set_source_rgba(cr, 0.4,0.4,0.4,1);
	} else {
		cairo_set_source_rgba(cr, 0.3,0.3,0.3,1);

	}

	cairo_arc(cr, x + (w/2), y + (h/2.2), (h/4.5), 0.0, 2 * M_PI);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

	float scale = value+0.05;

	cos_x =  (h/15) * (cos(((((scale)*0.75)-0.67)*2) * M_PI));
	sin_y =  (h/15) * (sin(((((scale)*0.75)-0.67)*2) * M_PI));
	cos_x2 =  (h/5) * (cos(((((scale)*0.75)-0.67)*2) * M_PI));
	sin_y2 =  (h/5) * (sin(((((scale)*0.75)-0.67)*2) * M_PI));

	cairo_move_to(cr,  x + (w/2) + cos_x, y + (h/2.2) + sin_y);
	cairo_line_to(cr,  x + (w/2) + cos_x2, y + (h/2.2) + sin_y2);
	cairo_stroke(cr);

	// -- text

	if (hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);

	}

	cairo_text_extents_t extents;
	cairo_set_font_size(cr, font_size);

	stringstream number;

	if (integer) values[current_value] = int(values[current_value]);
	number << fixed << setprecision(4) << values[current_value];

	cairo_text_extents(cr, number.str().c_str(), &extents);
	float x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, widget_height+widget_y_position);
	cairo_show_text(cr, number.str().c_str());

	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, sy);
	cairo_show_text(cr, label.c_str());
}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Knob::Left_Button_Press(int xm, int ym)
{
	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	float sx = widget_x_position;
	float sy = widget_y_position;
	float sw = widget_width;
	float sh = widget_height;
	
	float Knob_top = sy + font_size;
	float Knob_height = sh;

	float ypixel = (ym - widget_y_position);
	float value = (float)ypixel/ (Knob_height);
	if (value < 0) value = 0;
	if (value > 1) value = 1;
	normalised_values[0] = value;
}

//-------------------------------------------------------------------------------------------
// USER PRESSED MIDDLE MOUSE BUTTON - RESET WIDGET TO DEFAULT VALUE

void Delirium_UI_Widget_Knob::Middle_Button_Press()
{
	values[current_value] = default_values[current_value];
}


