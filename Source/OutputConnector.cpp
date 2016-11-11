/*
  ==============================================================================

    OutputConnector.cpp
    Created: 21 Oct 2016 4:02:11pm
    Author:  jonathan kakon

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OutputConnector.h"

//==============================================================================
OutputConnector::OutputConnector(int processorNodeId) : nodeId(processorNodeId)
{
  setSize(32, 32);
}

OutputConnector::~OutputConnector()
{
}

void OutputConnector::paint (Graphics& g)
{
  g.setColour(Colour(0x4F0F0F0F));
  g.fillRoundedRectangle(getLocalBounds().toFloat(), 10);

  Rectangle<float> r(getLocalBounds().withHeight(12).withWidth(12).toFloat());

  r.setX(10);
  r.setY(r.getY() + 10);

  g.setColour(Colours::blue);
  g.fillEllipse(r);
}

void OutputConnector::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

Point<int> OutputConnector::getClosestConnector(const int x, const int y)
{
  return Point<int>(16, 16);
}
