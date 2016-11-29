/*
  ==============================================================================

    IIRFilter.cpp
    Created: 19 Oct 2016 6:52:26pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "GenericIIRFilter.h"

void GenericIIRFilter::firstOrderAllPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer)
{
  float* modBufferPointer = modBuffer.getWritePointer(0);
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    updateFirstOrderCoefficients(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    firstOrderState.stateXh0 = static_cast<float>(*pointer - (firstOrderState.c0) * firstOrderState.stateXh1);
    *pointer =static_cast<float>(firstOrderState.c0 * firstOrderState.stateXh0 + firstOrderState.stateXh1);
    
    firstOrderState.stateXh2 = firstOrderState.stateXh1;
    firstOrderState.stateXh1 = firstOrderState.stateXh0;
    
    modBufferPointer++;
  }
  
}


void GenericIIRFilter::secondOrderAllPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer)
{
  float* modBufferPointer = modBuffer.getWritePointer(0);
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    updateSecondOrderCoefficients(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    secondOrderState.stateXh0 = (*pointer) - (secondOrderState.c1 * (1 - secondOrderState.c0) * secondOrderState.stateXh1) + (secondOrderState.c0 * secondOrderState.stateXh2);
    
    
    if(secondOrderState.stateXh0 != secondOrderState.stateXh0)
    {
      resetSecondOrderState();
      //setQ(1);
      DBG("Error in states");
    }
    
    *pointer = (float)((-(secondOrderState.c0) * secondOrderState.stateXh0) + (secondOrderState.c1 * (1 - secondOrderState.c0) * secondOrderState.stateXh1) + secondOrderState.stateXh2);
    
    secondOrderState.stateXh2 = secondOrderState.stateXh1;
    secondOrderState.stateXh1 = secondOrderState.stateXh0;
    
    modBufferPointer++;
  }
}

void GenericIIRFilter::allpassForLowShelf(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer)
{
  
  float* modBufferPointer = modBuffer.getWritePointer(0);
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    updateCoefficientsLowShelf(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    firstOrderState.stateXh0 = *pointer - (firstOrderState.c0) * firstOrderState.stateXh1;
    *pointer = (float)(firstOrderState.c0 * firstOrderState.stateXh0 + firstOrderState.stateXh1);
    
    firstOrderState.stateXh2 = firstOrderState.stateXh1;
    firstOrderState.stateXh1 = firstOrderState.stateXh0;
    modBufferPointer++;
  }
}

void GenericIIRFilter::allpassForHighShelf(AudioBuffer<float> &buffer, AudioBuffer<float>& modBuffer)
{
  float* modBufferPointer  = modBuffer.getWritePointer(0);
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    updateCoefficientsHighShelf(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    firstOrderState.stateXh0 = *pointer - (firstOrderState.c0) * firstOrderState.stateXh1;
    *pointer = (float)(firstOrderState.c0 * firstOrderState.stateXh0 + firstOrderState.stateXh1);
    
    firstOrderState.stateXh2 = firstOrderState.stateXh1;
    firstOrderState.stateXh1 = firstOrderState.stateXh0;
    modBufferPointer++;
  }
}


void GenericIIRFilter::updateFirstOrderCoefficients(float frequency)
{
  firstOrderState.c0 = (tan(double_Pi * (frequency)* oneOverSampleRate) - 1)/(tan(double_Pi * (frequency) * oneOverSampleRate) + 1);
}


void GenericIIRFilter::updateSecondOrderCoefficients(float frequency)
{
  secondOrderState.c0 = (tan(double_Pi * (frequency/(qParameter*sampleRate))) - 1)/(tan(double_Pi * (frequency/(qParameter*sampleRate))) + 1);
  
  secondOrderState.c1 = -cos(2 * double_Pi * (frequency)* oneOverSampleRate);
  
}


void GenericIIRFilter::updateCanonicalCoefficientsLowpass(float frequency)
{
  double k = tan(double_Pi * frequency / sampleRate);
  float q = qParameter;
  double oneOverDenom = 1.0/(k*k*q + k + q);
  
  canonicalState.b0 = (k*k*q)*oneOverDenom;
  canonicalState.b1 = 2 * canonicalState.b0;
  canonicalState.b2 = canonicalState.b0;
  
  canonicalState.a1 = (2*q*(k*k - 1))*oneOverDenom;
  canonicalState.a2 = (k*k*q - k + q)*oneOverDenom;
}

void GenericIIRFilter::updateCanonicalCoefficientsHighpass(float frequency)
{
  double k = tan(double_Pi * frequency / sampleRate);
  float q = qParameter;
  double oneOverDenom = 1.0/(k*k*q + k + q);
  
  canonicalState.b0 = q*oneOverDenom;
  canonicalState.b1 = (-2)*canonicalState.b0;
  canonicalState.b2 = canonicalState.b0;

  canonicalState.a1 = (2*q*(k*k - 1))*oneOverDenom;
  canonicalState.a2 = (k*k*q - k + q)*oneOverDenom;
}

void GenericIIRFilter::updateCanonicalCoefficientsBandpass(float frequency)
{
  double k = tan(double_Pi * frequency / sampleRate);
  float q = qParameter;
  double oneOverDenom = 1.0/(k*k*q + k + q);
  
  canonicalState.b0 = k*oneOverDenom;
  canonicalState.b1 = 0;
  canonicalState.b2 = (-1) * canonicalState.b0;
  
  canonicalState.a1 = (2*q*(k*k - 1))*oneOverDenom;
  canonicalState.a2 = (k*k*q - k + q)*oneOverDenom;
}

void GenericIIRFilter::updateCanonicalCoefficientsBandstop(float frequency)
{
  double k = tan(double_Pi * frequency / sampleRate);
  float q = qParameter;
  double oneOverDenom = 1.0/(k*k*q + k + q);
  
  canonicalState.b0 = (q*(1 + k*k))*oneOverDenom;
  canonicalState.b1 = (2*q*(k*k - 1))*oneOverDenom;
  canonicalState.b2 = canonicalState.b0;
  
  canonicalState.a1 = (2*q*(k*k - 1))*oneOverDenom;
  canonicalState.a2 = (k*k*q - k + q)*oneOverDenom;
}

void GenericIIRFilter::updateCoefficientsLowShelf(float frequency)
{
  if(gainParameter >= 0)
  {
      firstOrderState.c0 = (tan(double_Pi * (frequency)* oneOverSampleRate) - 1)/(tan(double_Pi * (frequency) * oneOverSampleRate) + 1);
  }
  else
  {
      firstOrderState.c0 = (tan(double_Pi * (frequency)* oneOverSampleRate) - (h0 + 1))/(tan(double_Pi * (frequency) * oneOverSampleRate) + h0 + 1);
  }
}

void GenericIIRFilter::updateCoefficientsHighShelf(float frequency)
{
  if(gainParameter >= 0)
  {
    firstOrderState.c0 = (tan(double_Pi * (frequency)* oneOverSampleRate) - 1)/(tan(double_Pi * (frequency) * oneOverSampleRate) + 1);
  }
  else
  {
    firstOrderState.c0 = ((h0 + 1)*tan(double_Pi * (frequency)* oneOverSampleRate) - 1)/((h0 + 1)*tan(double_Pi * (frequency) * oneOverSampleRate) + 1);
  }
}

void GenericIIRFilter::updateCoefficientsPeak(float frequency)
{
  if (gainParameter >= 0)
  {
    double k = tan(double_Pi * frequency / sampleRate);
    float q = qParameter;
    double oneOverDenom = 1.0/(k*k*q + k + q);
    
    canonicalState.b0 = (q + (h0+1)*k + q*k*k)*oneOverDenom;
    canonicalState.b1 = (2*q*(k*k - 1))*oneOverDenom;
    canonicalState.b2 = (q - (h0+1)*k + q*k*k)*oneOverDenom;
    
    canonicalState.a1 = canonicalState.b1;
    canonicalState.a2 = (q - k + q*k*k)*oneOverDenom;
  }
  else
  {
    double k = tan(double_Pi * frequency / sampleRate);
    float q = qParameter;
    double oneOverDenom = 1.0/(k*k*q + k/(h0+1) + q);
    
    
    canonicalState.b0 = (q + k + q*k*k)*oneOverDenom;
    canonicalState.b1 = (2*q*(k*k - 1))*oneOverDenom;
    canonicalState.b2 = (q - k + q*k*k)*oneOverDenom;
    
    canonicalState.a2 = (q - (h0+1)*k + q*k*k)*oneOverDenom;
    canonicalState.a2 = (q - (h0+1)*k + q*k*k)*oneOverDenom;
  }
}

void GenericIIRFilter::firstOrderLowPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::firstOrderAllPass(*filtered, modBuffer);
  
  float* filterPointer = filtered->getWritePointer(0);
  
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer = 0.5f * (*pointer + *filterPointer);
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


void GenericIIRFilter::firstOrderHighPass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::firstOrderAllPass(*filtered, modBuffer);
  float* filterPointer = filtered->getWritePointer(0);
  
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer =  0.5f * (*pointer - *filterPointer);
    filterPointer++;
  }
}


void GenericIIRFilter::bandpass(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::secondOrderAllPass(*filtered, modBuffer);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    buffer.setSample(0, sampleIndex, 0.5f * (buffer.getSample(0, sampleIndex) - (filtered->getSample(0, sampleIndex))));
  }
}


void GenericIIRFilter::bandstop(AudioBuffer<float>& buffer, AudioBuffer<float>& modBuffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::secondOrderAllPass(*filtered, modBuffer);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    buffer.setSample(0, sampleIndex, 0.5f * (buffer.getSample(0, sampleIndex) + filtered->getSample(0, sampleIndex)));
  }
}


void GenericIIRFilter::secondOrderLowPass(AudioBuffer<float> &buffer, AudioBuffer<float>& modBuffer)
{
  float* modBufferPointer = modBuffer.getWritePointer(0);
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCanonicalCoefficientsLowpass(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
                              - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = static_cast<float>(canonicalState.b0 * canonicalState.stateXh0
      + canonicalState.b1 * canonicalState.stateXh1
      + canonicalState.b2 * canonicalState.stateXh2);
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    modBufferPointer++;
    
  }
}


void GenericIIRFilter::secondOrderHighPass(AudioBuffer<float> &buffer, AudioBuffer<float>& modBuffer)
{
  float* modBufferPointer = modBuffer.getWritePointer(0);
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCanonicalCoefficientsHighpass(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
                              - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = static_cast<float>(canonicalState.b0 * canonicalState.stateXh0
      + canonicalState.b1 * canonicalState.stateXh1
      + canonicalState.b2 * canonicalState.stateXh2);
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    modBufferPointer++;
  }
}


void GenericIIRFilter::canonicalBandPass(AudioBuffer<float> &buffer, AudioBuffer<float>& modBuffer)
{
  float* modBufferPointer = modBuffer.getWritePointer(0);  
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCanonicalCoefficientsBandpass(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
                              - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = static_cast<float>(canonicalState.b0 * canonicalState.stateXh0
      + canonicalState.b1 * canonicalState.stateXh1
      + canonicalState.b2 * canonicalState.stateXh2);
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    modBufferPointer++;
  }
}


void GenericIIRFilter::canonicalBandstop(AudioBuffer<float> &buffer, AudioBuffer<float>& modBuffer)
{
  float* modBufferPointer = modBuffer.getWritePointer(0);
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCanonicalCoefficientsBandstop(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
                              - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = static_cast<float>(canonicalState.b0 * canonicalState.stateXh0
      + canonicalState.b1 * canonicalState.stateXh1
      + canonicalState.b2 * canonicalState.stateXh2);
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    modBufferPointer++;
  }
}

void GenericIIRFilter::lowShelf(AudioBuffer<float> &buffer, AudioBuffer<float>& modBuffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::allpassForLowShelf(*filtered, modBuffer);
  
  float* filterPointer = filtered->getWritePointer(0);
  
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer = *pointer + 0.5f * h0* (*pointer + *filterPointer);
    filterPointer++;
  }
}

void GenericIIRFilter::highShelf(AudioBuffer<float> &buffer, AudioBuffer<float>& modBuffer)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  GenericIIRFilter::allpassForHighShelf(*filtered, modBuffer);
  
  float* filterPointer = filtered->getWritePointer(0);
  
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer = *pointer + 0.5f * h0* (*pointer - *filterPointer);
    filterPointer++;
  }
}

void GenericIIRFilter::peak(AudioBuffer<float> &buffer, AudioBuffer<float>& modBuffer)
{
  float* modBufferPointer = modBuffer.getWritePointer(0);
  for (float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++) {
    
    updateCoefficientsPeak(computeCurrentFrequency(pointer, buffer, modBufferPointer));
    
    canonicalState.stateXh0 = *pointer - canonicalState.a1 * canonicalState.stateXh1
    - canonicalState.a2 * canonicalState.stateXh2;
    *pointer = static_cast<float>(canonicalState.b0 * canonicalState.stateXh0
      + canonicalState.b1 * canonicalState.stateXh1
      + canonicalState.b2 * canonicalState.stateXh2);
    
    canonicalState.stateXh2 = canonicalState.stateXh1;
    canonicalState.stateXh1 = canonicalState.stateXh0;
    modBufferPointer++;
  }
}


float GenericIIRFilter::computeCurrentFrequency(float* /*pointer*/, AudioBuffer<float>& /*buffer*/, float* modPointer)
{
  
  float modulationAmount = 1;
    if (*modPointer > 0)
      modulationAmount = 2.0f * currentCutoffFrequency;
    else
      modulationAmount = 0.5f*currentCutoffFrequency;
  
  float outFrequency;
  
//  for (int i = 0; i < buffer.getNumSamples(); i = i+128) {
//    
//  DBG("cutoffFrequency =" << currentCutoffFrequency);
//  DBG("modBuffer =" << *modPointer);
//  
//  }
  
  if(std::abs(targetCutoffFrequency - currentCutoffFrequency) < stepsize*currentCutoffFrequency)
    
  {  
    outFrequency = targetCutoffFrequency + *modPointer * modulationAmount;
    if(outFrequency <= 0)
      return 0;
    else if(outFrequency > 20000)
      return 20000;
    else
      return outFrequency;
    
  }
  
  else if (currentCutoffFrequency < targetCutoffFrequency)
    
  {
    
    currentCutoffFrequency += stepsize*currentCutoffFrequency;
    outFrequency = currentCutoffFrequency + *modPointer * modulationAmount;
    
    if(outFrequency <= 0)
      return 0;
    else if(outFrequency > 20000)
      return 20000;
    else
      return outFrequency;
    
    
    
  }
  
  else if(currentCutoffFrequency > targetCutoffFrequency)
    
  {
    
    currentCutoffFrequency -= stepsize*currentCutoffFrequency;
    outFrequency = currentCutoffFrequency + *modPointer * modulationAmount;
    
    if(outFrequency <= 0)
      return 0;
    else if(outFrequency > 20000)
      return 20000;
    else
      return outFrequency;
    
  }
  
  outFrequency = targetCutoffFrequency +  *modPointer * modulationAmount;
  if(outFrequency <= 0)
    return 0;
  else if(outFrequency > 20000)
    return 20000;
  else
    return outFrequency;
}










