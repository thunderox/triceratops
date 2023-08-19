#ifndef LFO_H
#define LFO_H

#include <string>

//==============================================================================
/**
   This class implements a LUT-based LFO with various waveforms and linear 
   interpolation.
   
   It uses 32-bit fixed-point phase and increment, where the 8 MSB
   represent the integer part of the number and the 24 LSB the fractionnal part
   
   @author		Remy Muller
   @date		20030822
*/
//==============================================================================


class LFO
{
public:

  /** type enumeration used to select the different waveforms*/
  //typedef 
  typedef enum {triangle, sinus, sawtooth, square, exponent, kNumWave} waveform_t;
  
  /** phase type */
  typedef unsigned int phase_t;


  /**  @param samplerate the samplerate in Hz */
  LFO(float samplerate);
  virtual ~LFO() {}

  /** increments the phase and outputs the new LFO value.
      @return the new LFO value between [-1;+1] */ 
  float tick();

  /** change the current rate
      @param rate new rate in Hz */
  void setRate(const float rate);

  /** change the current samplerate
      @param samplerate new samplerate in Hz */
  void setSampleRate(const float samplerate_) {samplerate = (samplerate_>0.0) ? samplerate : 44100.0f;}

  /** select the desired waveform for the LFO
      @param index tag of the waveform
   */
  void setWaveform(waveform_t index);

  /** @return the waveform's name as a std::string*/
  const std::string getWaveformName(long index){return waveNames[index];}

  /** @return the waveform's name as a C-string (char *) */
  const char * get_C_WaveformName(long index){return waveNames[index].c_str();}

  /** phase and phase increment
      the 8 Most Significant Bits represent the integer part,
      the 24 Least Significant Bits represent the fractionnal part.
      that way we can automatically adress the table with the MSB
      between 0-255 (which overflow automatically) and use the LSB 
      to determine the fractionnal part for linear interpolation with a precision of 
      \f[ 2^-24 \f] */
  phase_t phase,inc;
  float samplerate;

private:
  /** names of the waveforms for display purpose*/
  static const std::string waveNames[kNumWave];


  

  
  /** table length is 256+1, with table[0] = table[256] 
      that way we can perform linear interpolation:
      \f[ val = (1-frac)*u[n] + frac*u[n+1] \f]
      even with n = 255.
      For n higher than 255, n is automatically  wrapped to 0-255*/
  float table[257]; 
};

#endif	// #ifndef LFO_H
