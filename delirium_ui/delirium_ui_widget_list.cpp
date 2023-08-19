

#include "delirium_ui.hpp"

const int list_display_number = 8;

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_List::Draw(cairo_t* cr) 
{

	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;


	cairo_set_source_rgb(cr, 0,0,0); 
	cairo_rectangle(cr, wX, wY, wW, font_size*2);
	cairo_fill(cr);
	
	cairo_set_source_rgb(cr, 0.1,0,0); 
	cairo_rectangle(cr, wX, wY+(font_size*2), wW, wH-font_size*2);
	cairo_fill(cr);
	
	cairo_set_source_rgb(cr, 0.075,0,0); 
	cairo_rectangle(cr, (wX+wW)-(font_size*2), wY+(font_size*2), font_size*2, wH-(font_size*2));
	cairo_fill(cr);

	cairo_set_source_rgb(cr, 0.7, 0.7, 0.7); 
	
	cairo_set_font_size(cr, font_size);
	cairo_text_extents_t extents;
	cairo_text_extents(cr, label.c_str(), &extents);
	float x_text_centred = (wX + wW / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, wY + (font_size * 1.5));
	cairo_show_text(cr, label.c_str());
	
	int number_of_items = list_items.size();
	
	for (int ln=0; ln<list_display_number; ln++)
	{
		if (ln+list_scroll < number_of_items)
		{

			if (ln == list_position && hover)
			{
				cairo_set_source_rgb(cr, 0.2,0,0); 
				cairo_rectangle(cr, wX, wY+(font_size*3) + (font_size*1.5*ln), wW, font_size*1.5);
				cairo_fill(cr);
				cairo_set_source_rgb(cr, 1,1,1); 
				cairo_set_font_size(cr, font_size*1.25);
				cairo_move_to(cr, wX+font_size, wY+(font_size*4) + (font_size*1.5*ln));
				cairo_show_text(cr, list_items[ln+list_scroll].c_str()) ;
			}
			else
			{				
				cairo_set_source_rgb(cr, 0.7, 0.7, 0.7); 
				cairo_set_font_size(cr, font_size*1.25);
				cairo_move_to(cr, wX+font_size, wY+(font_size*4) + (font_size*1.5*ln));
				cairo_show_text(cr, list_items[ln+list_scroll].c_str()) ;
			}
		}
	}
	
}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_List::Left_Button_Press(int xm, int ym)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE OVER

void Delirium_UI_Widget_List::Mouse_Over(int xm, int ym)
{

	int wX = x_position * x_grid_size;
	int wY = y_position * y_grid_size;
	int wW = width * x_grid_size;
	int wH = height * y_grid_size;
	
	int mouseX = xm-wX;
	int mouseY = (ym-wY) - (font_size);
	
	if (mouseY > font_size*1.5 && mouseX < wH)
	{
		list_position = (int)(mouseY / (font_size*1.5))-1;
		if (list_position + list_scroll > list_items.size()-1 )
		{
			list_position = (list_items.size()-1) - list_scroll;
		}
	}
}
	


//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_List::Mouse_Scroll(int mx,int my, float delta)
{
	list_scroll -= delta;
	
	if (list_scroll < 0) list_scroll = 0;
	if (list_scroll > list_items.size()-1) list_scroll = list_items.size()-1;
}

