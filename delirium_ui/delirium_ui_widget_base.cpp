
#include "delirium_ui.hpp"
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------



Delirium_UI_Widget_Base::Delirium_UI_Widget_Base()
{



}



//-------------------------------------------------------------------------------------------

Delirium_UI_Widget_Base::~Delirium_UI_Widget_Base()
{
}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Base::Draw(cairo_t* cr)
{
	float widget_x_position = x_position * x_grid_size;
	float widget_y_position = y_position * y_grid_size;
	float widget_width = width * x_grid_size;
	float widget_height = height * y_grid_size;

	if (hover) {cairo_set_source_rgba(cr, 1,0,0,1); }
		else {cairo_set_source_rgba(cr, 0,0,0,1); }

	cairo_rectangle(cr, widget_x_position,widget_y_position,widget_width,widget_height);
	cairo_fill(cr);

}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Widget_Base::Left_Button_Press(int xm, int ym)
{

}

//-------------------------------------------------------------------------------------------
// USER PRESSED MIDDLE MOUSE BUTTON - RESET WIDGET TO DEFAULT VALUE

void Delirium_UI_Widget_Base::Middle_Button_Press()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_Base::Mouse_Over(int xm, int ym)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE SCROLL WHEEL

void Delirium_UI_Widget_Base::Mouse_Scroll(int mx,int my, float delta)
{

	double inc = increment;
	if (fine_increment) inc *= 0.1;

	if (type != deliriumUI_Knob)
	{
		if (min < max)
		{
			values[current_value] -= delta * inc;
			if (values[current_value] < min) values[current_value] = min;
			if (values[current_value] > max) values[current_value] = max;
		}
		else
		{
			values[current_value] += delta * inc;		
			if (values[current_value] > min) values[current_value] = min;
			if (values[current_value] < max) values[current_value] = max;
		}
	}
	else
	{
		if (min < max)
		{
			values[current_value] += (double)delta * (double)inc;
			if (values[current_value] <= min) values[current_value] = min;
			if (values[current_value] >= max) values[current_value] = max;
			// cout << normalised_values[current_value] << endl;

		}
		else
		{
			values[current_value] -= (double)delta * (double)inc;		
			if (values[current_value] >= min) values[current_value] = min;
			if (values[current_value] <= max) values[current_value] = max;
		}		
	}


	Convert_Scaled_To_Value();
	
	// cout << min << " - " << max << " - " << values[current_value] << " - " << normalised_values[current_value] << endl;
	
}

//----------------------------------------------------------------------------------------------------------------------------------------
// CONVERT NORMALISED 0~1 TO PARAMETERS MIN~MAX RANGE

void Delirium_UI_Widget_Base::Convert_Value_To_Scaled()
{
	float value = normalised_values[current_value];

	values[current_value] = (max-min)-(value * (max - min));

	if (min < 0) values[current_value] -= max;
	if (min > 0) values[current_value] += min;

}

//----------------------------------------------------------------------------------------------------------------------------------------
// CONVERT PARAMETERS MIN~MAX RANGE TO NORMALISED 0~1

void Delirium_UI_Widget_Base::Convert_Scaled_To_Value()

{
	normalised_values[current_value] = (values[current_value] - min) / (max - min);
	if (type == deliriumUI_Knob) { normalised_values[current_value] = 1-normalised_values[current_value]; }
	
	return;
	
}

//----------------------------------------------------------------------------------------------------------------------------------------





