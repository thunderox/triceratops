
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Knob::Draw(cairo_t* cr)
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;
	
	float knob_top = wY + (font_size / 4);

	cairo_set_source(cr, theme_background_grad);
	cairo_rectangle(cr, wX - font_size, wY - font_size, wW + (font_size*1.5), wH + font_size);
	cairo_fill(cr);

	cairo_set_line_width(cr, 2);
	
	float value = 1-normalised_values[0];

	float cos_x,sin_y,cos_x2,sin_y2;

	cos_x =  (wW/5) * (cos(((((1-value)*0.75)-0.3)*2) * M_PI));
	sin_y =  (wW/5) * (sin(((((1-value)*0.75)-0.3)*2) * M_PI));

	cairo_set_source_rgba(cr, 1.0,1.0,1.0,0.2);

	for (float scale=-.05; scale<1; scale+=0.05 )
	{
 		cos_x =  (wH/4.5) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		sin_y =  (wH/4.5) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));
 		cos_x2 =  (wH/3.5) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		sin_y2 =  (wH/3.5) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));

		if (scale > 0.95-value && value != 0) { cairo_set_source_rgba(cr, 1.0,1.0,1.0,0.6); }

		cairo_move_to(cr,  wX + (wW/2)  + cos_x, knob_top + (wH/2.2) + sin_y);
		cairo_line_to(cr,  wX + (wW/2) + cos_x2, knob_top + (wH/2.2) + sin_y2);
		cairo_stroke(cr);
	} 

	cos_x =  (wH/10) * (cos((((value*0.75)-0.61)*2) * M_PI));
	sin_y =  (wH/10) * (sin((((value*0.75)-0.61)*2) * M_PI));

	cairo_set_source_rgb(cr, 0,0,0);
	cairo_arc(cr, wX + (wW/2), knob_top + (wH/2.2), (wH/5), 0.0, 2 * M_PI);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

	cairo_set_source_rgb(cr, 0.2,0.2,0.2);

	if (hover) {
		cairo_set_source_rgba(cr, 0.4,0.4,0.4,1);
	} else {
		cairo_set_source_rgba(cr, 0.3,0.3,0.3,1);

	}

	cairo_arc(cr, wX + (wW/2), knob_top + (wH/2.2), (wH/5.5), 0.0, 2 * M_PI);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

	float scale = value+0.05;

	cos_x =  (wH/15) * (cos(((((scale)*0.75)-0.67)*2) * M_PI));
	sin_y =  (wH/15) * (sin(((((scale)*0.75)-0.67)*2) * M_PI));
	cos_x2 =  (wH/5) * (cos(((((scale)*0.75)-0.67)*2) * M_PI));
	sin_y2 =  (wH/5) * (sin(((((scale)*0.75)-0.67)*2) * M_PI));

	cairo_move_to(cr,  wX + (wW/2) + cos_x, knob_top + (wH/2.2) + sin_y);
	cairo_line_to(cr,  wX + (wW/2) + cos_x2,  knob_top + (wH/2.2) + sin_y2);
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

	if (integer)
	{
		values[current_value] = int(values[current_value]);
		number << values[current_value];
	}
	else
	{
		number << fixed << setprecision(4) << values[current_value];
	}
	
	cairo_text_extents(cr, number.str().c_str(), &extents);
	float x_text_centred = (wX + wW / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, wY + wH - font_size);
	cairo_show_text(cr, number.str().c_str());

	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (wX + wW / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, wY + font_size);
	cairo_show_text(cr, label.c_str());
}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Knob::Left_Button_Press(int xm, int ym)
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;
	
	float Knob_top = wY + (font_size / 2);
	float Knob_height = wH;

	float ypixel = (ym - wY);
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


