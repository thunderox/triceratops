
#include "delirium_ui.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------

Delirium_UI_Surface* Delirium_UI_Init(int width, int height, int gridX, int gridY)
{

	Delirium_UI_Surface* GUI = new Delirium_UI_Surface;

	GUI->all_widgets = cairo_image_surface_create ( CAIRO_FORMAT_ARGB32, width, height );

	GUI->width = width;
	GUI->height = height;
	GUI->x_grid_size = width/gridX;
	GUI->y_grid_size = width/gridY;

	GUI->background_rgb[0] = 0.1;
	GUI->background_rgb[1] = 0;
	GUI->background_rgb[2] = 0;

	GUI->current_widget = -1;
	GUI->draw_flag = true;
	GUI->drawn_at_least_once = false;

	group new_group;
	new_group.name = "global";
	new_group.visible_member = 0;
	GUI->groups.push_back(new_group);
	
	GUI->theme_background_grad = cairo_pattern_create_linear (0,0,0,height);
	cairo_pattern_add_color_stop_rgb(GUI->theme_background_grad, 0, 0.17,0.0,0.0);
	cairo_pattern_add_color_stop_rgb(GUI->theme_background_grad, 0.5, 0.3,0.0,0.0);
	cairo_pattern_add_color_stop_rgb(GUI->theme_background_grad, 1.0, 0.2,0.0,0.0);
		
	GUI->logo_surface = cairo_image_surface_create_from_png ("/usr/lib/lv2/thunderox_triceratops.lv2/logo.png");
	

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
		new_widget->surface_image = GUI->logo_surface;
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
	
	if (type == deliriumUI_Filter)
	{
		new_widget = new Delirium_UI_Widget_Filter();
		widget_created = true;
	}
	
	if (type == deliriumUI_Fader_Route)
	{
		new_widget = new Delirium_UI_Widget_Fader_Route();	
		widget_created = true;
	}
	
	if (type == deliriumUI_List)
	{
		new_widget = new Delirium_UI_Widget_List();	
		new_widget->list_position = 0;
		new_widget->list_scroll = 0;
		widget_created = true;
	}
	
	if (type == deliriumUI_Tabbed_Navigator)
	{
		new_widget = new Delirium_UI_Widget_Tabbed_Navigator();
		widget_created = true;
	}


	if (widget_created)
	{
		new_widget->group = group;
		new_widget->parameter_number = parameter_number;
		if (parameter_number > -1)
		{
			GUI->parameter_widget_number[parameter_number] = GUI->Widgets.size();
		}
		
		new_widget->x_position = grid_x_position;
		new_widget->y_position = grid_y_position;
		new_widget->width = grid_width;
		new_widget->height = grid_height;
		new_widget->x_grid_size = GUI->x_grid_size;
		new_widget->y_grid_size = GUI->y_grid_size;
		new_widget->gui_width = GUI->width;
		new_widget->gui_height = GUI->height;
		new_widget->theme_background_grad = GUI->theme_background_grad;
		new_widget->label = label;
		new_widget->type = type;
		new_widget->min = 0;
		new_widget->max = 1;
		new_widget->default_values[0] = 0;
		new_widget->current_value = 0;
		new_widget->increment = 0.01;
		new_widget->integer = (type == deliriumUI_Selector);
		new_widget->hover = false;
		new_widget->route_number = 0;
		new_widget->press_count = 0;
		if (type == deliriumUI_Switch) new_widget->toggle_mode = true; else new_widget->toggle_mode = false;
		GUI->Widgets.push_back(new_widget);
	}

	return GUI->Widgets.size() - 1; // Return Number Of Widgets Created
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

int Delirium_UI_Widget_Get_Current_Value(Delirium_UI_Surface* GUI)
{	
	int current_widget = GUI->current_widget;
	if (current_widget < 0) return -1;
	int current_value = GUI->Widgets[current_widget]->current_value;
	if (current_value < 0) return -1;
	return current_value;
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
	{
		if ( GUI->Widgets[GUI->current_widget]->parameter_number > -1 )
		{
			return GUI->Widgets[GUI->current_widget]->parameter_number + GUI->Widgets[GUI->current_widget]->current_value;
		}
	}
	else
	{
		return -1;
	}
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
	
	cairo_t *cr_all_widgets = cairo_create (GUI->all_widgets);

	if (GUI->draw_flag)
	{
		GUI->draw_flag = true;
		
		cairo_set_source(cr_all_widgets, GUI->theme_background_grad);
		cairo_paint(cr_all_widgets);

		for (uint x=0; x<GUI->Widgets.size(); x++)
		{
			bool widget_visible = GUI->Widgets[x]->active;
			
			if (widget_visible) GUI->Widgets[x]->Draw(cr_all_widgets);
		}
		
		cairo_set_source_surface (cr, GUI->all_widgets, 0, 0);
		cairo_paint(cr);
	}	
	else
	{
		int current_widget = GUI->current_widget;


		if (current_widget > -1)
		{
			bool widget_visible = GUI->Widgets[current_widget]->active;


			if (widget_visible)
			{
				if (GUI->Widgets[current_widget]->type != deliriumUI_Panel) GUI->Widgets[current_widget]->Draw(cr_all_widgets);
			}
		}
		cairo_set_source_surface (cr, GUI->all_widgets, 0, 0);
		cairo_paint(cr);
	}
	
	cairo_destroy(cr_all_widgets);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_MouseOver(Delirium_UI_Surface* GUI, cairo_t* cr, int mx,int my)
{

	if (GUI->drag == 0)
	{
		Delirium_UI_Mouse_Over(GUI,mx,my);
		cairo_t *cr_all_widgets = cairo_create (GUI->all_widgets);

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
					bool widget_visible = GUI->Widgets[x]->active;
					
					if (widget_visible)
					{
						GUI->Widgets[old_current_widget]->hover = false; // New current widget switch hover off previous
						if (GUI->Widgets[old_current_widget]->type != deliriumUI_Panel)
							GUI->Widgets[old_current_widget]->Draw(cr_all_widgets); // one and redraw it.fmous
					}
				}

				bool widget_visible = GUI->Widgets[x]->active;
				
				if (widget_visible)
				{
					GUI->current_widget = x;
					GUI->Widgets[x]->hover = true;
					if (GUI->Widgets[x]->type != deliriumUI_Panel) GUI->Widgets[x]->Draw(cr_all_widgets);
				}
			}
		}
	}

	if (GUI->current_widget > -1)
	{
		if (GUI->drag == 1 && !GUI->Widgets[GUI->current_widget]->toggle_mode)
		{	
			bool widget_visible = GUI->Widgets[GUI->current_widget]->active;

			if (widget_visible && GUI->Widgets[GUI->current_widget]->type != deliriumUI_Selector
				&& GUI->Widgets[GUI->current_widget]->type != deliriumUI_Filter)
			{
				Delirium_UI_Left_Button_Press(GUI,cr,-1,my);
			}
		}
	}	
}

//----------------------------------------------------------------------------------------------------------------------------------------
// LEFT MOUSE BUTTON PRESSED
  
void Delirium_UI_Left_Button_Press(Delirium_UI_Surface* GUI, cairo_t* cr, int xm, int ym)
{
	int current_widget = GUI->current_widget;
	if (current_widget < 0 || GUI->Widgets[current_widget]->type == deliriumUI_Panel) return;
	
	bool widget_visible = GUI->Widgets[current_widget]->active;

	if (!widget_visible) return;
	
	if (GUI->Widgets[current_widget]->type == deliriumUI_Tabbed_Navigator) // This is a navigation widget see which group it applies to
	{
		int val = GUI->Widgets[current_widget]->values[1];
		string group_name = GUI->Widgets[current_widget]->tabs_group_name[val];
		string member_name = GUI->Widgets[current_widget]->tabs_member_name[val];
		Delirium_UI_Group_Set_Visible_member(GUI, group_name, member_name);
		GUI->Widgets[current_widget]->pressed = false;
		
	}
	

	if (xm > -1 && GUI->Widgets[current_widget]->type != deliriumUI_Tabbed_Navigator)
	{
		if (GUI->Widgets[current_widget]->type == deliriumUI_Fader_Route) cout << ym << endl; 
		GUI->drag = 1 - GUI->drag;
	}
	
	if (current_widget > -1)
	{
		GUI->Widgets[current_widget]->pressed = true;
		GUI->Widgets[current_widget]->Left_Button_Press(xm,ym);
		Delirium_UI_Convert_Value_To_Range(GUI,current_widget);
		
		int current_value = GUI->Widgets[current_widget]->current_value;
	
		if (GUI->Widgets[current_widget]->integer)
		{
			GUI->Widgets[current_widget]->values[current_value] = int(GUI->Widgets[current_widget]->values[current_value]);
			Delirium_UI_Convert_Range_To_Value(GUI, current_widget);
		}

		if (GUI->drag==0)
		{
			GUI->draw_flag = true;
			GUI->Widgets[current_widget]->hover = false;	
			GUI->Widgets[current_widget]->press_count = 0;
			GUI->current_widget = -1;
			Delirium_UI_Display_All(GUI, cr);

		}
	}
	
	int current_value = GUI->Widgets[current_widget]->current_value;
	// cout << GUI->Widgets[current_widget]->values[current_value] << endl;

}

//----------------------------------------------------------------------------------------------------------------------------------------
// MIDDLE MOUSE BUTTON PRESSED - RESET PARAMETER TO DEFAULT

void Delirium_UI_Middle_Button_Press(Delirium_UI_Surface* GUI)
{
	int current_widget = GUI->current_widget;	
	bool widget_visible = GUI->Widgets[current_widget]->active;
	if (!widget_visible) return;

	if (current_widget > -1)
	{
		GUI->Widgets[current_widget]->pressed = true;

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

	if (GUI->Widgets[widget_number]->type == deliriumUI_Knob) { scaled_value = (max-min) - scaled_value; }

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


	cairo_pattern_destroy(GUI->theme_background_grad);

	for (uint x=0; x<GUI->Widgets.size(); x++)
	{
		delete(GUI->Widgets[x]);
	}
	delete GUI;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Set_Route_Destination(Delirium_UI_Surface* GUI, int widget_number, int dest)
{
	GUI->Widgets[widget_number]->route_number = dest;
}


//----------------------------------------------------------------------------------------------------------------------------------------

int Delirium_UI_Widget_Get_Type(Delirium_UI_Surface* GUI)
{
	int current_widget = GUI->current_widget;
	
	if (current_widget > -1)
	{
		return GUI->Widgets[current_widget]->type;
	}
}	


//----------------------------------------------------------------------------------------------------------------------------------------

int Delirium_UI_Widget_Get_Route_Number(Delirium_UI_Surface* GUI)
{
	int current_widget = GUI->current_widget;
	
	if (current_widget > -1)
	{
		return GUI->Widgets[current_widget]->route_number;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Set_Selector_Type(Delirium_UI_Surface* GUI, int widget_number, int _type)
{
	GUI->Widgets[widget_number]->wave_mode = _type;
}

//----------------------------------------------------------------------------------------------------------------------------------------
void Delirium_UI_Widget_List_Add_Item(Delirium_UI_Surface* GUI, int widget_number, string new_item)
{
	if (GUI->Widgets[widget_number]->type != deliriumUI_List) return; // This widget is not a list ignore!
	GUI->Widgets[widget_number]->list_items.push_back(new_item);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Set_Group_And_Member(Delirium_UI_Surface* GUI, int widget_number, string group_name, string member_name)
{
	GUI->Widgets[widget_number]->group_name = group_name;
	GUI->Widgets[widget_number]->member_name = member_name;	
}

//----------------------------------------------------------------------------------------------------------------------------------------

int Delirium_UI_Group_Create(Delirium_UI_Surface* GUI,string name)
{
	group new_group;
	new_group.name = name;
	new_group.visible_member = 0;
	GUI->groups.push_back(new_group);
	return GUI->groups.size()-1;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Group_Add_Member(Delirium_UI_Surface* GUI, string group_name, string member_name)
{
	for (int x=0; x<GUI->groups.size(); x++)
	{
		if (GUI->groups[x].name == group_name)
		{
			GUI->groups[x].members.push_back(member_name);
		}
	}
};


//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Group_Set_Visible_member(Delirium_UI_Surface* GUI, string group_name, string member_name)
{

	for (int x=0; x<GUI->groups.size(); x++)
	{
		if (GUI->groups[x].name == group_name)
		{
			for (int y=0; y<GUI->groups[x].members.size(); y++)
			{
				if (GUI->groups[x].members[y] == member_name)
				{
					GUI->groups[x].visible_member = y;
				}
			}
		}
	}
	
	Delirium_UI_Group_Set_Active_Widgets(GUI);
};


//----------------------------------------------------------------------------------------------------------------------------------------

bool Delirium_UI_Group_Is_Member_Visible(Delirium_UI_Surface* GUI, string group_name, string member_name)
{
	bool visible = false;
	
		for (int x=0; x<GUI->groups.size(); x++)
	{
		if (GUI->groups[x].name == group_name)
		{
			for (int y=0; y<GUI->groups[x].members.size(); y++)
			{
				if (GUI->groups[x].members[y] == member_name)
				{
					visible = true;
				}
			}
		}
	}
	return visible;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void Delirium_UI_Group_Set_Active_Widgets(Delirium_UI_Surface* GUI)
{

	for (int wd=0; wd<GUI->Widgets.size(); wd++)
	{
		if ( GUI->Widgets[wd]->group_name == "global")
		{
			GUI->Widgets[wd]->active = true; // widgets set to global are always visible
		}
		else
		{
			GUI->Widgets[wd]->active = false; // Reset all widgets to inactive
		}
	}


	for (int gr=1; gr<GUI->groups.size(); gr++)
	{
		int visible_member = GUI->groups[gr].visible_member;

		if (GUI->groups[gr].members.size() > 0)
		{
			string member_name = GUI->groups[gr].members[visible_member];

			for (int wd=0; wd<GUI->Widgets.size(); wd++)
			{
				if (GUI->Widgets[wd]->group_name == GUI->groups[gr].name && 
					GUI->Widgets[wd]->member_name == GUI->groups[gr].members[visible_member] )
					{
						GUI->Widgets[wd]->active = true; // Set this widget to active it belongs to a group that is currently in use

					}
			}
		
		}
	}
};


//----------------------------------------------------------------------------------------------------------------------------------------------


void Delirium_UI_Group_Add_Navigator_Tab(Delirium_UI_Surface* GUI, int widget_number, string new_tab, string group_name, string member_name)
{
	if (widget_number < 0) return;
	if (GUI->Widgets[widget_number]->type != deliriumUI_Tabbed_Navigator) return;
	GUI->Widgets[widget_number]->tabs.push_back(new_tab);
	GUI->Widgets[widget_number]->tabs_group_name.push_back(group_name);
	GUI->Widgets[widget_number]->tabs_member_name.push_back(member_name);
}




