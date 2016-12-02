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
    targetCutoffFrequency = frequency;
    qParameter = q;
    gainParameter = gain;

    currentCutoffFrequency = targetCutoffFrequency;
    
    resetFirstOrderState();
    resetSecondOrderState();
    resetCanonicalState();
    
  }
  
  void firstOrderAllPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer); //If we want to support double precision in the FilterProcessor we must implement double versions of all these functions!!!!!!
  void secondOrderAllPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  void allpassForHighShelf(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  void allpassForLowShelf(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  
  void firstOrderLowPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  void firstOrderHighPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  
  void bandpass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  void bandstop(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  
  void secondOrderLowPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  void secondOrderHighPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  
  void canonicalBandPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  void canonicalBandstop(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  
  void highShelf(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  void lowShelf(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  
  void peak(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer);
  
  void updateSampleRate(double newSampleRate)
  {
    sampleRate = newSampleRate;
    oneOverSampleRate = 1.0/sampleRate;
  }
  
  void setCutoff(float frequency)
  {
    targetCutoffFrequency = frequency;
  }
  
  void setQ(float q)
  {
    qParameter = q;
  }
  
  void setGain(float gain)
  {
    gainParameter = gain;
    h0 = powf(10, gain / 20.0f) - 1.0f;
  }
  
private:
  
  float cutoffFrequency;
  float qParameter;
  float gainParameter;
  float h0;
  
  float targetCutoffFrequency;    //changed by the slider
  float currentCutoffFrequency;   //internal current cutoff frequency
  float stepsize = 1.0f/500.0f;     //current cutoff frequency changes towards targeted
                                  //frequency with stepsize*currentCutoffFrequency
  

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
  
  float computeCurrentFrequency(float* pointer, AudioBuffer<float>& buffer, float* modPointer);
  
  
//==============================================================================
JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericIIRFilter)
  
};

#endif  // IIRFILTER_H_INCLUDED
