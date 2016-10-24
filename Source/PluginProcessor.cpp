/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SupersynthAudioProcessor::SupersynthAudioProcessor()
{
}

SupersynthAudioProcessor::~SupersynthAudioProcessor()
{
}


bool SupersynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SupersynthAudioProcessor::createEditor()
{
    return new SupersynthAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SupersynthAudioProcessor();
}
