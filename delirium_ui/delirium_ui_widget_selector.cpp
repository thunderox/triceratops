
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Selector::Draw(cairo_t* cr)
{
	if (integer) values[current_value] = int(values[current_value]);
	Convert_Scaled_To_Value();

	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;

	increment = 1;

	cairo_set_source(cr, theme_background_grad);
	cairo_rectangle(cr, x, y, w, h);
	cairo_fill(cr);
	
	cairo_set_source_rgb(cr, 0.15,0,0);
	cairo_rectangle(cr, x, y + (font_size), w, h - (font_size * 3) );
	cairo_fill(cr);
	
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, font_size);
	float x_text_centred; 

	// SHOW CURRENT WAVEFORM
	
	if (hover) {
		cairo_set_source_rgb(cr, 1, 1, 1);
	} else {
		cairo_set_source_rgb(cr, 0.6,0.6,0.6);
	}
					
	if (wave_mode == WAVE_MODE_OSC)
	{
					
		switch (int(values[current_value]))
		{
			case 0:
			// SAW

				cairo_text_extents(cr, "SAW", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "SAW");
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
				
				cairo_move_to(cr,x+(w/4), y+(h/1.5));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/3));
				cairo_stroke(cr);
				break;
				
			case 1:	
			// SQUARE

				cairo_text_extents(cr, "SQUARE", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "SQUARE");
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
					
				cairo_move_to(cr, x+(w/4), y+(h/1.5));
				cairo_line_to(cr, x+(w/4), y+(h/3));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/3));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/1.5));
				cairo_stroke(cr);
				break;
				
			case 2:
			// SINE

				cairo_text_extents(cr, "SINE", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "SINE"); 
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
					
				cairo_move_to(cr, x+(w/4), y+(h/2.75));
		  		cairo_curve_to(cr, x+(w/2), y+(h*1.5), x+(w/2),y-(h/2), x+(w-(w/4)),y+(h/1.5));
				cairo_stroke(cr);
				break;
				
			case 3:

			// NOISE

				cairo_text_extents(cr, "NOISE", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "NOISE");
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
					
				cairo_move_to(cr, x+(w/8), y+(h/2));
				cairo_line_to(cr, x+((w/12)*1), y+(h/2) + h/8);
				cairo_line_to(cr, x+((w/12)*2), y+(h/2) - h/8);	
				cairo_line_to(cr, x+((w/12)*3), y+(h/2) + h/8);
				cairo_line_to(cr, x+((w/12)*4), y+(h/2) - h/5);
				cairo_line_to(cr, x+((w/12)*5), y+(h/2) + h/5);
				cairo_line_to(cr, x+((w/12)*6), y+(h/2) - h/8);	
				cairo_line_to(cr, x+((w/12)*7), y+(h/2) + h/9);
				cairo_line_to(cr, x+((w/12)*8), y+(h/2) - h/5);
				cairo_line_to(cr, x+((w/12)*9), y+(h/2) + h/6);
				cairo_line_to(cr, x+((w/12)*10), y+(h/2));				
				cairo_stroke(cr);
				break;
				
		}
	}
	
	
	//--------
	
		if (wave_mode == WAVE_MODE_LFO)
	{
					
		switch (int(values[current_value]))
		{
			case 0:
			// SAW

				cairo_text_extents(cr, "SAW", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "SAW");
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
				
				cairo_move_to(cr,x+(w/4), y+(h/1.5));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/3));
				cairo_stroke(cr);
				break;
				
			case 1:	
			// SQUARE

				cairo_text_extents(cr, "SQUARE", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "SQUARE");
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
					
				cairo_move_to(cr, x+(w/4), y+(h/1.5));
				cairo_line_to(cr, x+(w/4), y+(h/3));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/3));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/1.5));
				cairo_stroke(cr);
				break;
				
			case 2:
			// SINE

				cairo_text_extents(cr, "SINE", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "SINE"); 
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
					
				cairo_move_to(cr, x+(w/4), y+(h/2.75));
		  		cairo_curve_to(cr, x+(w/2), y+(h*1.5), x+(w/2),y-(h/2), x+(w-(w/4)),y+(h/1.5));
				cairo_stroke(cr);
				break;
				
			case 3:
			// S&H

				cairo_text_extents(cr, "S&H", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "S&H"); 
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
				
				cairo_move_to(cr, x+(w/8), y+(h/1.75));
				cairo_line_to(cr, x+((w/6)*1), y+(h/2) + h/5);	
				cairo_line_to(cr, x+((w/6)*2), y+(h/2) + h/5);	
				cairo_line_to(cr, x+((w/6)*2), y+(h/2) - h/4);
				cairo_line_to(cr, x+((w/6)*3), y+(h/2) - h/4);		
				cairo_line_to(cr, x+((w/6)*3), y+(h/2) + h/8);	
				cairo_line_to(cr, x+((w/6)*4), y+(h/2) + h/8);	
				cairo_line_to(cr, x+((w/6)*4), y+(h/2) - h/8);			
				cairo_line_to(cr, x+((w/6)*5), y+(h/2) - h/8);		
				cairo_line_to(cr, x+((w/6)*5), y+(h/2) + h/5);				
				cairo_stroke(cr);
				break;
				
			case 4:
			// NOISE

				cairo_text_extents(cr, "NOISE", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "NOISE");
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
					
				cairo_move_to(cr, x+(w/8), y+(h/2));
				cairo_line_to(cr, x+((w/12)*1), y+(h/2) + h/8);
				cairo_line_to(cr, x+((w/12)*2), y+(h/2) - h/8);	
				cairo_line_to(cr, x+((w/12)*3), y+(h/2) + h/8);
				cairo_line_to(cr, x+((w/12)*4), y+(h/2) - h/5);
				cairo_line_to(cr, x+((w/12)*5), y+(h/2) + h/5);
				cairo_line_to(cr, x+((w/12)*6), y+(h/2) - h/8);	
				cairo_line_to(cr, x+((w/12)*7), y+(h/2) + h/9);
				cairo_line_to(cr, x+((w/12)*8), y+(h/2) - h/5);
				cairo_line_to(cr, x+((w/12)*9), y+(h/2) + h/6);
				cairo_line_to(cr, x+((w/12)*10), y+(h/2));				
				cairo_stroke(cr);
				break;
				
			case 5:
			// ISAW

				cairo_text_extents(cr, "ISAW", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "ISAW");

				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
							
				cairo_move_to(cr,x+(w/4), y+(h/3));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/1.5));
				cairo_stroke(cr);
				break;
				
			case 6:	
			// SQUARE
				
				cairo_text_extents(cr, "ISQUARE", &extents);
				x_text_centred = (x + w / 2) - extents.width / 2;
				cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
				cairo_show_text(cr, "ISQUARE");
				
				if (hover)
				{
					cairo_set_source_rgb(cr, 0.7,0.7,0.7);
				} else {
					cairo_set_source_rgb(cr, 0.5,0.5,0.5);
				}
					
				cairo_move_to(cr, x+(w/4), y+(h/3));
				cairo_line_to(cr, x+(w/4), y+(h/1.5));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/1.5));
				cairo_line_to(cr, x+(w-(w/4)), y+(h/3));
				cairo_stroke(cr);
				break;
		}
	}
		

	// DRAW LABEL
	
	cairo_set_source_rgb(cr, 0,0,0);
	cairo_rectangle(cr, x, y, w,font_size);
	cairo_fill(cr);

	if (hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);

	}

	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (x + w / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, y+font_size);
	cairo_show_text(cr, label.c_str());



}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Selector::Left_Button_Press(int xm, int ym)
{

	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;

	values[current_value]++;
	if (values[current_value] > max) values[current_value]=0;
	Convert_Scaled_To_Value();

}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_Selector::Mouse_Over(int xm, int ym)
{
}




