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
#include "RMSRequestable.h"

//==============================================================================
Worksheet::Worksheet(int width, int height) : tooltipWindow(new TooltipWindow(nullptr, 500)), popup(new PopupDisplay(*this))
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	setSize(width, height);
  startTimerHz(30);
}

Worksheet::~Worksheet()
{
  for (int i = connections.size()-1; i >= 0; --i)
  {
    clearEditorListeners(connections[i]);
    Connection* conn = connections.removeAndReturn(i);
    if(conn != nullptr)
    {
      delete conn;
    }
  }
}

void Worksheet::paint (Graphics& g)
{
	g.fillAll(Colour(0xFF2D2D30));

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

bool Worksheet::isInterestedInDragSource(const SourceDetails& /*dragSourceDetails*/)
{
  return true;
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

void Worksheet::componentMovedOrResized(Component& component, bool wasMoved, bool wasResized)
{
  findParentComponentOfClass<SupersynthAudioProcessorEditor>()->componentMovedOrResized(component, wasMoved, wasResized);
}

void Worksheet::timerCallback()
{
  
//  for (int i = editors.size() - 1; i >= 0; --i)
//  {
//    ProcessorEditorBase* editor = reinterpret_cast<ProcessorEditorBase*>(editors[i]);
//    if (editor != nullptr)
//    {
//      RMSRequestable& rmsProcessor = dynamic_cast<RMSRequestable&>(editor->getProcessor());
//
//      if (&rmsProcessor != nullptr)
//      {
//        float rms = rmsProcessor.getCurrentRMS();
//
//        for (int k = connections.size() - 1; k >= 0; --k)
//        {
//          if (connections[k]->outputNodeId == editor->getNodeId())
//            connections[k]->setPathColourFromRms(rms);
//        }
//      }
//    }
//  }
}

void Worksheet::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
{
  if(event.mods.isCtrlDown() || event.mods.isCommandDown())
  {
    zoomFactor *= (wheel.deltaY > 0) ? 1.1f: 0.9f;
    setTransform(AffineTransform().scaled(zoomFactor, zoomFactor));
    repaint();
  }
}

void Worksheet::addEditor(Component* editor)
{
  addEditor(editor, dropPosition.getX(), dropPosition.getY());
}

void Worksheet::addEditor(Component* editor, double x, double y)
{
  editors.add(editor);
  addAndMakeVisible(editor);
  Rectangle<int> r((int)x - editor->getWidth() / 2, (int)y - editor->getHeight() / 2, editor->getWidth(), editor->getHeight());
  editor->setBounds(r);
  editor->addComponentListener(this);
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

String Worksheet::getTooltipTextAt(const Point<int> point)
{
  String tooltip = String();
  for (Component* editor : editors)
  {
    tooltip = reinterpret_cast<ProcessorEditorBase*>(editor)->findConnectorTextAt(point.withX(point.x - editor->getX()).withY(point.y - editor->getY()));
    if (tooltip != "")
    {
      return tooltip;
    }
  }
  tooltip = (tooltip == "") ? " " : tooltip;
  return tooltip;
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
      if (findConnectorAt(draggingConnection->draggingToInput, x, y, connectorPosition, destId)
        && findParentComponentOfClass<SupersynthAudioProcessorEditor>()->testConnection(*draggingConnection, destId))
      {
        draggingConnection->dragEnd(connectorPosition.getX(), connectorPosition.getY());
        popup->updatePosition(getTooltipTextAt({ x,y }), connectorPosition);
        addAndMakeVisible(popup);
      }
      else
      {
        draggingConnection->dragEnd(x, y);
        removeChildComponent(popup);
      }
    }
    else
    {
      Point<int> connectorPosition{};
      if (findConnectorAt(draggingConnection->draggingToInput, x, y, connectorPosition, destId)
        && findParentComponentOfClass<SupersynthAudioProcessorEditor>()->testConnection(*draggingConnection, destId))
      {
        draggingConnection->dragStart(connectorPosition.getX(), connectorPosition.getY());
        popup->updatePosition(getTooltipTextAt({ x,y }), connectorPosition);
        addAndMakeVisible(popup);
      }
      else
      {
        draggingConnection->dragStart(x, y);
        removeChildComponent(popup);
      }
    }
  }
}

void Worksheet::endDraggingConnector(const MouseEvent& e)
{
  if (draggingConnection == nullptr)
    return;

  removeChildComponent(popup);
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
    newConnection->addComponentListener(this);
    draggingConnection.deleteAndZero();
  } else
  {
    findParentComponentOfClass<SupersynthAudioProcessorEditor>()->removeConnection(*draggingConnection);
    connections.remove(connections.indexOf(draggingConnection));
    draggingConnection.deleteAndZero();
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

void Worksheet::removeEditor(int nodeId)
{
  for(int i = editors.size() -1; i >= 0; --i )
  {
    if(reinterpret_cast<ProcessorEditorBase*>(editors[i])->getNodeId() == nodeId)
    {
      editors[i]->removeComponentListener(this);
      editors.remove(i, true);
    }
  }
}

void Worksheet::removeConnections(int nodeId, int mixerNodeId)
{
  for (int i = connections.size()-1; i >= 0; --i)
  {
    if (connections[i]->inputNodeId == nodeId || connections[i]->outputNodeId == nodeId 
      || connections[i]->inputNodeId == mixerNodeId || connections[i]->outputNodeId == mixerNodeId)
    {
      clearEditorListeners(connections[i]);
      Connection* conn = connections.removeAndReturn(i);
      delete conn;
    }
  }
}

void Worksheet::setZoomFactor(float zoom)
{
  zoomFactor = zoom;
  setTransform(AffineTransform().scaled(zoomFactor, zoomFactor));
  repaint();
}

float Worksheet::getZoomFactor() const
{
  return zoomFactor;
}

int Worksheet::getEditorX(int nodeId)
{
  for(Component* editor : editors)
  {
    if (reinterpret_cast<ProcessorEditorBase*>(editor)->getNodeId() == nodeId)
      return editor->getBoundsInParent().getX();
  }
  return 0;
}

int Worksheet::getEditorY(int nodeId)
{
  for (Component* editor : editors)
  {
    if (reinterpret_cast<ProcessorEditorBase*>(editor)->getNodeId() == nodeId)
      return editor->getBoundsInParent().getY();
  }
  return 0;
}

StringPairArray Worksheet::getInputAndChannelsOfEditor(int nodeId)
{
  for (Component* editor : editors)
  {
    if (reinterpret_cast<ProcessorEditorBase*>(editor)->getNodeId() == nodeId)
      return reinterpret_cast<ProcessorEditorBase*>(editor)->getMixerNodeConnectionIds();
  }
  return StringPairArray();
}

Rectangle<int> Worksheet::getConnectionRectangle(const int sourceId, const int destId, const int sourceChannel, const int destChannel)
{
  for(Connection* conn : connections)
  {
    if (conn->inputNodeId == destId && conn->inputNodeChannel == destChannel 
      && conn->outputNodeId == sourceId && conn->outputNodeChannel == sourceChannel)
      return Rectangle<int>(conn->getX1(), conn->getY1(), conn->getX2(), conn->getY2());
  }
  return Rectangle<int>();
}

void Worksheet::addConnection(int x0, int y0, int x1, int y1, int sourceNodeId, int sourceChannelId, int destNodeId, int destChannelId)
{
  Connection* newConnection = new Connection(destNodeId, destChannelId, sourceNodeId, sourceChannelId);

  newConnection->dragEnd(x0, y0);
  newConnection->dragStart(x1, y1);

  newConnection->toBack();
  connections.add(newConnection);
  addAndMakeVisible(newConnection);
  registerComponentListener(newConnection, newConnection->inputNodeId, newConnection->outputNodeId);
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
