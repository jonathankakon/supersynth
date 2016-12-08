/*
  ==============================================================================

    Distorter.cpp
    Created: 28 Nov 2016 8:45:22am
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "Distorter.h"
#include "Constants.h"

Distorter::Distorter()
{
  preFilter = new FIRFilter(lowpass_2600hz, 267, 0, 0);
  postFilter = new FIRFilter(lowpass_2600hz, 267, 0, 0);
}

Distorter::~Distorter()
{
  
}

void Distorter::processHardclip(AudioBuffer<float> &buffer)
{
  ScopedPointer<AudioBuffer<float>> upsampledBuffer = new AudioBuffer<float>(1, buffer.getNumSamples() * 8);
  
  float* const data = buffer.getWritePointer(0);
  float* const upsampledData = upsampledBuffer->getWritePointer(0);
  
  upsampledBuffer->clear();
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    upsampledData[sampleIndex * 8] = data[sampleIndex];
  }
  
  preFilter->applyFIRFilter(*upsampledBuffer);
  
  upsampledBuffer->applyGain(8);
  
  for(int sampleIndex = 0; sampleIndex < upsampledBuffer->getNumSamples(); sampleIndex++)
  {
    if(std::abs(upsampledData[sampleIndex]) > 1)
    {
      if(upsampledData[sampleIndex] < 0)
      {
        upsampledData[sampleIndex] = -1;
      }
      else
      {
        upsampledData[sampleIndex] = 1;
      }
    }
  }
  
  postFilter->applyFIRFilter(*upsampledBuffer);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    data[sampleIndex] = upsampledData[sampleIndex * 8];
  }
  
  
  
}

void Distorter::processTanhAprx(AudioBuffer<float>& buffer)
{
  float* const data = buffer.getWritePointer(0);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    float x = data[sampleIndex];
    
    data[sampleIndex] = x/powf(1 + powf(std::abs(x), 2.5), 0.4);
  }
}

void Distorter::setSampleRate(double newSampleRate)
{
  currentSampleRate = newSampleRate;
}
