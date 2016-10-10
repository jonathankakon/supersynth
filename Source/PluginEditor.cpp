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
    : AudioProcessorEditor (&p), processor (p),
	toolbox(new ToolboxComponent()), 
	collapseButton(new CollapseButton()),
	viewport(new Viewport()),
	worksheet(new Worksheet())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setResizable(true, true);
	setResizeLimits(300, 200, 10000, 10000);

	addAndMakeVisible(toolbox);
	Rectangle<int> r(getLocalBounds());
	toolbox->setBounds(r.withWidth(180));

	addAndMakeVisible(collapseButton);
	collapseButton->setBounds(r.withWidth(200).withTrimmedLeft(180));
	collapseButton->addListener(this);

	addAndMakeVisible(viewport);
	viewport->setBounds(r.withTrimmedLeft(200));
	viewport->setScrollOnDragEnabled(true);
	viewport->setScrollBarsShown(true, true);
	viewport->setViewedComponent(worksheet);
	viewport->getHorizontalScrollBar()->setColour(ScrollBar::ColourIds::backgroundColourId, Colour(0xFFFFFFFF));
}

SupersynthAudioProcessorEditor::~SupersynthAudioProcessorEditor()
{
	toolbox = nullptr;
	worksheet = nullptr;
	viewport = nullptr;
	collapseButton = nullptr;
}

//==============================================================================
void SupersynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colour(0xFFC8C8C8));
    g.setColour (Colour(0xFFD955A9));
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SupersynthAudioProcessorEditor::resized()
{
	Rectangle<int> r(getLocalBounds());
	toolbox->setBounds(r.withWidth(toolbox->getWidth()).withX(toolbox->getX()));
	Rectangle<int> toolsBounds(toolbox->getBounds());
	collapseButton->setBounds(r.withWidth(20).withX(toolsBounds.getRight()));
	viewport->setBounds(r.withTrimmedLeft(toolsBounds.getWidth() + toolsBounds.getX() + 20));


    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


void SupersynthAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{

	if (buttonThatWasClicked == collapseButton)
	{
		open = collapseButton->getToggleState();

		if (open)
		{
			Desktop::getInstance().getAnimator().animateComponent(toolbox, toolbox->getBounds().withX(-(toolbox->getWidth())), 1, 100, true, 1, 0.01);
			Desktop::getInstance().getAnimator().animateComponent(collapseButton, collapseButton->getBounds().withX(0), 1, 100, true, 1, 0.01);
			Desktop::getInstance().getAnimator().animateComponent(viewport, viewport->getBounds().withX(20).withRight(this->getRight()), 1, 100, true, 1, 0.01);
		}
		else
		{
			Rectangle<int> newToolboxBounds(toolbox->getBounds().withX(0));
			Desktop::getInstance().getAnimator().animateComponent(toolbox, newToolboxBounds, 1, 100, true, 1, 0.01);
			Desktop::getInstance().getAnimator().animateComponent(collapseButton, collapseButton->getBounds().withX(newToolboxBounds.getRight()), 1, 100, true, 1, 0.01);
			Desktop::getInstance().getAnimator().animateComponent(viewport, viewport->getBounds().withX(newToolboxBounds.getRight() + 20).withRight(this->getRight()), 1, 100, true, 1, 0.01);
		}
	}
}
