/*
  ==============================================================================

    InputConnector.cpp
    Created: 21 Oct 2016 4:02:01pm
    Author:  jonathan kakon

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "InputConnector.h"
#include "ProcessorEditorBase.h"

//==============================================================================
InputConnector::InputConnector(AudioProcessor* p, ProcessorEditorBase * b, int mixerNodeId) : AudioProcessorEditor(p),
   nodeId(mixerNodeId), sliders(OwnedArray<Slider>()), processor(*p), parent(*b)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  setSize(32, 32);
  Slider* firstSlider = new Slider();
  firstSlider->setRange(0, 1);
  firstSlider->addListener(this);
  firstSlider->setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
  firstSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  firstSlider->setBounds(Rectangle<int>(getLocalBounds().withX(getLocalBounds().getX() + 11).withY(getLocalBounds().getY() + 4).withWidth(24).withHeight(24)));
  sliders.add(firstSlider);

  parent.registerImmobileObject(*firstSlider);
  addAndMakeVisible(firstSlider);
}

InputConnector::~InputConnector()
{
}

void InputConnector::paint (Graphics& g)
{
  g.setColour(Colour(0x4F000000));
  g.fillRoundedRectangle (getLocalBounds().toFloat(), 10);
  
  g.setColour(Colours::blue);
  for (int i = 0; i < sliders.size(); i++)
  {
    Rectangle<float> r(getLocalBounds().withHeight(12).withWidth(12).toFloat());
    
    r.setX(0);
    r.setY(r.getY() + 10 + i*40);

    g.fillEllipse(r);
  }
}

void InputConnector::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void InputConnector::sliderValueChanged(Slider* /*slider*/)
{
}

void InputConnector::mouseDown(const MouseEvent & e)
{
  getWorksheet()->beginConnectorDrag(0, 0, nodeId, 0, e);
}

Point<int> InputConnector::getClosestConnector(const int /*x*/, const int y)
{
  return Point<int>(6, y - (y % 32) + 16);
}

void InputConnector::mouseDrag(const MouseEvent & e)
{
  getWorksheet()->dragConnector(e);
}

void InputConnector::mouseUp(const MouseEvent & e)
{
  getWorksheet()->endDraggingConnector(e);
}