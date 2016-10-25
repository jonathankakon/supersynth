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
    .withOutput("Audio", AudioChannelSet::stereo())
    .withInput("FrequencyControl", AudioChannelSet::mono())
    .withInput("VolumeControl", AudioChannelSet::mono()))
{
  NormalisableRange<float> frequencyRange (50.0f,20000.0f,0.1, 0.001);
  
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
  if (parameterIndex == 0)
  {
    oscillator.setFrequency(newValue);
  }
}

void WaveGeneratorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiBuffer) //fills channels 1 and 0
{
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
