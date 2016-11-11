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
#include "math.h"

class GenericIIRFilter
{
public:
  
  GenericIIRFilter(float frequency, float q, float gain)
  {
    cutoffFrequency = frequency;
    qParameter = q;
    gainParameter = gain;

    resetFirstOrderState();
    resetSecondOrderState();
    resetCanonicalState();
  }
  
  void firstOrderAllPass(AudioBuffer<float>& buffer); //If we want to support double precision in the FilterProcessor we must implement double versions of all these functions!!!!!!
  void secondOrderAllPass(AudioBuffer<float>& buffer);
  void allpassForHighShelf(AudioBuffer<float>& buffer);
  void allpassForLowShelf(AudioBuffer<float>& buffer);
  
  void firstOrderLowPass(AudioBuffer<float>& buffer);
  void firstOrderHighPass(AudioBuffer<float>& buffer);
  
  void bandpass(AudioBuffer<float>& buffer);
  void bandstop(AudioBuffer<float>& buffer);
  
  void secondOrderLowPass(AudioBuffer<float>& buffer);
  void secondOrderHighPass(AudioBuffer<float>& buffer);
  
  void canonicalBandPass(AudioBuffer<float>& buffer);
  void canonicalBandstop(AudioBuffer<float>& buffer);
  
  void highShelf(AudioBuffer<float>& buffer);
  void lowShelf(AudioBuffer<float>& buffer);
  
  void peak(AudioBuffer<float>& buffer);
  
  void updateSampleRate(double newSampleRate)
  {
    sampleRate = newSampleRate;
    oneOverSampleRate = 1.0/sampleRate;
  }
  
  void setCutoff(float frequency)
  {
    cutoffFrequency = frequency;
  }
  
  void setQ(float q)
  {
    qParameter = q;
  }
  
  void setGain(float gain)
  {
    gainParameter = gain;
    h0 = powf(10, gain / 20.0) - 1;
  }
  
private:
  
  float cutoffFrequency;
  float qParameter;
  float gainParameter;
  float h0;

  double sampleRate;
  double oneOverSampleRate;

  
  struct filterState
  {
    double c0, c1, stateXh0, stateXh1, stateXh2;  //c0 and c1 are the coefficients for the filter
  };
  
  struct canonicalFilterState
  {
    double b0, b1, b2, a1, a2, stateXh0, stateXh1, stateXh2;
  };
  
  filterState firstOrderState;
  filterState secondOrderState;
  canonicalFilterState canonicalState;
  
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
  
  void resetCanonicalState()
  {
    canonicalState.stateXh0 = 0;
    canonicalState.stateXh1 = 0;
    canonicalState.stateXh2 = 0;
  };
  
  void updateFirstOrderCoefficients(float frequency);
  void updateSecondOrderCoefficients(float frequency);
  void updateCanonicalCoefficientsLowpass(float frequency);
  void updateCanonicalCoefficientsHighpass(float frequency);
  void updateCanonicalCoefficientsBandpass(float frequency);
  void updateCanonicalCoefficientsBandstop(float frequency);
  void updateCoefficientsHighShelf(float frequency);
  void updateCoefficientsLowShelf(float frequency);
  void updateCoefficientsPeak(float frequency);
  
  float computeCurrentFrequency(float* pointer, AudioBuffer<float>& buffer);
  
  
//==============================================================================
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericIIRFilter)
  
};

#endif  // IIRFILTER_H_INCLUDED
