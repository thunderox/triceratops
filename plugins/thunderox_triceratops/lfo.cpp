//============================================================================
/**
	Implementation file for LFO.hpp
	
	@author		Remy Muller
	@date		20030822
*/
//============================================================================

#include <cmath>
#include "lfo.h"

const std::string LFO::waveNames[] = {"triangle", "sinus", "sawtooth", "square", "exponent"};

LFO::LFO(float samplerate)
  : samplerate(samplerate),
    phase(0),
    inc(0)
{
  setWaveform(LFO::sinus);   
  setRate(1.0f); //1Hz
}

static const float k1Div24lowerBits = 1.0f/(float)(1<<24);

float LFO::tick()
{

	
  // the 8 MSB are the index in the table in the range 0-255 
  int i = phase >> 24; 
  
  // and the 24 LSB are the fractionnal part
  float frac = (phase & 0x00FFFFFF) * k1Div24lowerBits;

  // increment the phase for the next tick
  phase += inc; // the phase overflow itself

  return table[i]*(1.0f-frac) + table[i+1]*frac; // linear interpolation
}

void LFO::setRate(float rate)
{
  /** the rate in Hz is converted to a phase increment with the following formula
      \f[ inc = (256*rate/samplerate) * 2^24 \f] */
  inc =  (unsigned int)((256.0f * rate / samplerate) * (float)(1<<24));
}

void LFO::setWaveform(waveform_t index)
{
  switch(index)
    {
    case sinus:
      {
	double pi = 4.0 * atan(1.0);

	int i;
	for(i=0;i<=256;i++)
	  table[i] = sin(2.0f*pi*(i/256.0f));

	break;
      }
    case triangle:
      {
	int i;
	for(i=0;i<64;i++)
	  {
	    table[i]     =        i / 64.0f;
	    table[i+64]  =   (64-i) / 64.0f;
	    table[i+128] =      - i / 64.0f;
	    table[i+192] = - (64-i) / 64.0f;
	  }
	table[256] = 0.0f;
	break;
      }
    case sawtooth:
      {
	int i;
	for(i=0;i<256;i++)
	  {
	    table[i] = 2.0f*(i/255.0f) - 1.0f;
	  }
	table[256] = -1.0f;
	break;
      }
    case square:
      {
	int i;
	for(i=0;i<128;i++)
	  {
	    table[i]     =  1.0f;
	    table[i+128] = -1.0f;
	  }
	table[256] = 1.0f;
	break;
      }
    case exponent:
      {
	/* symetric exponent similar to triangle */
	int i;
	float e = (float)exp(1.0f);
	for(i=0;i<128;i++)
	  {
	    table[i] = 2.0f * ((exp(i/128.0f) - 1.0f) / (e - 1.0f)) - 1.0f  ;
	    table[i+128] = 2.0f * ((exp((128-i)/128.0f) - 1.0f) / (e - 1.0f)) - 1.0f  ;
	  }
	table[256] = -1.0f;
	break;
      }

    default:
      {
	break;
      }
    } 
}
