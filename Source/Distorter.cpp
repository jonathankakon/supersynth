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
  filter = new GenericIIRFilter(18000, 0.72, 1);
}

Distorter::~Distorter()
{
  
}

void Distorter::processHardclip(AudioBuffer<float> &buffer)
{
  ScopedPointer<AudioBuffer<float>> upsampledBuffer = new AudioBuffer<float>(1, buffer.getNumSamples() * 8);
  ScopedPointer<AudioBuffer<float>> minusOneBuffer = new AudioBuffer<float>(1, buffer.getNumSamples() * 8);
  
  float* const data = buffer.getWritePointer(0);
  float* const upsampledData = upsampledBuffer->getWritePointer(0);
  float* const minusOneData = minusOneBuffer->getWritePointer(0);
  
  upsampledBuffer->clear();
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    upsampledData[sampleIndex * 8] = data[sampleIndex];
  }
  
  //fill the buffer with -1 so it doesnt modulate the cutoff frequency
  FloatVectorOperations::fill(minusOneData, -1, minusOneBuffer->getNumSamples());
  
  //do it three times for higher order
  filter->secondOrderLowPass(*upsampledBuffer, *minusOneBuffer);
//  filter->secondOrderLowPass(*upsampledBuffer, *minusOneBuffer);
//  filter->secondOrderLowPass(*upsampledBuffer, *minusOneBuffer);
  
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
  
  //LowPass filter at old Nyquist
  //do it three times for higher order
  filter->secondOrderLowPass(*upsampledBuffer, *minusOneBuffer);
  
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
  filter->updateSampleRate(newSampleRate);
  filter->setCutoff(newSampleRate/8 - 500); // cut a little before the nyquist
}