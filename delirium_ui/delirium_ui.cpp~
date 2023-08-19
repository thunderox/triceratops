
#include "delirium_ui.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------

Delirium_UI_Surface* Delirium_UI_Init(int width, int height, int gridX, int gridY)
{

	Delirium_UI_Surface* GUI = new Delirium_UI_Surface;

	GUI->width = width;
	GUI->height = height;
	GUI->x_grid_size = width/gridX;
	GUI->y_grid_size = width/gridY;

	GUI->drag = 0;

	GUI->background_rgb[0] = 0.1;
	GUI->background_rgb[1] = 0;
	GUI->background_rgb[2] = 0;

	GUI->current_widget = -1;
	GUI->draw_flag = true;

	return GUI;
}

//----------------------------------------------------------------------------------------------------------------------------------------

int  Delirium_UI_Create_Widget(Delirium_UI_Surface* GUI, int type, int group, float grid_x_position, float grid_y_position, float grid_width, float grid_height, string label, int parameter_number)
{	
	bool widget_created = false;

	Delirium_UI_Widget_Base* new_widget;

	if (type == 0)
	{

	}

	if (type == deliriumUI_Button)
	{
		new_widget = new Delirium_UI_Widget_Button();
		widget_created = true;
	}

	if (type == deliriumUI_Fader)
	{
		new_widget = new Delirium_UI_Widget_Fader();
		widget_created = true;
	}

	if (type == deliriumUI_Knob)
	{
		new_widget = new Delirium_UI_Widget_Knob();
		widget_created = true;
	}

	if (type == deliriumUI_Switch)
	{
		new_widget = new Delirium_UI_Widget_Switch();
		widget_created = true;
	}

	if (type == deliriumUI_Label)
	{
		new_widget = new Delirium_UI_Widget_Label();
		widget_created = true;
	}
	if (type == deliriumUI_Logo)
	{
		new_widget = new Delirium_UI_Widget_Logo();
		widget_created = true;
	}

	if (type == deliriumUI_Panel)
	{
		new_widget = new Delirium_UI_Widget_Panel();
		widget_created = true;
	}

	if (type == deliriumUI_ADSR)
	{
		new_widget = new Delirium_UI_Widget_ADSR();
		widget_created = true;
	}

	if (type == deliriumUI_Selector)
	{
		new_widget = new Delirium_UI_Widget_Selector();
		widget_created = true;
	}


	if (widget_created)
	{
		new_widget->group = group;
		new_widget->parameter_number = parameter_number;
		new_widget->x_position = grid_x_position;
		new_widget->y_position = grid_y_position;
		new_widget->width = grid_width;
		new_widget->height = grid_height;
		new_widget->x_grid_size = GUI->x_grid_size;
		new_widget->y_grid_size = GUI->y_grid_size;
		new_widget->label = label;
		new_widget->type = type;
		new_widget->min = 0;
		new_widget->max = 1;
		new_widget->default_values[0] = 0;
		new_widget->current_value = 0;
		new_widget->increment = 0.01;
		new_widget->integer = (type == deliriumUI_Selector);
		new_widget->hover = false;
		if (type == deliriumUI_Switch) new_widget->toggle_mode = true; else new_widget->toggle_mode = false;
		GUI->Widgets.push_back(new_widget);

	}

	return GUI->Widgets.size() - 1; // Return Number Of Widget Created
}

//----------------------------------------------------------------------------------------------------------------------------------------
// SET WIDGET VALUE

void Delirium_UI_Widget_Set_Value(Delirium_UI_Surface* GUI, int widget_number, float scaled_value)
{
	int current_value = GUI->Widgets[widget_number]->current_value;
	GUI->Widgets[widget_number]->values[current_value] = scaled_value;
	Delirium_UI_Convert_Range_To_Value(GUI, widget_number);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// SET WIDGET INCREMENT

void Delirium_UI_Widget_Set_Increment(Delirium_UI_Surface* GUI, int widget_number, float increment)
{
	GUI->Widgets[widget_number]->increment = increment;
}

//----------------------------------------------------------------------------------------------------------------------------------------
// GET WIDGET VALUE

float Delirium_UI_Widget_Get_Value(Delirium_UI_Surface* GUI)
{
	int current_widget = GUI->current_widget;
	if (current_widget < 0) return -1;
	int current_value = GUI->Widgets[GUI->current_widget]->current_value;

	return GUI->Widgets[GUI->current_widget]->values[current_value];
}

// SET WIDGET DEFAULT VALUE

void Delirium_UI_Widget_Set_Default_Value(Delirium_UI_Surface* GUI, int widget_number, float default_value)
{	
	int current_value = GUI->Widgets[widget_number]->current_value;
	if (current_value < 0) return;
	GUI->Widgets[widget_number]->default_values[current_value] = default_value;
}

//----------------------------------------------------------------------------------------------------------------------------------------
// GET WIDGET PARAMETER NUMBER

int Delirium_UI_Widget_Get_Parameter_Number(Delirium_UI_Surface* GUI)
{
	if (GUI->current_widget > -1)
		return GUI->Widgets[GUI->current_widget]->parameter_number;
			else return -1;
}


//----------------------------------------------------------------------------------------------------------------------------------------
// SET WIDGET MIN - MAX VALUES

void Delirium_UI_Widget_Set_Min_Max(Delirium_UI_Surface* GUI, int widget_number, float min, float max)
{
	GUI->Widgets[widget_number]->min = min;
	GUI->Widgets[widget_number]->max = max;
}


//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Display_All(Delirium_UI_Surface* GUI, cairo_t* cr)
{

	if (GUI->draw_flag)
	{
		// float r = GUI->background_rgb[0];
		// float g = GUI->background_rgb[1];
		// float b = GUI->background_rgb[2];


		GUI->draw_flag = false;

		cairo_pattern_t *linpat;
		linpat = cairo_pattern_create_linear (0,0,0,500);
		cairo_pattern_add_color_stop_rgb(linpat, 0, 0.2,0.2,0.2);
		cairo_pattern_add_color_stop_rgb(linpat, 1, 0.1,0.1,0.1);

		cairo_set_source(cr, linpat);
		cairo_paint(cr);
	
		cairo_pattern_destroy(linpat);

		for (uint x=0; x<GUI->Widgets.size(); x++)
		{
			GUI->Widgets[x]->Draw(cr);
		}
	
	}	
	else
	{
		int current_widget = GUI->current_widget;
		if (current_widget > -1)
		{
			if (GUI->Widgets[current_widget]->type != deliriumUI_Panel) GUI->Widgets[current_widget]->Draw(cr);
		}
	}

}

//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_MouseOver(Delirium_UI_Surface* GUI, cairo_t* cr, int mx,int my)
{

	if (GUI->drag == 0)
	{
		Delirium_UI_Mouse_Over(GUI,mx,my);

		int old_current_widget = GUI->current_widget;
		GUI->current_widget = -1;
		
		for (uint x=0; x<GUI->Widgets.size(); x++)
		{		
			Rectangle<int> r_widget;

			r_widget.setX((float)GUI->Widgets[x]->x_position * GUI->x_grid_size);
			r_widget.setY(GUI->Widgets[x]->y_position * GUI->y_grid_size);
			r_widget.setWidth(GUI->Widgets[x]->width * GUI->x_grid_size);
			r_widget.setHeight(GUI->Widgets[x]->height * GUI->y_grid_size);
			
			if (r_widget.contains(mx,my))
			{
 
				if (old_current_widget > -1)
				{
					GUI->Widgets[old_current_widget]->hover = false; // New current widget switch hover off previous
					if (GUI->Widgets[old_current_widget]->type != deliriumUI_Panel) GUI->Widgets[old_current_widget]->Draw(cr); // one and redraw it.
				}

				GUI->current_widget = x;

				GUI->Widgets[x]->hover = true;
				if (GUI->Widgets[x]->type != deliriumUI_Panel) GUI->Widgets[x]->Draw(cr);
			}
		}
	}

	if (GUI->current_widget > -1)
	{
		if (GUI->drag == 1 && !GUI->Widgets[GUI->current_widget]->toggle_mode)
		{	
			Delirium_UI_Left_Button_Press(GUI,cr,-1,my);
		}
	}	
}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED

void Delirium_UI_Left_Button_Press(Delirium_UI_Surface* GUI, cairo_t* cr, int xm, int ym)
{
	int current_widget = GUI->current_widget;
	if (xm > -1) GUI->drag = 1 - GUI->drag;
	
	if (current_widget > -1)
	{
		GUI->Widgets[current_widget]->pressed = 1 - GUI->Widgets[current_widget]->pressed;
		GUI->Widgets[current_widget]->Left_Button_Press(xm,ym);
		Delirium_UI_Convert_Value_To_Range(GUI,current_widget);

		if (GUI->drag==0)
		{
			GUI->draw_flag = true;
			GUI->Widgets[current_widget]->hover = false;
			GUI->current_widget = -1;
			GUI->Widgets[current_widget]->Draw(cr);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MIDDLE MOUSE BUTTON PRESSED - RESET PARAMETER TO DEFAULT

void Delirium_UI_Middle_Button_Press(Delirium_UI_Surface* GUI)
{
	int current_widget = GUI->current_widget;

	if (current_widget > -1)
	{
		GUI->Widgets[current_widget]->pressed = 1 - GUI->Widgets[current_widget]->pressed;

		if (GUI->Widgets[current_widget]->pressed)
		{
			GUI->Widgets[current_widget]->Middle_Button_Press();
			Delirium_UI_Convert_Range_To_Value(GUI, current_widget);
		}
	}
	
}

//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Mouse_Over(Delirium_UI_Surface* GUI, int xm, int ym)
{
	int current_widget = GUI->current_widget;
	
	if (current_widget > -1)
	{
		GUI->Widgets[current_widget]->Mouse_Over(xm,ym);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
// CONVERT VALUE 0..1 TO MIN MAX RANGE
void Delirium_UI_Convert_Value_To_Range(Delirium_UI_Surface* GUI, int widget_number)
{
	if (widget_number < 0) return;
	int current_value = GUI->Widgets[widget_number]->current_value;
	float value = GUI->Widgets[widget_number]->normalised_values[current_value];
	float scaled_value;
	float min = GUI->Widgets[widget_number]->min;
	float max = GUI->Widgets[widget_number]->max;

	scaled_value = (value * (max - min));

	if (GUI->Widgets[widget_number]->type == deliriumUI_Knob) { scaled_value = (max-min)-(value * (max - min)); }

	if (min < 0) scaled_value -= max;
	if (min > 0) scaled_value += min;

	GUI->Widgets[widget_number]->values[current_value] = scaled_value;
}

//----------------------------------------------------------------------------------------------------------------------------------------
// CONVERT MIN MAX RANGE TO VALUE 0..1
void Delirium_UI_Convert_Range_To_Value(Delirium_UI_Surface* GUI, int widget_number)
{
	if (widget_number < 0) return;
	int current_value = GUI->Widgets[widget_number]->current_value;
	float scaled_value = GUI->Widgets[widget_number]->values[current_value];
	float min = GUI->Widgets[widget_number]->min;
	float max = GUI->Widgets[widget_number]->max;
	int type = GUI->Widgets[widget_number]->type;

	GUI->Widgets[widget_number]->normalised_values[current_value] = (scaled_value - min) / (max - min);

	if (type == deliriumUI_Knob)
	{
		GUI->Widgets[widget_number]->normalised_values[current_value] = 1 - GUI->Widgets[widget_number]->normalised_values[current_value];
	}
	return;

}

//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Set_Integer(Delirium_UI_Surface* GUI, int widget_number, bool _integer)
{
	if (widget_number < 0) return;
	GUI->Widgets[widget_number]->integer = _integer;
}


//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Cleanup(Delirium_UI_Surface* GUI)
{
	for (uint x=0; x<GUI->Widgets.size(); x++)
	{
		delete(GUI->Widgets[x]);
	}
	delete GUI;
}



