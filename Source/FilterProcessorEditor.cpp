/*
  ==============================================================================

    FilterProcessorEditor.cpp
    Created: 7 Oct 2016 3:18:49pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FilterProcessorEditor.h"


//==============================================================================
FilterProcessorEditor::FilterProcessorEditor (FilterProcessor& p)
: ProcessorEditorBase (&p), processor (p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(200, 100);
}


FilterProcessorEditor::~FilterProcessorEditor()
{
}
//==============================================================================
void FilterProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::lightblue);
  g.setColour (Colours::black);
  g.setFont (15.0f);
  g.drawFittedText ("Filter!", getLocalBounds(), Justification::centred, 1);
}


void FilterProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}
