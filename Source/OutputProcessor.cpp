/*
  ==============================================================================

    OutputProcessor.cpp
    Created: 21 Oct 2016 3:37:25pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "OutputProcessor.h"

#include "OutputProcessorEditor.h"

void OutputProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  
}

void OutputProcessor::releaseResources()
{
  
}

//space for setpreferredBusArrangment



void OutputProcessor::processBlock(AudioSampleBuffer &, juce::MidiBuffer &)
{
  
}

AudioProcessorEditor* OutputProcessor::createEditor()
{
  return new OutputProcessorEditor(*this);
}

bool OutputProcessor::hasEditor() const
{
  return true;
}

const String OutputProcessor::getName() const
{
  return "Output";
}

bool OutputProcessor::acceptsMidi() const
{
  return false;
}

bool OutputProcessor::producesMidi() const
{
  return false;
}

double  OutputProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int OutputProcessor::getNumPrograms()
{
  return 1;
}

int OutputProcessor::getCurrentProgram()
{
  return 0;
}

void OutputProcessor::setCurrentProgram(int index)
{
  
}

const String OutputProcessor::getProgramName(int index)
{
  return String();
}

void OutputProcessor::changeProgramName(int index, const juce::String &newName)
{
  
}

void OutputProcessor::getStateInformation(juce::MemoryBlock &destData)
{
  
}

void OutputProcessor::setStateInformation(const void *data, int sizeInBytes)
{
  
}


