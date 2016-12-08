/*
  ==============================================================================

    FIRFilterProcessor.cpp
    Created: 8 Dec 2016 1:55:46pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FIRFilterProcessor.h"
#include "FIRFilterProcessorEditor.h"

#include "Constants.h"

FIRFilterProcessor::FIRFilterProcessor() : AudioProcessor(BusesProperties()
                                                          .withInput("Audio Input", AudioChannelSet::mono())
                                                          .withOutput("Audio Output", AudioChannelSet::mono()))
{
  int size = getTapsSize(lowpass_200hz_strong);
  filterFIR = new FIRFilter(lowpass_200hz_strong, size, 0, 0);
}

FIRFilterProcessor::~FIRFilterProcessor()
{
}



void FIRFilterProcessor::processBlock(AudioSampleBuffer & buffer, juce::MidiBuffer & midiBuffer)
{
  if(bypassBool)
  {
    return;
  }
  else
  {
    filterFIR->applyFIRFilter(buffer);
  }
}

int FIRFilterProcessor::getTapsSize(const float *array)
{
  return sizeof(array)/sizeof(*array);
}

void FIRFilterProcessor::audioProcessorParameterChanged(AudioProcessor* /*processor*/, int parameterIndex, float newValue)
{
  
}


void FIRFilterProcessor::prepareToPlay(double sampleRate , int samplesPerBlock)
{
  ignoreUnused(sampleRate, samplesPerBlock);
}

void FIRFilterProcessor::releaseResources()
{
}

AudioProcessorEditor* FIRFilterProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<FIRFilterProcessor, FIRFilterProcessorEditor>(this);
}

bool FIRFilterProcessor::hasEditor() const
{
  return true;
}

const String FIRFilterProcessor::getName() const
{
  return "FIRFilterProcessor";
}

bool FIRFilterProcessor::acceptsMidi() const
{
  return false;
}

bool FIRFilterProcessor::producesMidi() const
{
  return false;
}

double FIRFilterProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int FIRFilterProcessor::getNumPrograms()
{
  return 1;
}

int FIRFilterProcessor::getCurrentProgram()
{
  return 0;
}

void FIRFilterProcessor::setCurrentProgram(int index)
{
  ignoreUnused(index);
}

const String FIRFilterProcessor::getProgramName(int index)
{
  ignoreUnused(index);
  return String();
}

void FIRFilterProcessor::changeProgramName(int index, const juce::String &newName)
{
  ignoreUnused(index, newName);
}

void FIRFilterProcessor::getStateInformation(juce::MemoryBlock &destData)
{
  ignoreUnused(destData);
}

void FIRFilterProcessor::setStateInformation(const void *data, int sizeInBytes)
{
  ignoreUnused(data, sizeInBytes);
}