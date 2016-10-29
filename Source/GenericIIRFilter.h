/*
  ==============================================================================

    IIRFilter.h
    Created: 19 Oct 2016 6:52:04pm
    Author:  Paul Lehmann, Kian Hunziker

 An IIR Filter
 
  ==============================================================================
*/

#ifndef IIRFILTER_H_INCLUDED
#define IIRFILTER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class GenericIIRFilter
{
public:
  
  GenericIIRFilter(float frequency, float q)
  {
    cutoffFrequency = frequency;
    qParameter = q;

    resetFirstOrderState();
    resetSecondOrderState();
  }
  
  void firstOrderAllPass(AudioBuffer<float>& buffer); //If we want to support double precision in the FilterProcessor we must implement double versions of all these functions!!!!!!
  void secondOrderAllPass(AudioBuffer<float>& buffer);
  
  void firstOrderLowPass(AudioBuffer<float>& buffer);
  void firstOrderHighPass(AudioBuffer<float>& buffer);
  
  void bandpass(AudioBuffer<float>& buffer);
  void bandstop(AudioBuffer<float>& buffer);
  
  void highShelf(AudioBuffer<float>& buffer);
  void lowShelf(AudioBuffer<float>& buffer);
  
  void updateSampleRate(double newSampleRate)
  {
    sampleRate = newSampleRate;
  }
  
  void setCutoff(float frequency)
  {
    cutoffFrequency = frequency;
  }
  
  void setQ(float q)
  {
    qParameter = q;
  }
  
private:
  
  float cutoffFrequency;
  float qParameter;

  double sampleRate;

  
  struct filterState
  {
    double c0, c1, stateXh0, stateXh1, stateXh2;  //c0 and c1 are the coefficients for the filter
  };
  
  filterState firstOrderState;
  filterState secondOrderState;
  
  void resetFirstOrderState()
  {
    firstOrderState.stateXh0 = 0;
    firstOrderState.stateXh1 = 0;
  };
  void resetSecondOrderState()
  {
    secondOrderState.stateXh0 = 0;
    secondOrderState.stateXh1 = 0;
    secondOrderState.stateXh2 = 0;
  };
  void updateFirstOrderCoefficients(float frequency);
  void updateSecondOrderCoefficients(float frequency);
  
  float computeCurrentFrequency(float* pointer, AudioBuffer<float>& buffer);
  
//==============================================================================
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericIIRFilter)
  
};

#endif  // IIRFILTER_H_INCLUDED
