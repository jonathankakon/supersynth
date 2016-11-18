/*
  ==============================================================================

    NoiseOscillator.h
    Created: 7 Nov 2016 5:47:30pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef NOISEOSCILLATOR_H_INCLUDED
#define NOISEOSCILLATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class NoiseOscillator
{
public:
  
  void fillBufferNoise(AudioBuffer<float>& buffer);
  
private:
  
  Random rand;
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoiseOscillator)
};



#endif  // NOISEOSCILLATOR_H_INCLUDED
