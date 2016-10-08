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
: AudioProcessorEditor (&p), processor (p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (400, 300);
  
}

WaveGeneratorProcessorEditor::~WaveGeneratorProcessorEditor()
{
}

//==============================================================================
void WaveGeneratorProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::white);
  
  g.setColour (Colours::black);
  g.setFont (15.0f);
  g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void WaveGeneratorProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}

void WaveGeneratorProcessorEditor::sliderValueChanged(Slider* slider)
{
  
}
