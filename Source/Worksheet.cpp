/*
  ==============================================================================

    Worksheet.cpp
    Created: 10 Oct 2016 6:15:42pm
    Author:  Jonathan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Worksheet.h"
#include "ToolboxComponent.h"

#include "PluginEditor.h"

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
		g.drawLine(Line<float>((float)x, (float)r.getY(), (float)x, (float)r.getBottom()), 0.3f);
	}

	for (int y = 0; y < getHeight(); y += 50)
	{
		g.drawLine(Line<float>((float)r.getX(), (float)y, (float)r.getRight(), (float)y), 0.3f);
	}
}

void Worksheet::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool Worksheet::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
	if (dragSourceDetails.description.isInt())
	{
		return true;
	}
	return false;
}

void Worksheet::itemDragEnter(const SourceDetails& dragSourceDetails)
{
  ignoreUnused(dragSourceDetails);
	somethingIsBeingDraggedOver = true;
	beginDragAutoRepeat(100);
	repaint();
}

void Worksheet::itemDragMove(const SourceDetails& dragSourceDetails)
{	
	Viewport* const viewport = findParentComponentOfClass<Viewport>(); //Get the parent viewport
	if (viewport != nullptr) //Check for nullness
	{
		int x = viewport->getViewPositionX();
		int y = viewport->getViewPositionY();

		int relativeX = dragSourceDetails.localPosition.getX() - x;
		int relativeY = dragSourceDetails.localPosition.getY() - y;

		viewport->autoScroll(relativeX, relativeY, 50, 7);																																		  // ... based on the displayed area, paint just what's visible ... //
	}
}

void Worksheet::itemDragExit(const SourceDetails& /*dragSourceDetails*/)
{
	somethingIsBeingDraggedOver = false;
	beginDragAutoRepeat(0);
	repaint();
}

void Worksheet::itemDropped(const SourceDetails& dragSourceDetails)
{
	dropPosition = dragSourceDetails.localPosition;

	findParentComponentOfClass<SupersynthAudioProcessorEditor>()->addAudioProcessor(dragSourceDetails.description);

	somethingIsBeingDraggedOver = false;
	beginDragAutoRepeat(0);
	repaint();
}

void Worksheet::addEditor(Component* editor)
{
	editors.add(editor);
	addAndMakeVisible(editor);
	editor->setBounds(Rectangle<int>(dropPosition.getX() - editor->getWidth() / 2, dropPosition.getY() - editor->getHeight() / 2, editor->getWidth(), editor->getHeight()));
}
