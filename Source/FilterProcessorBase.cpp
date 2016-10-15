/*
  ==============================================================================

    FilterProcessorBase.cpp
    Created: 14 Oct 2016 3:57:59pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "FilterProcessorBase.h"



void FilterProcessorBase::resetAllpassState()
{
  currentAPState.xh0  = 0;
  currentAPState.xh1  = 0;
  currentAPState.xh2  = 0;
}

void FilterProcessorBase::applyFirstOrderAllpass(AudioBuffer<float>& buffer)
{
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    currentAPState.xh0 = buffer.getSample(0, sampleIndex) - (*firstOrderC0) * currentAPState.xh1;
    buffer.setSample(0, sampleIndex, *firstOrderC0 * currentAPState.xh0 + currentAPState.xh1);
    
    currentAPState.xh2 = currentAPState.xh1;
    currentAPState.xh1 = currentAPState.xh0;
  }
  
  buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples()); // mono => right channel 0 into channel 1
}

void FilterProcessorBase::applySecondOrderAllpass(AudioBuffer<float>& buffer)
{
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    currentAPState.xh0 = (buffer.getSample(0, sampleIndex)) - (*secondOrderC1 * (1 - *secondOrderC0) * currentAPState.xh1) + (*secondOrderC0 * currentAPState.xh2);
    
    buffer.setSample(0, sampleIndex, (-(*secondOrderC0) * currentAPState.xh0) + (*secondOrderC1 * (1 - *secondOrderC0) * currentAPState.xh1) + currentAPState.xh2 );
    
    currentAPState.xh2 = currentAPState.xh1;
    currentAPState.xh1 = currentAPState.xh0;
  }
  
  buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples()); // mono => right channel 0 into channel 1
}


void FilterProcessorBase::updateCoefficients()
{
  *firstOrderC0 = (tan(double_Pi * (*currentCutOffFrequency)/sampleRate) - 1)/(tan(double_Pi * (*currentCutOffFrequency)/sampleRate) + 1);
  
  *secondOrderC0 = (tan(double_Pi * (*currentBandwidth)/sampleRate) - 1)/(tan(double_Pi * (*currentBandwidth)/sampleRate) + 1);
  
  *secondOrderC1 = -cos(2 * double_Pi * (*currentCutOffFrequency)/sampleRate);
}
