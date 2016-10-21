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
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    firstOrderState.stateXh0 = buffer.getSample(0, sampleIndex) - (firstOrderState.c0) * firstOrderState.c0;
    buffer.setSample(0, sampleIndex, firstOrderState.c0 * firstOrderState.stateXh0 + firstOrderState.stateXh1);
    
    firstOrderState.stateXh2 = firstOrderState.stateXh1;
    firstOrderState.stateXh1 = firstOrderState.stateXh0;
  }
  
  buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples()); // mono => right channel 0 into channel 1
}

void GenericIIRFilter::secondOrderAllPass(AudioBuffer<float>& buffer)
{
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    secondOrderState.stateXh0 = (buffer.getSample(0, sampleIndex)) - (secondOrderState.c1 * (1 - secondOrderState.c0) * secondOrderState.stateXh1) + (secondOrderState.c0 * secondOrderState.stateXh2);
    
    buffer.setSample(0, sampleIndex, (-(secondOrderState.c0) * secondOrderState.stateXh0) + (secondOrderState.c1 * (1 - secondOrderState.c0) * secondOrderState.stateXh1) + secondOrderState.stateXh2 );
    
    secondOrderState.stateXh2 = secondOrderState.stateXh1;
    secondOrderState.stateXh1 = secondOrderState.stateXh0;
  }
  
  buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples()); // mono => right channel 0 into channel 1
}


void GenericIIRFilter::updateFirstOrderCoefficients()
{
  firstOrderState.c0 = (tan(double_Pi * (*currentCutOffFrequency)/sampleRate) - 1)/(tan(double_Pi * (*currentCutOffFrequency)/sampleRate) + 1);
  
}

void GenericIIRFilter::updateSecondOrderCoefficients()
{
  secondOrderState.c0 = (tan(double_Pi * (*currentCutOffFrequency)/sampleRate) - 1)/(tan(double_Pi * (*currentCutOffFrequency)/sampleRate) + 1);
  
  secondOrderState.c1 = -cos(2 * double_Pi * (*currentCutOffFrequency)/sampleRate);

}



void GenericIIRFilter::firstOrderLowPass(AudioBuffer<float>& buffer)
{
  AudioBuffer<float> *filtered = new AudioBuffer<float>(buffer);
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::firstOrderAllPass(*filtered);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    buffer.setSample(0, sampleIndex, 0.5 * (buffer.getSample(0, sampleIndex) + filtered->getSample(0, sampleIndex)));
  }
}


void GenericIIRFilter::firstOrderHighPass(AudioBuffer<float>& buffer)
{
  AudioBuffer<float> *filtered = new AudioBuffer<float>(buffer);
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::firstOrderAllPass(*filtered);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    buffer.setSample(0, sampleIndex, 0.5 * (buffer.getSample(0, sampleIndex) - filtered->getSample(0, sampleIndex)));
  }
}












