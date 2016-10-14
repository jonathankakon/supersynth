/*
  ==============================================================================

    WaveGeneratorProcessor.cpp
    Created: 7 Oct 2016 3:42:56pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "WaveGeneratorProcessor.h"
#include "WaveGeneratorProcessorEditor.h"


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

WaveGeneratorProcessor::~WaveGeneratorProcessor()
{
  
}

void WaveGeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  
  currentSampleRate = sampleRate;
  phaseIncrement = (*currentFrequency/currentSampleRate) * 2 * double_Pi;
  
  getProcessingPrecision();
}

void WaveGeneratorProcessor::releaseResources()
{
  ;
}

void WaveGeneratorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiBuffer) //fills channels 1 and 0
{
  if(currentWaveform == sine)
  {
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      buffer.setSample(0, sampleIndex, sin(currentPhase) * (*currentVolume) );
      buffer.setSample(1, sampleIndex, sin(currentPhase) * (*currentVolume) );
      
      currentPhase += phaseIncrement;
      if(currentPhase > 2 * double_Pi)
      {
        currentPhase -= 2 * double_Pi;
      }
    }
  }
  else if(currentWaveform == saw)
  {
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      buffer.setSample(0, sampleIndex, (currentPhase/double_Pi - 1.0) * (*currentVolume) );
      buffer.setSample(1, sampleIndex, (currentPhase/double_Pi - 1.0) * (*currentVolume) );
      
      currentPhase += phaseIncrement;
      if(currentPhase > 2 * double_Pi)
      {
        currentPhase -= 2 * double_Pi;
      }
    }
  }
  else //waveform == square
  {
    for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
      if(currentPhase > double_Pi)
      {
        buffer.setSample(0, sampleIndex, 1 * (*currentVolume));
      }
      else
      {
        buffer.setSample(1, sampleIndex, -1 * (*currentVolume));
      }
      
      currentPhase += phaseIncrement;
      if(currentPhase > 2 * double_Pi)
      {
        currentPhase -= 2 * double_Pi;
      }
    }
  }
  
}// End processBlock

AudioProcessorEditor* WaveGeneratorProcessor::createEditor()
{
  return new WaveGeneratorProcessorEditor (*this);
}

bool WaveGeneratorProcessor::hasEditor() const
{
  return true;
}


bool WaveGeneratorProcessor::supportsDoublePrecisionProcessing() const
{
  return false;
}

const String WaveGeneratorProcessor::getName() const
{
  return "aWave";
}

bool WaveGeneratorProcessor::acceptsMidi() const
{
  return true;
}

bool WaveGeneratorProcessor::producesMidi() const
{
  return false;
}

double WaveGeneratorProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int WaveGeneratorProcessor::getNumPrograms()
{
  return 1;
}

int WaveGeneratorProcessor::getCurrentProgram()
{
  return 1;
}

void WaveGeneratorProcessor::setCurrentProgram(int index)
{
  ;
}

const String WaveGeneratorProcessor::getProgramName(int index)
{
  return String();
}

void WaveGeneratorProcessor::changeProgramName(int index, const String& newName)
{
  ;
}

void WaveGeneratorProcessor::getStateInformation(MemoryBlock& destData)
{
  ;
}

void WaveGeneratorProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  ;
}

void WaveGeneratorProcessor::setWaveform(waveform newWaveform)
{
  currentWaveform = newWaveform;
}
