/*
  ==============================================================================

    NoiseGeneratorProcessorEditor.cpp
    Created: 27 Nov 2016 8:04:13pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "NoiseGeneratorProcessorEditor.h"
#include "NoiseGeneratorProcessor.h"

NoiseGeneratorProcessorEditor::NoiseGeneratorProcessorEditor (NoiseGeneratorProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor (p), processor (*p), parent(*b)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (80, 80);

}

NoiseGeneratorProcessorEditor::~NoiseGeneratorProcessorEditor()
{
  
}

void NoiseGeneratorProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll (Colours::lightgreen);
}

void NoiseGeneratorProcessorEditor::resized()
{
  
}
