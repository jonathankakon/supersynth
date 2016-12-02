/*
  ==============================================================================

    InputConnectorProcessor.cpp
    Created: 4 Nov 2016 1:35:47am
    Author:  Jonathan

  ==============================================================================
*/

#include "InputConnectorProcessor.h"

InputConnectorProcessor::InputConnectorProcessor() : AudioProcessor(BusesProperties()
  .withInput("0", AudioChannelSet::mono())
  .withOutput("output", AudioChannelSet::mono()))
{
}

InputConnectorProcessor::~InputConnectorProcessor()
{
}

void InputConnectorProcessor::prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/)
{
}

void InputConnectorProcessor::releaseResources()
{
}

void InputConnectorProcessor::processBlock(AudioSampleBuffer&, MidiBuffer&)
{
}
