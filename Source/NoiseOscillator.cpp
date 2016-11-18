/*
  ==============================================================================

    NoiseOscillator.cpp
    Created: 7 Nov 2016 5:47:30pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "NoiseOscillator.h"

void NoiseOscillator::fillBufferNoise(AudioBuffer<float> &buffer)
{
  float* const data = buffer.getWritePointer(0);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    data[sampleIndex] = (2 * rand.nextFloat()) - 1;
  }

}
