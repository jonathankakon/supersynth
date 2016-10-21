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
  
  GenericIIRFilter(AudioParameterFloat *cutOffFrequency)
  {
    currentCutOffFrequency = cutOffFrequency;
    updateFirstOrderCoefficients();
    updateSecondOrderCoefficients();
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
  
private:
  
  AudioParameterFloat *currentCutOffFrequency;
  double sampleRate;

  
  struct filterState
  {
    double c0, c1, stateXh0, stateXh1, stateXh2;
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
  void updateFirstOrderCoefficients();
  void updateSecondOrderCoefficients();
  
  
//==============================================================================
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericIIRFilter)
  
};

#endif  // IIRFILTER_H_INCLUDED
