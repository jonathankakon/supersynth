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


void GenericIIRFilter::updateCanonicalCoefficientsLowpass(float frequency)
{
  double k = tan(double_Pi * frequency / sampleRate);
  float q = qParameter;
  double denom = (k*k*q + k + q);
  
  canonicalState.b0 = (k*k*q)/denom;
  canonicalState.b1 = 2 * canonicalState.b0;
  canonicalState.b2 = canonicalState.b0;
  
  canonicalState.a1 = (2*q*(k*k - 1))/denom;
  canonicalState.a2 = (k*k*q - k + q)/denom;
}

void GenericIIRFilter::updateCanonicalCoefficientsHighpass(float frequency)
{
  double k = tan(double_Pi * frequency / sampleRate);
  float q = qParameter;
  double denom = (k*k*q + k + q);
  
  canonicalState.b0 = q/denom;
  canonicalState.b1 = (-2)*canonicalState.b0;
  canonicalState.b2 = canonicalState.b0;

  canonicalState.a1 = (2*q*(k*k - 1))/denom;
  canonicalState.a2 = (k*k*q - k + q)/denom;
}

void GenericIIRFilter::updateCanonicalCoefficientsBandpass(float frequency)
{
  double k = tan(double_Pi * frequency / sampleRate);
  float q = qParameter;
  double denom = (k*k*q + k + q);
  
  canonicalState.b0 = k/denom;
  canonicalState.b1 = 0;
  canonicalState.b2 = (-1) * canonicalState.b0;
  
  canonicalState.a1 = (2*q*(k*k - 1))/denom;
  canonicalState.a2 = (k*k*q - k + q)/denom;
}

void GenericIIRFilter::updateCanonicalCoefficientsBandstop(float frequency)
{
  double k = tan(double_Pi * frequency / sampleRate);
  float q = qParameter;
  double denom = (k*k*q + k + q);
  
  canonicalState.b0 = (q*(1 + k*k))/denom;
  canonicalState.b1 = (2*q*(k*k - 1))/denom;
  canonicalState.b2 = canonicalState.b0;
  
  canonicalState.a1 = (2*q*(k*k - 1))/denom;
  canonicalState.a2 = (k*k*q - k + q)/denom;
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


void GenericIIRFilter::secondOrderLowPass(AudioBuffer<float> &buffer)
{
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCanonicalCoefficientsLowpass(computeCurrentFrequency(pointer, buffer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
                              - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = canonicalState.b0 * canonicalState.stateXh0
               + canonicalState.b1 * canonicalState.stateXh1
               + canonicalState.b2 * canonicalState.stateXh2;
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    
  }
}


void GenericIIRFilter::secondOrderHighPass(AudioBuffer<float> &buffer)
{
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCanonicalCoefficientsHighpass(computeCurrentFrequency(pointer, buffer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
                              - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = canonicalState.b0 * canonicalState.stateXh0
               + canonicalState.b1 * canonicalState.stateXh1
               + canonicalState.b2 * canonicalState.stateXh2;
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    
  }
}


void GenericIIRFilter::canonicalBandPass(AudioBuffer<float> &buffer)
{
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCanonicalCoefficientsBandpass(computeCurrentFrequency(pointer, buffer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
                              - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = canonicalState.b0 * canonicalState.stateXh0
               + canonicalState.b1 * canonicalState.stateXh1
               + canonicalState.b2 * canonicalState.stateXh2;
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    
  }
}


void GenericIIRFilter::canonicalBandstop(AudioBuffer<float> &buffer)
{
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCanonicalCoefficientsBandstop(computeCurrentFrequency(pointer, buffer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
                              - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = canonicalState.b0 * canonicalState.stateXh0
               + canonicalState.b1 * canonicalState.stateXh1
               + canonicalState.b2 * canonicalState.stateXh2;
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    
  }
}


float GenericIIRFilter::computeCurrentFrequency(float* pointer, AudioBuffer<float>& buffer)
{
  return cutoffFrequency;
}










