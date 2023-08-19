
#include "fastmaths.hpp"


//------------------------------------------------------------------------------------------------
// APPROX POW

double fast_pow(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}

//------------------------------------------------------------------------------------------------
// APPROX ATAN

double fast_atan( double x )
{
    return (x / (1.0 + 0.28 * (x * x)));
}

//------------------------------------------------------------------------------------------------
// WHITE NOISE

#define ROTL( x, s) ((x)<<(s))^((x)>>((32)-(s)))

noise::noise()
{	

	for (int x=0; x<8192; ++x)
	{
		buffer[x] = ((float)(rand() % 65536)/32768)-1;
	}

	position = rand() % 8190;
	endpos = rand() % 8190;
	speed = rand() % 4 - rand() % 8;
	if (speed==0) {speed=1; }

}

float noise::tick()
{


	position+=speed;
	if (position < 0 || position > endpos)
	{ 
		endpos = rand() % 8190;
		position = rand() % 8190;
		speed = rand() % 4 - rand() % 8;
		if (speed==0) {speed=1; }
	}
	return buffer[position];


}

noise::~noise()
{
}

//-----------------------------------------------------------------------------------------------
// INERTIA

inertia::inertia()
{
	value = 440;
	speed = 0;
}

inertia::~inertia()
{

}

void inertia::set_speed(double new_speed)
{
	speed = new_speed;
}

double inertia::slide(double new_value)
{
	
	if (speed==50) { value = new_value; return value; }

	if (value == new_value) { return value; }

	if (value < new_value)
	{
		value+=speed*4;
		if (value > new_value) { value = new_value; }
	}

	if (value > new_value)
	{
		value-=speed*4;
		if (value < new_value) {value = new_value; }
	}

	return value;
}

//===============================================================

float p4fast(float in)
{
  long *lp,l;

  lp=(long *)(&in);
  l=*lp;

  l-=0x3F800000l; /* un-bias */
  l<<=2;          /* **4 */
  l+=0x3F800000l; /* bias */
  *lp=l;

  /* compiler will read this from memory since & operator had been used */
  return in;
}

//===============================================================

float fastishP2F (float pitch)
{
long convert;
float *p=(float *)&convert;
float fl,fr,warp,out;

pitch *=0.0833333; //pitch scaling. remove this line for pow(2,a)
fl = floor(pitch);
fr = pitch - fl;
float fr2 = fr*fr;
warp = fr*0.696 + fr2*0.225 + fr*fr2*0.079;  // chebychev approx
//warp = fr*0.65 + fr*fr*0.35; // chebychev approx

out = fl+warp;
out *= 8388608.0; //2^23;
out += 127.0 * 8388608.0; //2^23;

convert = out; //magic

return *p;
}




