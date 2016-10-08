/*
  ==============================================================================

    WaveGeneratorProcessor.cpp
    Created: 7 Oct 2016 3:42:56pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "WaveGeneratorProcessor.h"


WaveGeneratorProcessor::WaveGeneratorProcessor()
{
  NormalisableRange<float> frequencyRange (100.0f,10000.0f);
  frequencyRange.skew = 0.2;
  
  addParameter(currentFrequency = new AudioParameterFloat("currentFrequency",
                                                          "Frequency",
                                                          frequencyRange,
                                                          440.0));
  
  addParameter(currentVolume = new AudioParameterFloat("volume",
                                                       "Volume",
                                                       NormalisableRange<float>(0.0,1.0),
                                                       0.5));
  
  currentWaveform = saw;
  currentPhase = 0;
  phaseIncrement = 0;
  
}

bool WaveGeneratorProcessor::supportsDoublePrecisionProcessing() const
{
  return true;
}

void WaveGeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  
  currentSampleRate = sampleRate;
  phaseIncrement = (*currentFrequency/currentSampleRate) * 2 * double_Pi;
  
  getProcessingPrecision();
}

void WaveGeneratorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiBuffer) //fills channels 1 and 0
{
  if(currentWaveform == sine)
  {
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      buffer.setSample(0, sampleIndex, sin(currentPhase));
      buffer.setSample(1, sampleIndex, sin(currentPhase));
    }
  }
  else if(currentWaveform == saw)
  {
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      buffer.setSample(0, sampleIndex, currentPhase/double_Pi - 1.0);
      buffer.setSample(1, sampleIndex, currentPhase/double_Pi - 1.0);
    }
  }
  else //waveform == square
  {
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      buffer.setSample(0, sampleIndex, 2 * (currentPhase && 0x8000000000000000) - 1); //extract sign bit and map to +-1
      buffer.setSample(1, sampleIndex, 2 * (currentPhase && 0x8000000000000000) - 1);
    }
  }
    
  currentPhase += phaseIncrement;
  if(currentPhase > 2 * double_Pi)
  {
    currentPhase -= 2 * double_Pi;
  }
}

void WaveGeneratorProcessor::setFrequency(float newFrequency)
{
  *currentFrequency = newFrequency;
  phaseIncrement = (*currentFrequency/currentSampleRate) * 2 * double_Pi;
}
