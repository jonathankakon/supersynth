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
                                                          220.0));
  
  addParameter(currentVolume = new AudioParameterFloat("volume",
                                                       "Volume",
                                                       NormalisableRange<float>(0.0,1.0),
                                                       0.5));
  
  currentWaveform = square;
  
}

WaveGeneratorProcessor::~WaveGeneratorProcessor()
{
  
}

void WaveGeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  
  currentSampleRate = sampleRate;
  
  getProcessingPrecision();
  
  oscillator.setSampleRate(sampleRate);
  oscillator.setFrequency(*currentFrequency);
}

void WaveGeneratorProcessor::releaseResources()
{
  ;
}

void WaveGeneratorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiBuffer) //fills channels 1 and 0
{
  if(currentWaveform == sine)
  {
    oscillator.fillBufferSine(buffer);
  }
  else if(currentWaveform == saw)
  {
    oscillator.fillBufferRisingSaw(buffer);
  }
  else //waveform == square
  {
    oscillator.fillBufferSquarePulse(buffer);
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
