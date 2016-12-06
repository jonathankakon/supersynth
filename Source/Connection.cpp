/*
  ==============================================================================

    Connection.cpp
    Created: 28 Oct 2016 4:18:36pm
    Author:  jonathan kakon

    Heavily copied from JUCE Plugin Host example.
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Connection.h"
#include "Worksheet.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "ProcessorEditorBase.h"

Connection::Connection(int inId, int inChannel, int outId, int outChannel) : Component(),
inputNodeId(inId), inputNodeChannel(inChannel), outputNodeId(outId), outputNodeChannel(outChannel),
draggingToInput(inId == 0), dragging(false), lastInputX(0),
lastOutputX(0), lastOutputY(0), lastInputY(0), stopPathRedraw(false)
{
  rmsColour = Colour(0, 0, 255);
  setAlwaysOnTop(false);
}

Connection::Connection(Connection& c) : Component(),
inputNodeId(c.inputNodeId), inputNodeChannel(c.inputNodeChannel), outputNodeId(c.outputNodeId),
outputNodeChannel(c.outputNodeChannel), draggingToInput(c.inputNodeId == 0), stopPathRedraw(c.getStopPathRedraw()),
dragging(false), lastInputX(c.getX1()), lastOutputX(c.getX2()), lastOutputY(c.getY2()), lastInputY(c.getY1())
{
  setAlwaysOnTop(false);
  resizeToFit();
}

Connection::~Connection()
{
  findParentComponentOfClass<Worksheet>()->clearEditorListeners(this);
}

void Connection::paint (Graphics& g)
{
  g.setColour(rmsColour);
  g.fillPath(path);
}

void Connection::resized()
{
  if(stopPathRedraw)
  {
    stopPathRedraw = false;
    return;
  }
  float x1, y1, x2, y2;
  getPoints(x1, y1, x2, y2);

  lastInputX = x1;
  lastInputY = y1;
  lastOutputX = x2;
  lastOutputY = y2;


  stopPathRedraw = true;
  setBounds(getX() - 100, getY(), this->getWidth() + 200, this->getHeight());

  x1 -= getX();
  y1 -= getY();
  x2 -= getX();
  y2 -= getY();

  path.clear();
  path.startNewSubPath(x1, y1);
  path.cubicTo(x1 - jmin(1.2f*std::abs(x2 - x1), 100.0f), y1,
               x2 + jmin(1.2f*std::abs(x2 - x1), 100.0f), y2,
    x2, y2);

  PathStrokeType wideStroke(8.0f);
  wideStroke.createStrokedPath(hitPath, path);

  PathStrokeType stroke(2.5f);
  stroke.createStrokedPath(path, path);

  path.setUsingNonZeroWinding(true);

}

bool Connection::hitTest(int x, int y)
{
  if (hitPath.contains((float)x, (float)y))
  {
    double distanceFromStart, distanceFromEnd;
    getDistancesFromEnds(x, y, distanceFromStart, distanceFromEnd);

    //avoid clicking when over connector
    bool isHit = distanceFromStart > 7.0 && distanceFromEnd > 7.0;
    return isHit;
  }

  return false;
}

void Connection::mouseDown(const MouseEvent&)
{
  dragging = false;
}

void Connection::mouseDrag(const MouseEvent& e)
{
  if (dragging)
  {
    getWorksheet()->dragConnector(e);
  }
  else if (e.mouseWasDraggedSinceMouseDown())
  {
    dragging = true;

    double distanceFromStart, distanceFromEnd;
    getDistancesFromEnds(e.x, e.y, distanceFromStart, distanceFromEnd);

    draggingToInput = (distanceFromStart < distanceFromEnd);

    getWorksheet()->beginConnectorDrag(outputNodeId, outputNodeChannel, inputNodeId, inputNodeChannel, e);

    inputNodeId = (draggingToInput ? 0 : inputNodeId);
    outputNodeId = (draggingToInput ? outputNodeId : 0);
  }
}

void Connection::mouseUp(const MouseEvent& e)
{
  if (dragging)
    getWorksheet()->endDraggingConnector(e);
}

void Connection::componentMovedOrResized(Component& component, bool /*wasMoved*/, bool /*wasResized*/)
{
  if (ProcessorEditorBase* editor = reinterpret_cast<ProcessorEditorBase*>(&component))
  {
    int x = 0;
    int y = 0;

    if(editor->hasInputWithId(inputNodeId, x, y))
    {
      lastInputX = (float)(x + editor->getX());
      lastInputY = (float)(y + editor->getY());
    }
    else if (editor->hasOutputWithId(outputNodeId, x, y))
    {
      lastOutputX = (float)(x + editor->getX());
      lastOutputY = (float)(y + editor->getY());
    }
  }
  resizeToFit();
}

void Connection::dragEnd(int x, int y)
{
  lastInputX = (float)x;
  lastInputY = (float)y;
  resizeToFit();
}

void Connection::dragStart(int x, int y)
{
  lastOutputX = (float)x;
  lastOutputY = (float)y;
  resizeToFit();
}

void Connection::update()
{
  float x1, y1, x2, y2;
  getPoints(x1, y1, x2, y2);

  if (lastInputX != x1
    || lastInputY != y1
    || lastOutputX != x2
    || lastOutputY != y2)
  {
    resizeToFit();
  }
}

void Connection::setFixedConnectorPosition(const Point<int>& point)
{
  if (draggingToInput)
  {
    lastOutputX = (float)point.getX();
    lastOutputY = (float)point.getY();
  }
  else
  {
    lastInputX = (float)point.getX();
    lastInputY = (float)point.getY();
  }
  resizeToFit();
}

void Connection::setDestNodeId(int nodeId)
{
  if (draggingToInput)
    inputNodeId = nodeId;
  else
    outputNodeId = nodeId;
}

void Connection::setPathColourFromRms(float rms)
{
  NormalisableRange<float> range = NormalisableRange<float>(0, 1, 1e-7, 4);
  rmsColour = Colour(range.convertFrom0to1(rms) * 255, 0, (1 - rms) * 255);
  repaint();
}

void Connection::getDistancesFromEnds(int x, int y, double& distanceFromStart, double& distanceFromEnd) const
{
  float x1, y1, x2, y2;
  getPoints(x1, y1, x2, y2);

  distanceFromStart = juce_hypot(x - (x1 - getX()), y - (y1 - getY()));
  distanceFromEnd = juce_hypot(x - (x2 - getX()), y - (y2 - getY()));
}

void Connection::getPoints(float& x1, float& y1, float& x2, float& y2) const
{
  
  x1 = lastInputX;
  y1 = lastInputY;
  x2 = lastOutputX;
  y2 = lastOutputY;
}

void Connection::resizeToFit()
{
  float x1, y1, x2, y2;
  getPoints(x1, y1, x2, y2);

  const Rectangle<int> newBounds(std::abs((int)jmin(x1, x2) - 4),
    std::abs((int)jmin(y1, y2) - 4),
    (int)std::abs(x1 - x2) + 8,
    (int)std::abs(y1 - y2) + 8);

  if (newBounds != getBounds())
    setBounds(newBounds);
  else
    resized();

  repaint();

  x1 = x2;
}
