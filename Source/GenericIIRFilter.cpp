/*
  ==============================================================================

    IIRFilter.cpp
    Created: 19 Oct 2016 6:52:26pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "GenericIIRFilter.h"

void GenericIIRFilter::firstOrderAllPass(AudioBuffer<float>& buffer)
{
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    updateFirstOrderCoefficients(computeCurrentFrequency(pointer, buffer));
    
    firstOrderState.stateXh0 = *pointer - (firstOrderState.c0) * firstOrderState.stateXh1;
    *pointer = firstOrderState.c0 * firstOrderState.stateXh0 + firstOrderState.stateXh1;
    
    firstOrderState.stateXh2 = firstOrderState.stateXh1;
    firstOrderState.stateXh1 = firstOrderState.stateXh0;
  }
  
}



void GenericIIRFilter::secondOrderAllPass(AudioBuffer<float>& buffer)
{
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    updateSecondOrderCoefficients(computeCurrentFrequency(pointer, buffer));
    
    secondOrderState.stateXh0 = (*pointer) - (secondOrderState.c1 * (1 - secondOrderState.c0) * secondOrderState.stateXh1) + (secondOrderState.c0 * secondOrderState.stateXh2);
    
    if(secondOrderState.stateXh0 != secondOrderState.stateXh0)
    {
      resetSecondOrderState();
      //setQ(1);
      DBG("Error in states");
    }
    
    *pointer = (-(secondOrderState.c0) * secondOrderState.stateXh0) + (secondOrderState.c1 * (1 - secondOrderState.c0) * secondOrderState.stateXh1) + secondOrderState.stateXh2;
    
    secondOrderState.stateXh2 = secondOrderState.stateXh1;
    secondOrderState.stateXh1 = secondOrderState.stateXh0;
  }
  
}


void GenericIIRFilter::updateFirstOrderCoefficients(float frequency)
{
  firstOrderState.c0 = (tan(double_Pi * (frequency)/sampleRate) - 1)/(tan(double_Pi * (frequency)/sampleRate) + 1);
}



void GenericIIRFilter::updateSecondOrderCoefficients(float frequency)
{
  secondOrderState.c0 = (tan(double_Pi * (frequency/(qParameter*sampleRate))) - 1)/(tan(double_Pi * (frequency/(qParameter*sampleRate))) + 1);
  
  secondOrderState.c1 = -cos(2 * double_Pi * (frequency)/sampleRate);
  
  if(secondOrderState.c0 == NAN)
    DBG("Error");
  
  if(secondOrderState.c1 == NAN)
    DBG("Error2");
}



void GenericIIRFilter::firstOrderLowPass(AudioBuffer<float>& buffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::firstOrderAllPass(*filtered);
  
  float* filterPointer = filtered->getWritePointer(0);
  
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer = 0.5 * (*pointer + *filterPointer);
    filterPointer++;
  }
  /*
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::firstOrderAllPass(*filtered);
  
  filterPointer = filtered->getWritePointer(0);
  
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer = 0.5 * (*pointer + *filterPointer);
    filterPointer++;
  }
  
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::firstOrderAllPass(*filtered);
  
  filterPointer = filtered->getWritePointer(0);
  
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer = 0.5 * (*pointer + *filterPointer);
    filterPointer++;
  }
*/
  
}



void GenericIIRFilter::firstOrderHighPass(AudioBuffer<float>& buffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::firstOrderAllPass(*filtered);
  float* filterPointer = filtered->getWritePointer(0);
  
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer =  0.5 * (*pointer - *filterPointer);
    filterPointer++;
  }
}



void GenericIIRFilter::bandpass(AudioBuffer<float>& buffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::secondOrderAllPass(*filtered);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    buffer.setSample(0, sampleIndex, 0.5 * (buffer.getSample(0, sampleIndex) - (filtered->getSample(0, sampleIndex))));
  }
}



void GenericIIRFilter::bandstop(AudioBuffer<float>& buffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::secondOrderAllPass(*filtered);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    buffer.setSample(0, sampleIndex, 0.5 * (buffer.getSample(0, sampleIndex) + filtered->getSample(0, sampleIndex)));
  }
}

float GenericIIRFilter::computeCurrentFrequency(float* pointer, AudioBuffer<float>& buffer)
{
  return cutoffFrequency;
}










