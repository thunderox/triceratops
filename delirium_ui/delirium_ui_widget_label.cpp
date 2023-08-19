
#include "delirium_ui.hpp"


//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Label::Draw(cairo_t* cr) 
{
	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	if (hover) {cairo_set_source_rgba(cr, 0.25,0,0,0.25); }
		else {cairo_set_source_rgba(cr, 0,0,0,0.5); }

	cairo_rectangle(cr, widget_x_position,widget_y_position,widget_width,widget_height);
	cairo_fill(cr);

	cairo_set_source_rgba(cr, 0,0,0,1);
	cairo_rectangle(cr, widget_x_position,widget_y_position,widget_width,widget_height);
	cairo_stroke(cr);

	if (hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);;
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);;

	}

	// -- text

	cairo_text_extents_t extents;
	cairo_set_font_size(cr, 16);
	cairo_text_extents(cr, label.c_str(), &extents);
	float x_text_centred = (widget_x_position + widget_width / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, widget_y_position+18);
	cairo_show_text(cr, label.c_str());
}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Label::Left_Button_Press(int xm, int ym)
{
}




