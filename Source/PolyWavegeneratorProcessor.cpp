/*
  ==============================================================================

    PolyWavegeneratorProcessor.cpp
    Created: 9 Dec 2016 11:08:47am
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "PolyWavegeneratorProcessor.h"
#include "PolyWavegeneratorEditor.h"

PolyWavegeneratorProcessor::PolyWavegeneratorProcessor()
{
  
}

PolyWavegeneratorProcessor::~PolyWavegeneratorProcessor()
{
  
}

void PolyWavegeneratorProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  
}

void PolyWavegeneratorProcessor::releaseResources()
{
  
}

void PolyWavegeneratorProcessor::audioProcessorParameterChanged(AudioProcessor *processor, int parameterIndex, float newValue)
{
  
}

void PolyWavegeneratorProcessor::processBlock(AudioSampleBuffer&, MidiBuffer&)
{
  
}

AudioProcessorEditor* PolyWavegeneratorProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<PolyWavegeneratorProcessor, PolyWavegeneratorEditor>(this);
}
