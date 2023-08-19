
#include "delirium_ui.hpp"

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Filter::Draw(cairo_t* cr) 
{
	float wX = x_position * x_grid_size;
	float wY = y_position * y_grid_size;
	float wW = width * x_grid_size;
	float wH = height * y_grid_size;

	cairo_set_source_rgb(cr, 0.15,0,0);
	cairo_rectangle(cr, wX, wY + (font_size * 0.5), wW, wH);
	cairo_fill(cr);
	
	int filter_type = values[0];
	
	cairo_text_extents_t extents;
	float x_text_centred;
	cairo_set_font_size(cr, font_size);
	
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, wX - 1, wY - (font_size * 1.5), wW + 2, (font_size * 2) );
	cairo_fill(cr);
	
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, wX - 1, wY + wH - (font_size * 1.5), wW + 2, (font_size * 1.5) );
	cairo_fill(cr);
	
	// -- draw filter waveform
	
	float filter_freq_x_offset = (filter_frequency * 40) - 20;
	
	switch (filter_type)
	{
		case 0:
			cairo_text_extents(cr, "OFF", &extents);
			x_text_centred = (wX + wW / 2) - extents.width / 2;

			cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
			cairo_move_to(cr,x_text_centred, wY + wH - 2);
			cairo_show_text(cr, "OFF");
			break;
			
		case 1:	
			cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
			cairo_move_to(cr, wX + 5, wY + (wH/3));
			cairo_line_to(cr, wX + 5 + filter_freq_x_offset + 20, wY + (wH/3));
			cairo_line_to(cr, wX + 5 + filter_freq_x_offset + 20, wY + (wH/3) - (filter_resonance * 24));
			cairo_line_to(cr, wX + 5 + filter_freq_x_offset + 30, wY + (wH/3));
			cairo_line_to(cr, wX + 5 + filter_freq_x_offset + 30, wY + (wH/3));
			cairo_line_to(cr, wX + (wW - 10), wY + (wH / 1.5));
			cairo_stroke(cr);
				
			cairo_text_extents(cr, "LOW PASS", &extents);
			x_text_centred = (wX + wW / 2) - extents.width / 2;

			cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
			cairo_move_to(cr,x_text_centred, wY + wH - 2);
			cairo_show_text(cr, "LOW PASS");
			break;
		
		case 2:
			cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
			cairo_move_to(cr, (wX + wW - 5), wY + (wH/3));
			cairo_line_to(cr, (wX + wW - 5) - (filter_freq_x_offset + 20), wY + (wH/3));
			cairo_line_to(cr, (wX + wW - 5) - (filter_freq_x_offset + 20), wY + (wH/3) - (filter_resonance * 24));
			cairo_line_to(cr, (wX + wW - 5) - (filter_freq_x_offset + 30), wY + (wH/3));
			cairo_line_to(cr, (wX + wW - 5) - (filter_freq_x_offset + 30), wY + (wH/3));
			cairo_line_to(cr, (wX + wW) - (wW - 10), wY + (wH / 1.5));
			cairo_stroke(cr);
			
			
			cairo_text_extents(cr, "HIGH PASS", &extents);
			x_text_centred = (wX + wW / 2) - extents.width / 2;

			cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
			cairo_move_to(cr,x_text_centred, wY + wH - 2);
			cairo_show_text(cr, "HIGH PASS");
			break;
			
			
		case 3:		
			
			cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
			cairo_move_to(cr, wX + 5, wY + (wH/1.5));

			cairo_curve_to(cr,
				wX + 5,wY + (wH / 1.5),
				wX + (wW / 2) + filter_freq_x_offset, wY + (wH / 3),
				(wX + (wW / 2)) + filter_freq_x_offset , wY + (wH / 4)  );	

			cairo_line_to(cr, wX + (wW / 1.9) + filter_freq_x_offset, wY + (wH / 4) - (filter_resonance * 16));
			cairo_line_to(cr, wX + (wW / 1.8)  + filter_freq_x_offset , wY + (wH / 4));

			cairo_curve_to(cr,
				wX + (wW - (wW/2.2)) + filter_freq_x_offset , wY + (wH / 4),
				wX + (wW - (wW/2.2)) + filter_freq_x_offset, wY + (wH / 3),
				(wX + wW) - 5, wY + (wH / 1.5) );
			
			cairo_stroke(cr);
			
			cairo_text_extents(cr, "BANDPASS", &extents);
			x_text_centred = (wX + wW / 2) - extents.width / 2;

			cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
			cairo_move_to(cr,x_text_centred, wY + wH - 2);
			cairo_show_text(cr, "BANDPASS");
			break;
			
		case 4:			
			cairo_text_extents(cr, "FORMANT", &extents);
			x_text_centred = (wX + wW / 2) - extents.width / 2;

			cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
			cairo_move_to(cr,x_text_centred, wY + wH - 2);
			cairo_show_text(cr, "FORMANT");
			break;
	}
	
	// -- text
	
	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (wX + wW / 2) - extents.width / 2;

	cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
	cairo_move_to(cr,x_text_centred, wY - 1);
	cairo_show_text(cr, label.c_str());
}


//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Filter::Left_Button_Press(int xm, int ym)
{
	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;
		
	toggle_mode = 1 - toggle_mode;

	if (toggle_mode==0) 
	{
		values[0]++;
		if (values[0]>4) values[0]=0;
		Convert_Scaled_To_Value();
	}
}



