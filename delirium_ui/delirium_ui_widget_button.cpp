

#include "delirium_ui.hpp"


//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Button::Draw(cairo_t* cr) 
{

	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;

	// -- text

	if (values[1]==1)
	{
		cairo_set_source_rgb(cr, 0.3,0,0); 
		cairo_rectangle(cr, wX, wY, wW, wH);
		cairo_fill(cr);
	
	}
	else
	{
		cairo_set_source_rgb(cr, 0,0,0); 
		cairo_rectangle(cr, wX, wY, wW, wH);
		cairo_fill(cr);
	}
	

	cairo_set_source_rgb(cr, 0.7, 0.7, 0.7); 

	
	cairo_set_font_size(cr, font_size);
	cairo_text_extents_t extents;
	cairo_text_extents(cr, label.c_str(), &extents);
	float x_text_centred = (wX + wW / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, wY + (font_size * 1.5));
	cairo_show_text(cr, label.c_str());
}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Button::Left_Button_Press(int xm, int ym)
{
}
