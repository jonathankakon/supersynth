/*
  ==============================================================================

    VAOscillator.cpp
    Created: 19 Oct 2016 6:53:50pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "VAOscillator.h"

#include "Constants.h"

VAOscillator::VAOscillator()
{
  currentSampleRate = 0;
  currentFrequency = 440.0;
  
  currentPhase = 0.0;
  
  phaseInc = 0.0;
  
  transformer = new FFT(11, false);
}

  //==============================================================================
  // waveforms:

void VAOscillator::fillBufferSine(AudioBuffer<float>& buffer)// add midi buffer and know channel with control Voltage
{
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {

    buffer.setSample(0, sampleIndex, sin(currentPhase) );
    
    //update the Phase
    phaseInc = (2 * double_Pi * (currentFrequency)/currentSampleRate);
    
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }

  }
}

void VAOscillator::fillBufferRisingSaw(AudioBuffer<float> &buffer)
{
  float* const data = buffer.getWritePointer(0);
  
  /*
  if(buffer.getNumSamples() == 512)
  {
    ScopedPointer<AudioBuffer<float>> oversampled = new AudioBuffer<float>(1, 8 * buffer.getNumSamples());
  
    float* const oversampledData = oversampled->getWritePointer(0);
  
    for(int sampleIndex = 0; sampleIndex < 4 * buffer.getNumSamples(); sampleIndex++)
    {
      oversampledData[sampleIndex] = (2 * currentPhase) - 1;
    
      currentPhase += phaseInc;
    
      if(currentPhase > 2 * double_Pi)
      {
        currentPhase -= 2 * double_Pi;
      }
    }
  
    transformer->performRealOnlyForwardTransform(oversampledData);
  
    // Brickwall filter the transformed buffer
  
    for(int sampleIndex = buffer.getNumSamples(); sampleIndex < 3 * buffer.getNumSamples(); sampleIndex++)
    {
      oversampledData[sampleIndex] = 0;
      
      oversampledData[5 * buffer.getNumSamples() + sampleIndex] = 0;
    }
  
    transformer->performRealOnlyInverseTransform(oversampledData);
  
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      data[sampleIndex] = oversampledData[4 * sampleIndex];
    }
  }
  else
  {
    buffer.clear();
  }
  */
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    
    data[sampleIndex] = (2 * currentPhase) -1;
    
    if(true)
    {
      
      data[sampleIndex] += getPolyBlep(currentPhase, phaseInc);
      
      
      if(currentPhase <= twoPiHalfPulseLength * currentFrequency)
      {
        //start at mid point and then percentage
        data[sampleIndex] += blepData[(int)( (currentPhase/(2 * twoPiHalfPulseLength * currentFrequency)) * 2561 )];
      }
      else if(currentPhase >= 2 * double_Pi - twoPiHalfPulseLength * currentFrequency)
      {
        data[sampleIndex] += blepData[(int)( (currentPhase - (2 * double_Pi - twoPiHalfPulseLength * currentFrequency) )/(2 * twoPiHalfPulseLength * currentFrequency) * 2561 ) + 1280];
      }
      
      
    }
    
      
    //update the Phase
    phaseInc = (2 * double_Pi * (currentFrequency)/currentSampleRate);
    
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
   
  }
   
  
}

void VAOscillator::fillBufferFallingSaw(AudioBuffer<float> &buffer)
{
}

void VAOscillator::fillBufferSquarePulse(AudioBuffer<float> &buffer)
{
}

void VAOscillator::fillBufferTriangle(AudioBuffer<float> &buffer)
{
}


  //==============================================================================

double VAOscillator::getSampleRate()
{
  return currentSampleRate;
}

void VAOscillator::setSampleRate(double newSampleRate)
{
  currentSampleRate = newSampleRate;
  fourFoldSampleRate = 4 * newSampleRate;
  
  twoPiHalfPulseLength = double_Pi * 6 * double_Pi/20000.0f;
  updatePhaseInc();
}

double VAOscillator::getFrequency()
{
  return currentFrequency;
}

void VAOscillator::setFrequency(double newFrequency)
{
  currentFrequency = newFrequency;
  updatePhaseInc();
}


  //==============================================================================
  // Privates:


double VAOscillator::getPolyBlep(double phase, double phaseIncr)
{
  if (phase < phaseIncr)
  {
    phase /= phaseIncr;
    return (phase + phase + (phase * phase) -1);
  }
  else if(phase > 2 * double_Pi - phaseIncr)
  {
    phase = (phase - 2 * double_Pi)/phaseIncr;
    return ((phase * phase) + phase + phase +1);
  }
  else
    return 0;
}

void VAOscillator::updatePhaseInc()
{
  phaseInc = 2 * double_Pi * (currentFrequency/currentSampleRate);
}


