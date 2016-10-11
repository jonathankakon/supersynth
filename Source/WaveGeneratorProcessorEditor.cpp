/*
  ==============================================================================

    WaveGeneratorProcessorEditor.cpp
    Created: 7 Oct 2016 3:43:18pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "WaveGeneratorProcessorEditor.h"
#include "WaveGeneratorProcessor.h"


//==============================================================================
WaveGeneratorProcessorEditor::WaveGeneratorProcessorEditor (WaveGeneratorProcessor& p)
: ProcessorEditorBase (&p), processor (p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (200, 100);
  
}

WaveGeneratorProcessorEditor::~WaveGeneratorProcessorEditor()
{
}

//==============================================================================
void WaveGeneratorProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::lightgreen);
  
  g.setColour (Colours::black);
  g.setFont (15.0f);
  g.drawFittedText ("Wave Generator!", getLocalBounds(), Justification::centred, 1);
}

void WaveGeneratorProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}

void WaveGeneratorProcessorEditor::sliderValueChanged(Slider* slider)
{
  
}
