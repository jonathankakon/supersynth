/*
  ==============================================================================

    VAOscillator.cpp
    Created: 19 Oct 2016 6:53:50pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "VAOscillator.h"
#include "Constants.h"

VAOscillator::VAOscillator(): isActive(true), fourFoldSampleRate(0), currentPhaseOffset(double_Pi), twoPiHalfPulseLength(0), phaseToIncludeBlep(0)
{
  currentSampleRate = 0;
  currentFrequency = 440.0;

  currentPhase = 0.0;

  phaseInc = 0.0;

  phaseModAmp = 2 * double_Pi;

  postFilterState = 0.0;
}

//==============================================================================
  // waveforms:

void VAOscillator::fillBufferSine(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer, Array<int>& midiOns)// add midi buffer and know channel with control Voltage
{
  if(isActive)
  {
    float* const data = buffer.getWritePointer(0);
    float const *phaseMod = phaseModBuffer.getReadPointer(0);
    float const *volMod = volumeModBuffer.getReadPointer(0);
    float const *pitchMod = pitchModBuffer.getReadPointer(0);

    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      //check if this is the best i can do
      if(midiOns.size() != 0)
      {
        if(sampleIndex == midiOns[0])
        {
          resetPhase();
          midiOns.remove(0);
        }
      }
      
      double phase = currentPhase + phaseModAmp * phaseMod[sampleIndex] + currentPhaseOffset;
      
      while (phase < 0)
      {
        phase += 2 * double_Pi;
      }
      while (phase > 2 * double_Pi)
      {
        phase -= 2 * double_Pi;
      }
      
      data[sampleIndex] = static_cast<float>(sin(phase) * std::abs(0.5 * (volMod[sampleIndex] + 1)));
      
      currentPhase += 2 * double_Pi * ((currentFrequency * (pitchMod[sampleIndex] + 2.0))/currentSampleRate);
      
      if(currentPhase > 2 * double_Pi)
      {
        currentPhase -= 2 * double_Pi;
      }
    }
  }
  else
  {
    buffer.clear();
  }
}// end sine

void VAOscillator::fillBufferRisingSaw(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer)
{
  if(isActive)
  {
    float* const data = buffer.getWritePointer(0);
    float const *phaseMod = phaseModBuffer.getReadPointer(0);
    float const *volMod = volumeModBuffer.getReadPointer(0);
    float const *pitchMod = pitchModBuffer.getReadPointer(0);
    
    //write momentary phase values into the buffer
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      double phase = currentPhase + phaseModAmp * phaseMod[sampleIndex];
      
      while (phase < 0)
      {
        phase += 2 * double_Pi;
      }
      while (phase > 2 * double_Pi)
      {
        phase -= 2 * double_Pi;
      }
      
      currentPhase += 2 * double_Pi * ((currentFrequency * (pitchMod[sampleIndex] + 2.0))/currentSampleRate);
      
      if(currentPhase > 2 * double_Pi)
      {
        currentPhase -= 2 * double_Pi;
      }
      
      data[sampleIndex] = (float)((2 * phase)/(2 * double_Pi) - 1);
      
      data[sampleIndex] += getBlep(phase , currentFrequency);// i have to watch out here because actually currentFrequency is not valid for this calculation
      
      data[sampleIndex] *= std::abs(0.5f * (volMod[sampleIndex] + 1));
    }
  }
  else
  {
    buffer.clear();
  }
}// end rising Saw

void VAOscillator::fillBufferFallingSaw(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer)
{
  if(isActive)
  {
    fillBufferRisingSaw(buffer, phaseModBuffer, volumeModBuffer, pitchModBuffer);
    
    float* const data = buffer.getWritePointer(0);
    
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      data[sampleIndex] *= -1;
    }
  }
  else
  {
    buffer.clear();
  }
}// end falling Saw

void VAOscillator::fillBufferSquarePulse(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer)
{
  if(isActive)
  {
    float* const data = buffer.getWritePointer(0);
    float const *phaseMod = phaseModBuffer.getReadPointer(0);
    float const *volMod = volumeModBuffer.getReadPointer(0);
    float const *pitchMod = pitchModBuffer.getReadPointer(0);
    
    //write momentary phase values into the buffer
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      double phase = currentPhase + phaseModAmp * phaseMod[sampleIndex];
      
      while (phase < 0)
      {
        phase += 2 * double_Pi;
      }
      while (phase > 2 * double_Pi)
      {
        phase -= 2 * double_Pi;
      }
      
      currentPhase += 2 * double_Pi * ((currentFrequency * (pitchMod[sampleIndex] + 2.0))/currentSampleRate);
      
      if(currentPhase > 2 * double_Pi)
      {
        currentPhase -= 2 * double_Pi;
      }
      
      if(phase < double_Pi)
      {
        data[sampleIndex] = -1;
      }
      else
      {
        data[sampleIndex] = 1;
      }

      data[sampleIndex] += getBlep(phase, currentFrequency);
        
      if(phase < double_Pi)
      {
        data[sampleIndex] -= getBlep(phase + double_Pi, currentFrequency);
      }
      if(phase > double_Pi)
      {
        data[sampleIndex] -= getBlep(phase - double_Pi, currentFrequency);
      }
      
      data[sampleIndex] *= (float)std::abs(0.5 * (volMod[sampleIndex] + 1));
    }
  }
  else
  {
    buffer.clear();
  }
}// end square pulse

void VAOscillator::fillBufferTriangle(AudioBuffer<float>& buffer, AudioBuffer<float>& phaseModBuffer, AudioBuffer<float>& volumeModBuffer, AudioBuffer<float>& pitchModBuffer)
{
  if(isActive)
  {
    float* const data = buffer.getWritePointer(0);
    float const *phaseMod = phaseModBuffer.getReadPointer(0);
    float const *volMod = volumeModBuffer.getReadPointer(0);
    float const *pitchMod = pitchModBuffer.getReadPointer(0);
    
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      double phase = currentPhase + phaseModAmp * phaseMod[sampleIndex];
      
      while (phase < 0)
      {
        phase += 2 * double_Pi;
      }
      while (phase > 2 * double_Pi)
      {
        phase -= 2 * double_Pi;
      }
          
      currentPhase += 2 * double_Pi * ((currentFrequency * (pitchMod[sampleIndex] + 2.0))/currentSampleRate);
      
      if(currentPhase > 2 * double_Pi)
      {
        currentPhase -= 2 * double_Pi;
      }
    
      if(phase < double_Pi)
      {
        data[sampleIndex] = (float)((2 * phase)/double_Pi - 1);
      }
      else
      {
        data[sampleIndex] = (float)((3 - 2 * phase/double_Pi));
      }
      
      //the 0.000026 is kind of a magic number i didnt calculate it just found it by trying out
      data[sampleIndex] += (float)(0.000026 * currentFrequency * getTriRes(phase, currentFrequency));
      if(phase < double_Pi)
      {
        data[sampleIndex] -= (float)(0.000026 * currentFrequency * getTriRes(phase + double_Pi, currentFrequency));
      }
      else if(phase >= double_Pi)
      {
        data[sampleIndex] -= (float)(0.000026 * currentFrequency * getTriRes(phase - double_Pi, currentFrequency));
      }
      
      data[sampleIndex] *= (float)std::abs(0.5 * (volMod[sampleIndex] + 1));
    }
  }
  else
  {
    buffer.clear();
  }
}// end triangle


  //==============================================================================
  // getters and setters

double VAOscillator::getSampleRate() const
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

double VAOscillator::getFrequency() const
{
  return currentFrequency;
}

void VAOscillator::setFrequency(double newFrequency)
{
  currentFrequency = newFrequency;
  updatePhaseInc();
}

void VAOscillator::setActive(bool shouldBeActive)
{
  isActive = shouldBeActive;
}

  //==============================================================================
  // Privates:

void VAOscillator::updatePhaseInc()
{
  phaseInc = 2 * double_Pi * (currentFrequency/currentSampleRate);
}


float VAOscillator::getBlep(double phase, double frequency) const
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

float VAOscillator::getTriRes(double phase, double frequency) const
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
    data[sampleIndex] -= (float)((0.35/0.65) * postFilterState);
  }
}
