
#include <cmath>
#include <math.h>
#include <algorithm>
#include <iostream>

using namespace std;

#define ranf() ((float) rand() / (float) RAND_MAX)

double fast_pow(double, double);
double fast_atan(double);
float p4fast(float);
float fastishP2F (float);

//-----------------------------------------------

class noise
{
	public:

	float buffer[8192];
	int position,endpos,speed;

	noise();
	virtual ~noise();
	float tick();

};

//-----------------------------------------------

class inertia
{
	public:
	
	inertia();
	virtual ~inertia();
	void set_speed(double);
	double slide(double);	
	double value;

	private:

	double speed;


};

//-----------------------------------------------



//-----------------------------------------------
