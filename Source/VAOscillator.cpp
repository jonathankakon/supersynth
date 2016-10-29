/*
  ==============================================================================

    VAOscillator.cpp
    Created: 19 Oct 2016 6:53:50pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "VAOscillator.h"

VAOscillator::VAOscillator()
{
  currentSampleRate = 0;
  currentFrequency = 440.0;
  
  currentPhase = 0.0;
  
  phaseInc = 0.0;
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
  fillBufferNonLimitedRisingSaw(buffer);
}

void VAOscillator::fillBufferSquarePulse(AudioBuffer<float> &buffer)
{
  fillBufferNonLimitedSquare(buffer);
}



  //==============================================================================

double VAOscillator::getSampleRate()
{
  return currentSampleRate;
}

void VAOscillator::setSampleRate(double newSampleRate)
{
  currentSampleRate = newSampleRate;
  
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


void VAOscillator::fillBufferNonLimitedRisingSaw(AudioBuffer<float> &buffer)
{
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    buffer.setSample(0, sampleIndex, (currentPhase/double_Pi - 1.0) );// remove this. pass around just one buffer and copy in the output processor
    
    // TODO: Add code for mixing in the BLEP
    
    /*
    
    if (currentPhase < 2 * double_Pi * currentFrequency * lengthOfPulse) // maybe save this in a member variable which is updated with frequency and sampleRate to be faster
    {
      mixInBlep();
    }

    else if (currentPhase > 2 * double_Pi (1 - currentFrequency * lengthOfPulse) )
    {
      mixInBlep
    }
     
    */
    
    currentPhase += phaseInc;
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
  }
}

void VAOscillator::fillBufferNonLimitedSquare(AudioBuffer<float> &buffer)
{
  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    if(currentPhase > double_Pi)
    {
      buffer.setSample(0, sampleIndex, 1);
    }
    else
    {
      buffer.setSample(0, sampleIndex, -1);
    }
    
    currentPhase += phaseInc;
    if(currentPhase > 2 * double_Pi)
    {
      currentPhase -= 2 * double_Pi;
    }
  }
}


//==============================================================================

void VAOscillator::updatePhaseInc()
{
  phaseInc = 2 * double_Pi * (currentFrequency/currentSampleRate);
}

