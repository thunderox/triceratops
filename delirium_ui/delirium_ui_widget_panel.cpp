
#include "delirium_ui.hpp"

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Panel::Draw(cairo_t* cr) 
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;

	float r = 0.1;
	float g = 0;
	float b = 0;

	cairo_text_extents_t extents;
 
	cairo_set_font_size(cr, font_size);
	cairo_text_extents(cr, label.c_str(), &extents);
	float x_text_centred = (wX + wW / 2) - extents.width / 2;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, wX - 1, wY - (font_size * 1.5), wW + 2, wH + (font_size * 1.5) );
	cairo_fill(cr);

	cairo_set_source(cr, theme_background_grad);
	//cairo_paint(cr);

	// cairo_set_source_rgb(cr, 0.2,0,0);
	cairo_rectangle(cr, wX, wY + (font_size * 0.5), wW, wH);
	cairo_fill(cr);

	// -- text

	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_move_to(cr,x_text_centred, wY - 1);
	cairo_show_text(cr, label.c_str());
	
}


//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Panel::Left_Button_Press(int xm, int ym)
{
}



