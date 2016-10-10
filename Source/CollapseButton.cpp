/*
  ==============================================================================

    CollapseButton.cpp
    Created: 10 Oct 2016 5:42:54pm
    Author:  Jonathan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "CollapseButton.h"

//==============================================================================
CollapseButton::CollapseButton()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

CollapseButton::~CollapseButton()
{
}

void CollapseButton::paintButton (Graphics& g, bool isMouseOver, bool isButtonDown)
{
	g.fillAll (Colour(0x00));
	g.setColour(Colour(0xFF1F1F1F));

	if (isMouseOver)
	{
		g.fillAll(Colour(0xFF007ACC));
		g.setColour(Colour(0xFFFFFFFF));
	}

	if (isButtonDown)
	{
		g.fillAll(Colour(0xFF1C97EA));
		g.setColour(Colour(0xFFFFFFFF));
	}
      // clear the background
	
	Rectangle<int> r (getLocalBounds());

	juce::Line<float> line;

	if (!this->getToggleState())
	{
		line = Line<float>(r.getRight(), r.getBottom() / 2, r.getX() + 5, r.getBottom() / 2);
	}
	else
	{
		line = Line<float>(r.getX(), r.getBottom() / 2, r.getRight() - 5, r.getBottom() / 2);
	}

	g.drawArrow(line, 0, 10, 10);
}

void CollapseButton::resized()
{
	this->repaint();
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
