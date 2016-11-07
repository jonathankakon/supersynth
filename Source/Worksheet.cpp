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
#include "ProcessorEditorBase.h"

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
		g.drawLine(Line<float>(static_cast<float>(x), static_cast<float>(r.getY()), static_cast<float>(x), static_cast<float>(r.getBottom())), 0.3f);
	}

	for (int y = 0; y < getHeight(); y += 50)
	{
		g.drawLine(Line<float>(static_cast<float>(r.getX()), static_cast<float>(y), static_cast<float>(r.getRight()), static_cast<float>(y)), 0.3f);
	}
}

void Worksheet::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool Worksheet::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
  return true;
  if (dragSourceDetails.description.hasSameTypeAs(new var(new ToolboxComponent::ModulesListElement("", "", nullptr))));
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

		viewport->autoScroll(relativeX, relativeY, 50, 7);	// ... based on the displayed area, paint just what's visible ... //
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

	findParentComponentOfClass<SupersynthAudioProcessorEditor>()->addAudioProcessor(static_cast<ToolboxComponent::ModulesListElement*> (dragSourceDetails.description.getObject()));

	somethingIsBeingDraggedOver = false;
	beginDragAutoRepeat(0);
	repaint();
}

void Worksheet::addEditor(Component* editor)
{
  addEditor(editor, dropPosition.getX(), dropPosition.getY());
}

void Worksheet::addEditor(Component* editor, double x, double y)
{
  editors.add(editor);
  addAndMakeVisible(editor);
  Rectangle<int>r(x - editor->getWidth() / 2, y - editor->getHeight() / 2, editor->getWidth(), editor->getHeight());
  editor->setBounds(r);
}

void Worksheet::beginConnectorDrag(int outputNodeId, int outputChannel, int inputNodeId, int inputChannel, const MouseEvent& e)
{
  draggingConnection = dynamic_cast<Connection*> (e.originalComponent);

  if (draggingConnection == nullptr)
  {
    const MouseEvent e2(e.getEventRelativeTo(this));
    draggingConnection = new Connection(inputNodeId, inputChannel, outputNodeId, outputChannel);
    Point<int> connectorPosition{};
    int nodeId = 0;
    if (findConnectorAt(!draggingConnection->draggingToInput, e2.x, e2.y, connectorPosition, nodeId))
      draggingConnection->setFixedConnectorPosition(connectorPosition);
  }
  else
  {
    if(SupersynthAudioProcessorEditor* mainProcessor = findParentComponentOfClass<SupersynthAudioProcessorEditor>())
    {
      mainProcessor->setViewPortDragScrolling(false);
      mainProcessor->removeConnection(*draggingConnection);
    }
  }

  addAndMakeVisible(draggingConnection);
  draggingConnection->toFront(false);

  dragConnector(e);
}

void Worksheet::dragConnector(const MouseEvent& e)
{
  const MouseEvent e2(e.getEventRelativeTo(this));

  if(draggingConnection != nullptr)
  {
    int x = e2.x;
    int y = e2.y;
    int destId = 0;

    //findPin, updateConnection, check if can connect, snap to pin.

    if(draggingConnection->draggingToInput)
    {
      Point<int> connectorPosition{};
      if(findConnectorAt(draggingConnection->draggingToInput, x, y, connectorPosition, destId)
            && findParentComponentOfClass<SupersynthAudioProcessorEditor>()->testConnection(*draggingConnection, destId))
        draggingConnection->dragEnd(connectorPosition.getX(), connectorPosition.getY());
      else
        draggingConnection->dragEnd(x, y);
    }
    else
    {
      Point<int> connectorPosition{};
      if (findConnectorAt(draggingConnection->draggingToInput, x, y, connectorPosition, destId)
            && findParentComponentOfClass<SupersynthAudioProcessorEditor>()->testConnection(*draggingConnection, destId))
        draggingConnection->dragStart(connectorPosition.getX(), connectorPosition.getY());
      else
        draggingConnection->dragStart(x, y);
    }
  }
}

void Worksheet::endDraggingConnector(const MouseEvent& e)
{
  if (draggingConnection == nullptr)
    return;

  const MouseEvent e2(e.getEventRelativeTo(this));

  Point<int> connectorPosition{};
  int nodeId;
  if(findConnectorAt(draggingConnection->draggingToInput, e2.x, e2.y, connectorPosition, nodeId)) // hasPin and is valid
  {
    Connection* newConnection = new Connection(*draggingConnection);
    newConnection->setDestNodeId(nodeId);
    connections.add(newConnection);
    addAndMakeVisible(newConnection);
    registerComponentListener(newConnection, newConnection->inputNodeId, newConnection->outputNodeId);
    findParentComponentOfClass<SupersynthAudioProcessorEditor>()->addConnection(newConnection);
    draggingConnection = nullptr;
  } else
  {
    findParentComponentOfClass<SupersynthAudioProcessorEditor>()->removeConnection(*draggingConnection);
    connections.removeObject(draggingConnection, false);
    draggingConnection = nullptr;
  }
}


bool Worksheet::findConnectorAt(const bool isInput, const int x, const int y, Point<int>& outPosition, int& nodeId) const
{
  for (Component* editor : editors)
  {
    if (reinterpret_cast<ProcessorEditorBase*>(editor)->findConnectorAt(isInput, x - editor->getX(), y - editor->getY(), outPosition, nodeId))
    {
      outPosition.addXY(editor->getX(), editor->getY());
      return true;
    }
  }  
  return false;
}

void Worksheet::clearEditorListeners(Connection* connection)
{
  for(Component* editor : editors)
  {
    editor->removeComponentListener(connection);
  }
}

void Worksheet::registerComponentListener(Connection* connection, int inputNodeId, int outputNodeId)
{
  for (Component* editor : editors)
  {
    reinterpret_cast<ProcessorEditorBase*>(editor)->registerNodeConnectionListener(connection, inputNodeId, outputNodeId);
  }
}
