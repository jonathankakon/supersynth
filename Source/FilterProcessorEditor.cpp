/*
  ==============================================================================

    FilterProcessorEditor.cpp
    Created: 7 Oct 2016 3:18:49pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FilterProcessor.h"
#include "FilterProcessorEditor.h"


//==============================================================================
FilterProcessorEditor::FilterProcessorEditor (FilterProcessor& p)
: FilterProcessorEditor (&p), processor (p)
{
  setSize(400, 300);
}


FilterProcessorEditor::~FilterProcessorEditor()
{
}


void FilterProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}
