/*
  ==============================================================================

    Distorter.cpp
    Created: 28 Nov 2016 8:45:22am
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "Distorter.h"

Distorter::Distorter()
{
  
}

Distorter::~Distorter()
{
  
}

void Distorter::processHardclip(AudioBuffer<float> &buffer)
{
  float* const data = buffer.getWritePointer(0);
  
  buffer.applyGain(gain);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    if(std::abs(data[sampleIndex]) > 1)
    {
      if(data[sampleIndex] < 0)
      {
        data[sampleIndex] = -1;
      }
      else
      {
        data[sampleIndex] = 1;
      }
    }
  }
  
  buffer.applyGain(invGain);
  
}

void Distorter::processTanhAprx(AudioBuffer<float>& buffer)
{
  float* const data = buffer.getWritePointer(0);
  
  buffer.applyGain(gain);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    float x = data[sampleIndex];
    
    data[sampleIndex] = x/powf(1 + powf(std::abs(x), 2.5), 0.4);
  }
  
  buffer.applyGain(invGain);
}
