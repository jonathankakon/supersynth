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

class IIRFilter
{
public:
  
  void firstOrderAllPass(AudioBuffer<float> buffer); //If we want to support double precision in the FilterProcessor we must implement double versions of all these functions!!!!!!
  void secondOrderAllPass(AudioBuffer<float> buffer);
  
  void firstOrderLowPass(AudioBuffer<float> buffer);
  void firstOrderHighPass(AudioBuffer<float> buffer);
  
  void bandpass(AudioBuffer<float> buffer);
  void bandstop(AudioBuffer<float> buffer);
  
  void highShelf(AudioBuffer<float> buffer);
  void lowShelf(AudioBuffer<float> buffer);
  
private:
  
  double coefficientA0, coefficientA1, coefficientB0, coefficientB1, coefficientB2; //filter coefficients
  
  double stateXh0, stateXh1, stateXh2; //filter state
  
//==============================================================================
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilter)
  
};

#endif  // IIRFILTER_H_INCLUDED
