/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SupersynthAudioProcessor::SupersynthAudioProcessor() : AudioProcessor(BusesProperties()
    .withInput("AudioIn", AudioChannelSet::mono(), true)
    .withOutput("AudioOut", AudioChannelSet::mono(), true)),
  graph(new AudioProcessorGraph())
{
}

SupersynthAudioProcessor::~SupersynthAudioProcessor()
{
  graph = nullptr;
}

bool SupersynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

void SupersynthAudioProcessor::getStateInformation(MemoryBlock& destData)
{
  if (stateInformation != nullptr)
  {
    copyXmlToBinary(*stateInformation, destData);
  }
}

void SupersynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  stateInformation = getXmlFromBinary(data, sizeInBytes);
}

AudioProcessorEditor* SupersynthAudioProcessor::createEditor()
{
  return new SupersynthAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SupersynthAudioProcessor();
}
