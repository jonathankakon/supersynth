/*
  ==============================================================================

    FilterProcessor.cpp
    Created: 20 Oct 2016 4:38:12pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "FilterProcessor.h"
#include "FilterProcessorEditor.h"

void FilterProcessor::prepareToPlay(double sampleRate , int samplesPerBlock)
{
  
}

void FilterProcessor::releaseResources()
{
  
}

void FilterProcessor::processBlock(AudioSampleBuffer &, juce::MidiBuffer &)
{
  
}

AudioProcessorEditor* FilterProcessor::createEditor()
{
  return new FilterProcessorEditor(* this);
}

bool FilterProcessor::hasEditor() const
{
  return true;
}

const String FilterProcessor::getName() const
{
  return "aFilter";
}

bool FilterProcessor::acceptsMidi() const
{
  return false;
}

bool FilterProcessor::producesMidi() const
{
  return false;
}

double FilterProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int FilterProcessor::getNumPrograms()
{
  return 1;
}

int FilterProcessor::getCurrentProgram()
{
  return 0;
}

void FilterProcessor::setCurrentProgram(int index)
{
}

const String FilterProcessor::getProgramName(int index)
{
  return String();
}

void FilterProcessor::changeProgramName(int index, const juce::String &newName)
{
}

void FilterProcessor::getStateInformation(juce::MemoryBlock &destData)
{
  
}

void FilterProcessor::setStateInformation(const void *data, int sizeInBytes)
{
  
}





