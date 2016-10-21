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
  
  
  void fillBufferSaw(AudioBuffer<float> buffer);
  void fillBufferSine(AudioBuffer<float> buffer);
  void fillBufferSquarePulse(AudioBuffer<float> buffer);
  void fillBufferTriangle(AudioBuffer<float> buffer);
  
private:
  
  void fillBufferNonLimitedSaw(AudioBuffer<float> buffer);
  void fillBufferNonLimitedSquare(AudioBuffer<float> buffer);
  void fillBufferNonLimitedTriangle(AudioBuffer<float> buffer);
  
  void mixInBlitResidualSaw(AudioBuffer<float> nonLimitedBuffer);
  void mixInBlitResidualSquare(AudioBuffer<float> nonLimitedBuffer);
  void mixInBlitResidualTriangle(AudioBuffer<float> nonLimitedBuffer);
  
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VAOscillator)
};

#endif  // VAOSCILLATOR_H_INCLUDED
