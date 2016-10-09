/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ToolboxComponent.h"


//==============================================================================
SupersynthAudioProcessorEditor::SupersynthAudioProcessorEditor (SupersynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), toolbox(new ToolboxComponent())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setResizable(true, true);
	setResizeLimits(300, 200, 10000, 10000);

	addAndMakeVisible(toolbox);
	Rectangle<int> r(getLocalBounds());
	toolbox->setBounds(r.withWidth(200));

}

SupersynthAudioProcessorEditor::~SupersynthAudioProcessorEditor()
{
	toolbox = nullptr;
}

//==============================================================================
void SupersynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colour(0xFF293134));
    g.setColour (Colour(0xFFD955A9));
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SupersynthAudioProcessorEditor::resized()
{
	Rectangle<int> r(getLocalBounds());
	toolbox->setBounds(r.withWidth(200));
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
