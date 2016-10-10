/*
  ==============================================================================

    Worksheet.cpp
    Created: 10 Oct 2016 6:15:42pm
    Author:  Jonathan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Worksheet.h"

//==============================================================================
Worksheet::Worksheet()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	setSize(1000, 1000);
}

Worksheet::~Worksheet()
{
}

void Worksheet::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

	g.fillAll(Colour(0xFF2D2D30));   // clear the background

	Rectangle<int> r(getLocalBounds());

	g.setColour(Colour(0xFFC8C8C8));
	g.drawRect(r);

    g.setColour (Colours::darkgrey);
	for (int x = 0; x < getWidth(); x += 50)
	{
		g.drawLine(Line<float>(x, r.getY(), x, r.getBottom()), 0.3);
	}

	for (int y = 0; y < getHeight(); y += 50)
	{
		g.drawLine(Line<float>(r.getX(), y, r.getRight(), y), 0.3);
	}

	g.setColour(Colours::black);  // draw an outline around the component
    g.setFont (14.0f);
    g.drawText ("Worksheet + " + message, getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void Worksheet::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool Worksheet::isInterestedInDragSource(const SourceDetails& /*dragSourceDetails*/)
{
	// normally you'd check the sourceDescription value to see if it's the
	// sort of object that you're interested in before returning true, but for
	// the demo, we'll say yes to anything..
	return true;
}

void Worksheet::itemDragEnter(const SourceDetails& /*dragSourceDetails*/)
{
	somethingIsBeingDraggedOver = true;
	repaint();
}

void Worksheet::itemDragMove(const SourceDetails& /*dragSourceDetails*/)
{
}

void Worksheet::itemDragExit(const SourceDetails& /*dragSourceDetails*/)
{
	somethingIsBeingDraggedOver = false;
	repaint();
}

void Worksheet::itemDropped(const SourceDetails& dragSourceDetails)
{
	message = "Items dropped: " + dragSourceDetails.description.toString();

	somethingIsBeingDraggedOver = false;
	repaint();
}
