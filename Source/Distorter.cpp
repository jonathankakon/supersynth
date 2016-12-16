/*
  ==============================================================================

    Distorter.cpp
    Created: 28 Nov 2016 8:45:22am
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "Distorter.h"
#include "Constants.h"

Distorter::Distorter(int bufferSize): oversamplingFactor(4)
{
  for(int i  = 0; i < 4; i++)
  {
    IIRFilter* filter = new IIRFilter();
    filter->setCoefficients(IIRCoefficients::makeLowPass(48000, 48000.0f/16.0f));
    preFilterArray.add(filter);
  }
  
  for(int i  = 0; i < 4; i++)
  {
    IIRFilter* filter = new IIRFilter();
    filter->setCoefficients(IIRCoefficients::makeLowPass(48000, 48000.0f/16.0f));
    postFilterArray.add(filter);
  }
}

Distorter::~Distorter()
{
  
}

void Distorter::processHardclip(AudioBuffer<float> &buffer)
{
  float* const data = buffer.getWritePointer(0);
  float* const upsampledData = upsampledBuffer.getWritePointer(0);
  
  upsampledBuffer.clear();
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    upsampledData[sampleIndex * oversamplingFactor] = data[sampleIndex];
  }
  
  for(int i = 0; i < preFilterArray.size(); i++)
  {
    preFilterArray[i]->processSamples(upsampledData, upsampledBuffer.getNumSamples());
  }
  
  upsampledBuffer.applyGain(2 * oversamplingFactor + 2); // * 2 because the filters dont seem to be normalized
  
  for(int sampleIndex = 0; sampleIndex < upsampledBuffer.getNumSamples(); sampleIndex++)
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
  
  for(int i = 0; i < postFilterArray.size(); i++)
  {
    postFilterArray[i]->processSamples(upsampledData, upsampledBuffer.getNumSamples());
  }
  
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    data[sampleIndex] = upsampledData[sampleIndex * oversamplingFactor];
  }
  
}

void Distorter::processTanhAprx(AudioBuffer<float>& buffer)
{
  
  float* const data = buffer.getWritePointer(0);
  float* const upsampledData = upsampledBuffer.getWritePointer(0);
  
  upsampledBuffer.clear();
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    upsampledData[sampleIndex * oversamplingFactor] = data[sampleIndex];
  }
  
  for(int i = 0; i < preFilterArray.size(); i++)
  {
    preFilterArray[i]->processSamples(upsampledData, upsampledBuffer.getNumSamples());
  }
  
  upsampledBuffer.applyGain(2 * oversamplingFactor + 2); // * 2 because the filters dont seem to be normalized
  
  for(int sampleIndex = 0; sampleIndex < upsampledBuffer.getNumSamples(); sampleIndex++)
  {
    float x = upsampledData[sampleIndex];
    
    upsampledData[sampleIndex] = x/powf(1 + powf(std::abs(x), 2.5), 0.4);
  }
  
  for(int i = 0; i < postFilterArray.size(); i++)
  {
    postFilterArray[i]->processSamples(upsampledData, upsampledBuffer.getNumSamples());
  }
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    data[sampleIndex] = upsampledData[sampleIndex * oversamplingFactor];
  }
  
}

void Distorter::setSampleRate(double newSampleRate)
{
  currentSampleRate = newSampleRate;
}

void Distorter::setBufferSize(int newSize)
{
  upsampledBuffer.setSize(1, newSize * oversamplingFactor);
}
