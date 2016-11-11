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
  targetFrequency = 440.0;
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
    
    updateFrequency();
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
  }
}// end sine

void VAOscillator::fillBufferRisingSaw(AudioBuffer<float> &buffer)
{
  float* const data = buffer.getWritePointer(0);
  
//  if(buffer.getNumSamples() == 512)
//  {
//    ScopedPointer<AudioBuffer<float>> oversampled = new AudioBuffer<float>(1, 8 * buffer.getNumSamples());
//  
//    float* const oversampledData = oversampled->getWritePointer(0);
//  
//    for(int sampleIndex = 0; sampleIndex < 4 * buffer.getNumSamples(); sampleIndex++)
//    {
//      oversampledData[sampleIndex] = (2 * currentPhase)/(2 * double_Pi) - 1;
//    
//      updatePhaseInc();
//    }
//  
//    transformer->performRealOnlyForwardTransform(oversampledData);
//  
//    // Brickwall filter the transformed buffer
//  
//    for(int sampleIndex = buffer.getNumSamples() + 2; sampleIndex < 4 * buffer.getNumSamples(); sampleIndex++)
//    {
//      oversampledData[sampleIndex] = 0;
//      
//      oversampledData[4 * buffer.getNumSamples() + sampleIndex] = 0;
//    }
//  
//    transformer->performRealOnlyInverseTransform(oversampledData);
//  
//    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
//    {
//      data[sampleIndex] = oversampledData[4 * sampleIndex];
//    }
//  }
//  else
//  {
//    buffer.clear();
//  }
  
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    
    data[sampleIndex] = (2 * currentPhase)/(2 * double_Pi) - 1;
    
    if(blepOn == 1)
    {
      
      //data[sampleIndex] += getPolyBlep(currentPhase, phaseInc);
      
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
    
    updateFrequency();
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
   
  }
  
}// end rising Saw

void VAOscillator::fillBufferFallingSaw(AudioBuffer<float> &buffer)
{
  float* const data = buffer.getWritePointer(0);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {

    data[sampleIndex] = -(2 * currentPhase)/(2 * double_Pi) + 1;
  
    //mix In the BLEP
    if(blepOn == 1)
    {
      //data[sampleIndex] += getPolyBlep(currentPhase, phaseInc);
      
      if(currentPhase <= twoPiHalfPulseLength * currentFrequency)
      {
        //start at mid point and then percentage
        data[sampleIndex] += blepData[2561 - (int)( (currentPhase/(2 * twoPiHalfPulseLength * currentFrequency)) * 2561 )];
      }
      else if(currentPhase >= 2 * double_Pi - twoPiHalfPulseLength * currentFrequency)
      {
        data[sampleIndex] += blepData[2561 - (int)( (currentPhase - (2 * double_Pi - twoPiHalfPulseLength * currentFrequency) )/(2 * twoPiHalfPulseLength * currentFrequency) * 2561 ) + 1280];
      }
    }
    
    updateFrequency();
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
    
  }
}// end falling Saw

void VAOscillator::fillBufferSquarePulse(AudioBuffer<float> &buffer)
{
  float* const output = buffer.getWritePointer(0);
  
  AudioBuffer<float>* fallingSaw = new AudioBuffer<float>(1, buffer.getNumSamples());
  
  fillBufferRisingSaw(buffer);
  
  fillBufferFallingSaw(*fallingSaw);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    output[sampleIndex] += fallingSaw->getSample(0,((int)((currentSampleRate/currentFrequency)/2) + sampleIndex) % buffer.getNumSamples());
  }

  updateFrequency();
  currentPhase += phaseInc;
  
  if(currentPhase > 2 * double_Pi)
  {
    currentPhase -= 2 * double_Pi;
  }
  
}// end square pulse

void VAOscillator::fillBufferTriangle(AudioBuffer<float> &buffer)
{
  
  
}


  //==============================================================================
  // getters and setters

double VAOscillator::getSampleRate()
{
  return currentSampleRate;
}

void VAOscillator::setSampleRate(double newSampleRate)
{
  currentSampleRate = newSampleRate;
  fourFoldSampleRate = 4 * newSampleRate;
  
  twoPiHalfPulseLength = double_Pi * 6 * double_Pi/60000.0f;
  
  updatePhaseInc();
}

double VAOscillator::getFrequency()
{
  return currentFrequency;
}

void VAOscillator::setFrequency(double newFrequency)
{
  targetFrequency = newFrequency;
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

void VAOscillator::updateFrequency()
{
  //max step size == 1/500 * currentfrequency
  if(std::abs(currentFrequency - targetFrequency) < 0.001 * currentFrequency) // import change the std::abs to something crossplatform
  {
    currentFrequency = targetFrequency;
  }
  else if(currentFrequency < targetFrequency)
  {
    currentFrequency += 0.001 * currentFrequency;
  }
  else
  {
    currentFrequency -= 0.001 * currentFrequency;
  }
  
  updatePhaseInc();
}

void VAOscillator::updatePhaseInc()
{
  phaseInc = 2 * double_Pi * (currentFrequency/currentSampleRate);
}


