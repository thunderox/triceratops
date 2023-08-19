

#include "delirium_ui.hpp"


//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Tabbed_Navigator::Draw(cairo_t* cr) 
{

	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;

	cairo_set_source_rgb(cr, 0,0,0); 
	cairo_rectangle(cr, wX, wY, wW, wH);
	cairo_fill(cr);

	int xply = wW / tabs.size();
	
	cairo_set_source_rgb(cr, 0.0,0.15,0); 
	cairo_rectangle(cr, wX + (values[2]*xply), wY, xply, wH);
	cairo_fill(cr);	

	for (int x=0; x<tabs.size(); x++)
	{
		cairo_set_font_size(cr, font_size);
		cairo_text_extents_t extents;
		cairo_text_extents(cr, label.c_str(), &extents);
		float x_text_centred = (wX + wW / 2) - extents.width / 2;
		if (x == values[1]) cairo_set_source_rgb(cr, 1, 1, 1); 
			else cairo_set_source_rgb(cr, 0.6,0.6,0.6); 
		cairo_move_to(cr,wX + (xply*0.25)+(xply*x), wY + (font_size * 1.5));
		cairo_show_text(cr, tabs[x].c_str());
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Tabbed_Navigator::Left_Button_Press(int xm, int ym)
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;
	
	int xply = wW / tabs.size();
	
	values[2] = (int)(xm-wX)/xply;
	if (values[2] > tabs.size()-1) values[2] = tabs.size()-1;
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_Tabbed_Navigator::Mouse_Over(int xm, int ym)
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;
	
	int xply = wW / tabs.size();
	
	values[1] = (int)(xm-wX)/xply;
	
	if (values[1] > tabs.size()) values[1] = tabs.size();
	
	
}
