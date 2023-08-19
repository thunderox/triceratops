
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_ADSR::Draw(cairo_t* cr)
{
	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;

	float sx = x;
	float sy = y + (h/6);
	float sw = w;
	float sh = h * 0.8;

	cairo_set_source_rgb(cr, 0,0,0);
	cairo_rectangle(cr, x,y-font_size,w,h);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, 0.15,0,0);
	cairo_rectangle(cr, x,y,w,h);
	cairo_fill(cr);

	// DRAW LABEL

	cairo_text_extents_t extents;
	cairo_set_font_size(cr, font_size);
	float x_text_centred; 
	cairo_set_source_rgb(cr, 0.8,0.8,0.8);
	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (x + w / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, y);
	cairo_show_text(cr, label.c_str());

	// DRAW VALUE

	if (hover)
	{
	
		stringstream number;

		switch (current_value)
		{
			case 0:
				number << "Attack: ";		
				break;
	
			case 1:
				number << "Decay: ";
				break;
	
			case 2:
				number << "Sustain: ";
				break;
	
			case 3:
				number << "Release: ";
				break;
		}

		number << fixed << setprecision(4) << values[current_value];
	
		cairo_text_extents(cr, number.str().c_str(), &extents);
		x_text_centred = (x + w / 2) - extents.width / 2;
		cairo_move_to(cr,x_text_centred, (h+y)-(font_size*0.5));
		cairo_show_text(cr, number.str().c_str());
	}

	// ----------

	cairo_set_line_width(cr, w/80);
	cairo_move_to(cr, sx+(w/5), sy);

	float yp;

	cairo_set_source_rgba(cr, 0.7, 0.7, 0.7, 1);

	for (int xc=1; xc<5; xc++)
	{
		float xcol = xc * (w / 5);
		
		yp = (normalised_values[xc-1] * (h/2));

		if (xc > 1)
		{
			cairo_line_to(cr,  sx+(xcol), sy+yp);
			cairo_stroke(cr);
		}
				
		if (hover && xc==current_value+1 )
		{
			cairo_set_source_rgba(cr, 0.9, 0.9, 0.9, 1);
			cairo_arc(cr, sx+(xcol), sy+yp, (h/20), 0.0, 2 * M_PI);
		} else
		{
			cairo_set_source_rgba(cr, 0.7, 0.7, 0.7, 1);
			cairo_arc(cr, sx+(xcol), sy+yp, (h/24), 0.0, 2 * M_PI);
		}

		cairo_fill(cr);


		cairo_move_to(cr, sx+xcol, sy+yp);

	}

}

//-------------------------------------------------------------------------------------------
// USER PRESSED LEFT MOUSE BUTTON

void Delirium_UI_Widget_ADSR::Left_Button_Press(int xm, int ym)
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
	float fader_height = sh * 0.5;

	float ypixel = (ym - widget_y_position) - font_size;
	float value = (float)ypixel/ (fader_height);
	if (value < 0) value = 0;
	if (value > 1) value = 1;
	
	normalised_values[current_value] = value;
}


//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_ADSR::Mouse_Over(int xm, int ym)
{

	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;

	int new_current_value = int((xm-x)/(w/5))-1;

	if (new_current_value >= 0 && new_current_value <= 3) current_value = new_current_value;
	
}





