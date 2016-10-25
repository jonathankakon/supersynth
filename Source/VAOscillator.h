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
  VAOscillator(double sampleRate);
  
  void fillBufferSine(AudioBuffer<float>& buffer);
  void fillBufferRisingSaw(AudioBuffer<float>& buffer);
  void fillBufferFallingSaw(AudioBuffer<float>& buffer);
  void fillBufferSquarePulse(AudioBuffer<float>& buffer);
  void fillBufferTriangle(AudioBuffer<float>& buffer);
  
  double getSampleRate();
  void setSampleRate(double newSampleRate);
  
  double getFrequency();
  void setFrequency(double newFrequency);
  
private:
  
  // naive not bandlimited waveforms
  void fillBufferNonLimitedRisingSaw(AudioBuffer<float>& buffer);
  void fillBufferNonLimitedFallingSaw(AudioBuffer<float>& buffer);
  void fillBufferNonLimitedSquare(AudioBuffer<float>& buffer);
  void fillBufferNonLimitedTriangle(AudioBuffer<float>& buffer);
  
  //Bandlimit the naive waveforms
  //TODO: I will remove these functions and do the bandlimiting directly when synthesizing the naive waves, because i have to use the currentPhase a lot to mix in the correct values
  void mixInBlitResidualRisingSaw(AudioBuffer<float>& nonLimitedBuffer);
  void mixInBlitResidualFallingSaw(AudioBuffer<float>& nonLimitedBuffer);
  void mixInBlitResidualSquare(AudioBuffer<float>& nonLimitedBuffer);
  void mixInBlitResidualTriangle(AudioBuffer<float>& nonLimitedBuffer);
  
  double currentSampleRate;
  double currentFrequency;
  double currentPhase;
  double phaseInc;
  double phaseOffset; // only for midi stuff. this determines the starting point of the wave when noteOn happens.
  
  void updatePhaseInc();
  
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VAOscillator)
};

#endif  // VAOSCILLATOR_H_INCLUDED
