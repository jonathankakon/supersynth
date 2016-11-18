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
  
  void fillBufferSine(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer);
  void fillBufferRisingSaw(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer);
  void fillBufferFallingSaw(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer);
  void fillBufferSquarePulse(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer);
  void fillBufferTriangle(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer);
  
  
  // getters and setters
  
  double getSampleRate();
  void setSampleRate(double newSampleRate);
  
  double getFrequency();
  void setFrequency(double newFrequency);
  
  inline void setBlepOn(double on){blepOn = (int)on;}
  
private:
  
  double currentSampleRate;
  double fourFoldSampleRate;
  double currentFrequency;
  double targetFrequency;
  double currentPhase;
  double phaseInc;
  double phaseOffset; // only for midi stuff. this determines the starting point of the wave when noteOn happens.
  
  int blepOn = 1;
  
  double twoPiHalfPulseLength;
  double phaseToIncludeBlep;
  
  void updateFrequency();
  void updatePhaseInc();
  
  double getBlep(double phase, double frequency);
  double getTriRes(double phase, double frequency);
  
  double phaseModAmp;
  ScopedPointer<Array<float>> phaseArray;
  
  //the post Filter is now in every VAoscillator but could eventually be moved to the one outputprocessor
  double postFilterState;
  void postFilter(AudioBuffer<float> buffer);
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VAOscillator)
};

#endif  // VAOSCILLATOR_H_INCLUDED
