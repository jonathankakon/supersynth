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
  
  void fillBufferSine(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer);
  void fillBufferRisingSaw(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer);
  void fillBufferFallingSaw(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer);
  void fillBufferSquarePulse(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer);
  void fillBufferTriangle(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer);
  
  
  // getters and setters
  
  double getSampleRate() const;
  void setSampleRate(double newSampleRate);
  
  double getFrequency() const;
  void setFrequency(double newFrequency);

private:
  
  double currentSampleRate;
  double fourFoldSampleRate;
  double currentFrequency;
  double currentPhase;
  double phaseInc;
  double phaseOffset; // only for midi stuff. this determines the starting point of the wave when noteOn happens.
  
  double twoPiHalfPulseLength;
  double phaseToIncludeBlep;
  
  void updatePhaseInc();
  
  float getBlep(double phase, double frequency) const;
  float getTriRes(double phase, double frequency) const;
  
  double phaseModAmp;
  ScopedPointer<Array<float>> phaseArray;
  
  //the post Filter is now in every VAoscillator but could eventually be moved to the one outputprocessor
  double postFilterState;
  void postFilter(AudioBuffer<float> buffer);
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VAOscillator)
};

#endif  // VAOSCILLATOR_H_INCLUDED
