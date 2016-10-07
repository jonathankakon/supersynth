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
  
  
}

bool WaveGeneratorProcessor::supportsDoublePrecisionProcessing() const
{
  return true;
}

void WaveGeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  
  currentSampleRate = sampleRate;
  
  getProcessingPrecision();
}

void WaveGeneratorProcessor::processBlock (AudioSampleBuffer&, MidiBuffer&)
{
  
}
