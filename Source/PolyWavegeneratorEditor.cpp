/*
  ==============================================================================

    PolyWavegeneratorEditor.cpp
    Created: 9 Dec 2016 11:09:22am
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "PolyWavegeneratorEditor.h"

PolyWavegeneratorEditor::PolyWavegeneratorEditor(PolyWavegeneratorProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor(p), processor(*p), parent(*b)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(110, 60);
  
}

PolyWavegeneratorEditor::~PolyWavegeneratorEditor()
{
  
}

void PolyWavegeneratorEditor::paint(juce::Graphics &g)
{
  g.fillAll(Colours::rosybrown);
}

void PolyWavegeneratorEditor::resized()
{
  
}

// copied from juce demo project GenericEditor.h
void PolyWavegeneratorEditor::sliderValueChanged(Slider* slider)
{

}
