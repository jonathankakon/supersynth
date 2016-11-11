/*
  ==============================================================================

    VAOscillator.h
    Created: 19 Oct 2016 6:53:50pm
    Author:  Paul Lehmann
 
 A virtual analogue Oscillator

  ==============================================================================
*/

#ifndef VAOSCILLATOR_H_INCLUDED
#define VAOSCILLATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//if we want to support double precision in our Synth we need to implement double versions of all functions
class VAOscillator
{
public:
  
  VAOscillator();
  
  void fillBufferSine(AudioBuffer<float>& buffer);
  void fillBufferRisingSaw(AudioBuffer<float>& buffer);
  void fillBufferFallingSaw(AudioBuffer<float>& buffer);
  void fillBufferSquarePulse(AudioBuffer<float>& buffer);
  void fillBufferTriangle(AudioBuffer<float>& buffer);
  
  double getSampleRate();
  void setSampleRate(double newSampleRate);
  
  double getFrequency();
  void setFrequency(double newFrequency);
  
  inline void setBlepOn(double on){blepOn = (int)on;}
  
private:
  
  double currentSampleRate;
  double fourFoldSampleRate;
  double currentFrequency;
  double currentPhase;
  double phaseInc;
  double phaseOffset; // only for midi stuff. this determines the starting point of the wave when noteOn happens.
  double twoPiHalfPulseLength;
  double phaseToIncludeBlep;
  
  ScopedPointer<FFT> transformer;
  
  double getPolyBlep(double phase, double phaseIncr);
  void updatePhaseInc();
  
  int blepOn = 0;
  
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VAOscillator)
};

#endif  // VAOSCILLATOR_H_INCLUDED
