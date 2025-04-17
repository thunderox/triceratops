
#include <iostream>
#include <vector>
#include <cairo/cairo.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include "DistrhoUI.hpp"
#include <string>
#include <vector> 


#ifndef DELIRIUM_UI
#define DELIRIUM_UI

const float font_size = 12;
const int WAVE_MODE_OSC = 1;
const int WAVE_MODE_LFO = 2;

using namespace std;

typedef enum 
{
	deliriumUI_Button=1,
	deliriumUI_Knob=2,
	deliriumUI_MicroKnob=3,
	deliriumUI_Fader=4, 
	deliriumUI_Switch=5,
	deliriumUI_ADSR=6,
	deliriumUI_Wave_Selector=7,
	deliriumUI_LFO=8,
	deliriumUI_Selector=9,
	deliriumUI_Panel=10,
	deliriumUI_Label=11,
	deliriumUI_Logo=12,
	deliriumUI_Filter=13,
	deliriumUI_Fader_Route=14,
	deliriumUI_List=15,
	deliriumUI_Tabbed_Navigator=16
} deliriumUI_WidgetType;


struct group
{
	string name;
	vector <string> members;
	int visible_member;
};

class Delirium_UI_Widget_Base 
{

	public:

	Delirium_UI_Widget_Base();
	virtual ~Delirium_UI_Widget_Base();	
	virtual void Draw(cairo_t*);
	virtual void Left_Button_Press(int,int);
	virtual void Middle_Button_Press();
	virtual void Mouse_Over(int,int);
	virtual void Mouse_Scroll(int, int, float);
	virtual void Convert_Value_To_Scaled();
	virtual void Convert_Scaled_To_Value();

	float	x_position;
	float	y_position;
	float	width;
	float	height;
	float x_grid_size;
	float y_grid_size;
	float gui_width;
	float gui_height;

	bool pressed;
	bool hover;
	bool toggle_mode;
	bool integer;

	string label;
	int group;
	string group_name;
	string member_name;
	int parameter_number;
	int type;
	bool inverted;

	double normalised_values[4];
	double min;
	double max;
	double values[4];
	double default_values[4];
	int current_value;
	double increment;
	bool fine_increment;
	int route_number;
	int press_count;
	int wave_mode;
	vector <string> list_items;
	int list_position, list_scroll;
	bool active;
	vector <string> tabs;
	vector <string> tabs_group_name;
	vector <string> tabs_member_name;
	
	cairo_pattern_t *theme_background_grad;
	cairo_surface_t *surface_image;
	cairo_surface_t *logo_image;

	private:
 
};

class Delirium_UI_Widget_Button : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
};

class Delirium_UI_Widget_Fader : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Middle_Button_Press();
};

class Delirium_UI_Widget_Knob : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Middle_Button_Press();
};

class Delirium_UI_Widget_Switch : public Delirium_UI_Widget_Base
{
	public: void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Mouse_Scroll(int,int,float);
};

class Delirium_UI_Widget_Label : public Delirium_UI_Widget_Base
{
	public: void Draw(cairo_t*);
	void Left_Button_Press(int,int);
};

class Delirium_UI_Widget_Logo : public Delirium_UI_Widget_Base
{
	public: void Draw(cairo_t*);
	void Left_Button_Press(int,int);
};


class Delirium_UI_Widget_Panel : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
};

class Delirium_UI_Widget_ADSR : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Mouse_Over(int,int);
};

class Delirium_UI_Widget_Selector : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Mouse_Over(int,int);
};

class Delirium_UI_Widget_Filter : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	float filter_frequency;
	float filter_resonance;
};

class Delirium_UI_Widget_Fader_Route : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Middle_Button_Press();
	void Mouse_Scroll(int,int,float);
};

class Delirium_UI_Widget_List : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Mouse_Over(int,int);
	void Mouse_Scroll(int, int, float);

};

class Delirium_UI_Widget_Tabbed_Navigator : public Delirium_UI_Widget_Base
{
	public:
	void Draw(cairo_t*);
	void Left_Button_Press(int,int);
	void Mouse_Over(int,int);
};

struct Delirium_UI_Surface
{

	int width;
	int height;
	float x_grid_size;
	float y_grid_size;
	int current_widget;
	bool drag;
	bool draw_flag;
	int drawn_at_least_once; // workaround for hosts that do weird things with the window on initialisation, looking at you Renoise!

	float background_rgb[3];
	float foreground_rgb[3];

	int current_group;
	bool group_visible[32];

	vector <Delirium_UI_Widget_Base*> Widgets;
	int parameter_widget_number[512];
		
	cairo_pattern_t *theme_background_grad;
	cairo_surface_t* logo_surface;
		
	vector <group> groups;
			
	cairo_surface_t* all_widgets;

};

Delirium_UI_Surface* Delirium_UI_Init(int,int,int,int);

int  Delirium_UI_Create_Widget(Delirium_UI_Surface*, int, int, float, float, float, float, string, int);
void Delirium_UI_Widget_Set_Value(Delirium_UI_Surface*, int, float);
void Delirium_UI_Widget_Set_Increment(Delirium_UI_Surface*, int, float);
void Delirium_UI_Widget_Set_Min_Max(Delirium_UI_Surface*, int, float, float);
float Delirium_UI_Widget_Get_Value(Delirium_UI_Surface*);
void Delirium_UI_Widget_Set_Default_Value(Delirium_UI_Surface*, int, float);
int Delirium_UI_Widget_Get_Current_Value(Delirium_UI_Surface*);
int Delirium_UI_Widget_Get_Parameter_Number(Delirium_UI_Surface*);
void Delirium_UI_Display_All(Delirium_UI_Surface*, cairo_t*);
void Delirium_UI_MouseOver(Delirium_UI_Surface*, cairo_t*, int,int);
void Delirium_UI_Left_Button_Press(Delirium_UI_Surface*, cairo_t*, int,int);
void Delirium_UI_Middle_Button_Press(Delirium_UI_Surface*);
void Delirium_UI_Mouse_Over(Delirium_UI_Surface*, int,int);
void Delirium_UI_Convert_Value_To_Range(Delirium_UI_Surface*, int);
void Delirium_UI_Convert_Range_To_Value(Delirium_UI_Surface*, int);
void Delirium_UI_Cleanup(Delirium_UI_Surface*);
void Delirium_UI_Widget_Set_Integer(Delirium_UI_Surface*, int, bool);
void Delirium_UI_Widget_Set_Route_Destination(Delirium_UI_Surface*, int, int);
int Delirium_UI_Widget_Get_Type(Delirium_UI_Surface*);
int Delirium_UI_Widget_Get_Route_Number(Delirium_UI_Surface*);
void Delirium_UI_Widget_Set_Selector_Type(Delirium_UI_Surface*, int, int);
void Delirium_UI_Widget_List_Add_Item(Delirium_UI_Surface*, int, string);

void Delirium_UI_Widget_Set_Group_And_Member(Delirium_UI_Surface*, int, string, string);

int Delirium_UI_Group_Create(Delirium_UI_Surface*, string);
void Delirium_UI_Group_Add_Member(Delirium_UI_Surface*, string, string);
void Delirium_UI_Group_Set_Visible_member(Delirium_UI_Surface*, string, string);
bool Delirium_UI_Group_Is_Member_Visible(Delirium_UI_Surface*, string, string);
void Delirium_UI_Group_Set_Active_Widgets(Delirium_UI_Surface*);

void Delirium_UI_Group_Add_Navigator_Tab(Delirium_UI_Surface*, int, string,string,string);

#endif





