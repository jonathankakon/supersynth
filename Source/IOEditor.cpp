/*
  ==============================================================================

    IOEditor.cpp
    Created: 1 Nov 2016 1:06:01pm
    Author:  Jonathan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "IOEditor.h"

//==============================================================================
IOEditor::IOEditor(InternalIOProcessor* p, ProcessorEditorBase* b) : AudioProcessorEditor(p), processor(*p), parent(*b)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
  setSize(100, 40);
}

IOEditor::~IOEditor()
{
}

void IOEditor::paint(Graphics& g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */

  g.fillAll(Colours::teal);   // clear the background

  g.setColour(Colours::black);
  g.setFont(14.0f);
  String title = "";
  if (processor.IOType == AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode)
    title = "Audio Input";
  else if (processor.IOType == AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode)
    title = "Audio Output";
  else if (processor.IOType == AudioProcessorGraph::AudioGraphIOProcessor::midiInputNode)
    title = "Midi Input";

  g.drawText(title, getLocalBounds(),
    Justification::centred, true);   // draw some placeholder text
}

void IOEditor::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
