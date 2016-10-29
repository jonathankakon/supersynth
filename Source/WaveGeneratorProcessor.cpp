/*
  ==============================================================================

    WaveGeneratorProcessor.cpp
    Created: 7 Oct 2016 3:42:56pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "WaveGeneratorProcessor.h"
#include "WaveGeneratorProcessorEditor.h"


WaveGeneratorProcessor::WaveGeneratorProcessor() : AudioProcessor(BusesProperties()
    .withOutput("Audio", AudioChannelSet::mono())
    .withInput("FrequencyControl", AudioChannelSet::mono())
    .withInput("VolumeControl", AudioChannelSet::mono()))
{
  NormalisableRange<float> frequencyRange (50.0f,20000.0f,0.1f, 0.001f);
  
  addParameter(currentFrequency = new AudioParameterFloat("currentFrequency",
                                                          "Frequency",
                                                          frequencyRange,
                                                          48000.0/128.0));
  
  addParameter(currentVolume = new AudioParameterFloat("volume",
                                                       "Volume",
                                                       NormalisableRange<float>(0.0,1.0),
                                                       0.5));

  addListener(this);
  
  currentWaveform = saw;
  
}

WaveGeneratorProcessor::~WaveGeneratorProcessor()
{
  
}

void WaveGeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  ignoreUnused(samplesPerBlock);

  currentSampleRate = sampleRate;
  
  getProcessingPrecision();
  
  oscillator.setSampleRate(sampleRate);
  oscillator.setFrequency(currentFrequency);
}

void WaveGeneratorProcessor::releaseResources()
{
}

void WaveGeneratorProcessor::audioProcessorParameterChanged(AudioProcessor * processor, int parameterIndex, float newValue)
{
  ignoreUnused(processor);
  if (parameterIndex == 0)
  {
    oscillator.setFrequency(newValue);
  }
}

void WaveGeneratorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiBuffer) //fills channels 1 and 0
{
  ignoreUnused(midiBuffer);
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
  if(currentWaveform == sine)
  {
    oscillator.fillBufferSine(outBuffer);
  }
  else if(currentWaveform == saw)
  {
    oscillator.fillBufferRisingSaw(outBuffer);
  }
  else //waveform == square
  {
    oscillator.fillBufferSquarePulse(outBuffer);
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
  ignoreUnused(index);
}

const String WaveGeneratorProcessor::getProgramName(int index)
{
  ignoreUnused(index);
  return String();
}

void WaveGeneratorProcessor::changeProgramName(int index, const String& newName)
{
  ignoreUnused(index, newName);
}

void WaveGeneratorProcessor::getStateInformation(MemoryBlock& destData)
{
  ignoreUnused(destData);
}

void WaveGeneratorProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  ignoreUnused(data, sizeInBytes);
}

void WaveGeneratorProcessor::setWaveform(waveform newWaveform)
{
  currentWaveform = newWaveform;
}
