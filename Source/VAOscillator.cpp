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
  
  phaseModAmp = 2*double_Pi/10;
  
  postFilterState = 0.0;
}

  //==============================================================================
  // waveforms:

void VAOscillator::fillBufferSine(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer)// add midi buffer and know channel with control Voltage
{
  float* const data = buffer.getWritePointer(0);
  float const *phaseMod = phaseModBuffer.getReadPointer(0);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {

    data[sampleIndex] = sin(currentPhase + phaseModAmp * phaseMod[sampleIndex]);
    
    updateFrequency();
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
  }
}// end sine

void VAOscillator::fillBufferRisingSaw(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer)
{
  float* const data = buffer.getWritePointer(0);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    
    data[sampleIndex] = (2 * currentPhase)/(2 * double_Pi) - 1;
    
    if(blepOn == 1)
    {
      data[sampleIndex] += getBlep(currentPhase, currentFrequency);
    }
    
    updateFrequency();
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
  }
  
  //postFilter(buffer);
}// end rising Saw

void VAOscillator::fillBufferFallingSaw(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer)
{
  fillBufferRisingSaw(buffer, phaseModBuffer);
  
  float* const data = buffer.getWritePointer(0);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    data[sampleIndex] *= -1;
  }
  
}// end falling Saw

void VAOscillator::fillBufferSquarePulse(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer)
{
  float* const data = buffer.getWritePointer(0);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    if(currentPhase < double_Pi)
    {
      data[sampleIndex] = -1;
    }
    else
    {
      data[sampleIndex] = 1;
    }
    
    if(blepOn == 1)
    {
    data[sampleIndex] += getBlep(currentPhase, currentFrequency);
    if(currentPhase < double_Pi)
    {
      data[sampleIndex] -= getBlep(currentPhase + double_Pi, currentFrequency);
    }
    if(currentPhase > double_Pi)
    {
      data[sampleIndex] -= getBlep(currentPhase - double_Pi, currentFrequency);
    }
    }
  
    
    updateFrequency();
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
  }
  
}// end square pulse

void VAOscillator::fillBufferTriangle(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer)
{
  float* const data = buffer.getWritePointer(0);
  
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    if(currentPhase < double_Pi)
    {
      data[sampleIndex] = (2 * currentPhase)/double_Pi - 1;
    }
    else
    {
      data[sampleIndex] = 3 - 2 * currentPhase/double_Pi;
    }
    
    if(blepOn == 1)
    {
      data[sampleIndex] += currentFrequency * getTriRes(currentPhase, currentFrequency);
      if(currentPhase < double_Pi)
      {
        data[sampleIndex] -= currentFrequency * getTriRes(currentPhase + double_Pi, currentFrequency);
      }
      if(currentPhase > double_Pi)
      {
        data[sampleIndex] -= currentFrequency * getTriRes(currentPhase - double_Pi, currentFrequency);
      }
    }
    
    updateFrequency();
    currentPhase += phaseInc;
    
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
    
  }
}// end triangle


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
  
  twoPiHalfPulseLength = double_Pi * 6 * double_Pi/80000.0f;
  
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


void VAOscillator::updateFrequency()
{
  //max step size == 1/500 * currentfrequency
  if(std::abs(currentFrequency - targetFrequency) < 0.0005 * currentFrequency) // import change the std::abs to something crossplatform
  {
    currentFrequency = targetFrequency;
  }
  else if(currentFrequency < targetFrequency)
  {
    currentFrequency += 0.0005 * currentFrequency;
  }
  else
  {
    currentFrequency -= 0.0005 * currentFrequency;
  }
  
  updatePhaseInc();
}

void VAOscillator::updatePhaseInc()
{
  phaseInc = 2 * double_Pi * (currentFrequency/currentSampleRate);
}


double VAOscillator::getBlep(double phase, double frequency)
{
  if(phase <= twoPiHalfPulseLength * frequency)
  {
    //start at mid point and then percentage
    return blepData[(int)( (phase/(2 * twoPiHalfPulseLength * frequency)) * 2561 )];
  }
  else if(phase >= 2 * double_Pi - twoPiHalfPulseLength * frequency)
  {
    return blepData[(int)( (phase - (2 * double_Pi - twoPiHalfPulseLength * frequency) )/(2 * twoPiHalfPulseLength * frequency) * 2561 ) + 1280];
  }
  else
  {
    return 0;
  }
}

double VAOscillator::getTriRes(double phase, double frequency)
{
  if(phase <= twoPiHalfPulseLength * frequency)
  {
    //start at the middle
    return triangleResData[1280 + (int)((phase/(2 * twoPiHalfPulseLength * frequency)) * 2561 )];
  }
  else if (phase >= 2 * double_Pi - twoPiHalfPulseLength * frequency)
  {
    return triangleResData[(int)(((phase - 2 * double_Pi + twoPiHalfPulseLength * frequency)/(2 * twoPiHalfPulseLength * frequency)) * 2561 )];
  }
  else
  {
    return 0;
  }
}

void VAOscillator::postFilter(AudioBuffer<float> buffer)
{
  float* const data = buffer.getWritePointer(0);
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    postFilterState = (1/0.65) * data[sampleIndex];
    data[sampleIndex] -= (0.35/0.65) * postFilterState;
  }
}
