

#include "delirium_ui.hpp"


//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Button::Draw(cairo_t* cr) 
{

	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	cairo_set_source_rgba(cr, 0,0,0,1); 

	cairo_rectangle(cr, widget_x_position,widget_y_position,widget_width,widget_height);
	cairo_fill(cr);

}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Button::Left_Button_Press(int xm, int ym)
{
}
