/*
  ==============================================================================

    ProcessorEditorBase.cpp
    Created: 11 Oct 2016 2:46:30pm
    Author:  Jonathan

  ==============================================================================
*/

#include "ProcessorEditorBase.h"
#include "PluginEditor.h"


ProcessorEditorBase::ProcessorEditorBase(AudioProcessor * p)
  : AudioProcessorEditor(p), draggingEnabled(true), processor(*p), dragStop(new DragStopHelper(*this))
{
}

ProcessorEditorBase::~ProcessorEditorBase()
{
}

bool ProcessorEditorBase::findConnectorAt(const bool isInput, int x, int y, Point<int>& outPosition, int& nodeId)
{
    Component* componentAtPos = getComponentAt(x, y);
    if(InputConnector* in = dynamic_cast<InputConnector*>(componentAtPos))
    {
      outPosition = in->getClosestConnector(x - in->getX(), y - in->getY());
      outPosition.setX(outPosition.getX() + in->getX());
      outPosition.setY(outPosition.getY() + in->getY());
      nodeId = in->getNodeId();
      return isInput; 
    }
    else if (OutputConnector* out = dynamic_cast<OutputConnector*>(componentAtPos))
    {
      outPosition = out->getClosestConnector(x - out->getX(), y - out->getY());
      outPosition.setX(outPosition.getX() + out->getX());
      outPosition.setY(outPosition.getY() + out->getY());
      nodeId = out->getNodeId();
      return !isInput;
    }
    return false;
}

void ProcessorEditorBase::mouseDown(const MouseEvent& e)
{
  if (draggingEnabled)
  {
    dragger.startDraggingComponent(this, e);
    beginDragAutoRepeat(100);
  }
}

void ProcessorEditorBase::mouseDrag(const MouseEvent& e)
{
  if (draggingEnabled)
  {
    dragger.dragComponent(this, e, nullptr);

    Viewport* const viewport = findParentComponentOfClass<Viewport>(); //Get the parent viewport
    if (viewport != nullptr) //Check for nullness
    {
      int x = viewport->getViewPositionX();
      int y = viewport->getViewPositionY();

      int relativeX = getX() + e.x - x;
      int relativeY = getY() + e.y - y;

      viewport->autoScroll(relativeX, relativeY, 50, 7);
    }
  }
}

void ProcessorEditorBase::mouseUp(const MouseEvent&)
{
	beginDragAutoRepeat(0);
}


void ProcessorEditorBase::setViewPortDragging(bool enableDragging) const
{
  findParentComponentOfClass<SupersynthAudioProcessorEditor>()->setViewPortDragScrolling(enableDragging);
}


void ProcessorEditorBase::setComponentDragging(bool enableDragging)
{
  draggingEnabled = enableDragging;
}

int ProcessorEditorBase::addProcessorToGraph(AudioProcessor* processor, int nodeIdToConnect, int channelNumberToConnect) const
{
  return findParentComponentOfClass<SupersynthAudioProcessorEditor>()->addAudioProcessor(processor, nodeIdToConnect, channelNumberToConnect);
}

void ProcessorEditorBase::removeProcessor(int nodeId, Array<int> mixerNodeIds) const
{
  findParentComponentOfClass<SupersynthAudioProcessorEditor>()->removeAudioProcessor(nodeId, mixerNodeIds);
}

void ProcessorEditorBase::registerImmobileObject(Component & component) const
{
  component.addMouseListener(dragStop, false);
}

void ProcessorEditorBase::mouseExit(const MouseEvent&)
{
  setViewPortDragging(true);
}

void ProcessorEditorBase::mouseEnter(const MouseEvent&)
{
  setViewPortDragging(false);
}


//DragStopHelper to stop dragging of components in lower levels
ProcessorEditorBase::DragStopHelper::DragStopHelper(ProcessorEditorBase& baseEditor) : owner(baseEditor)
{
}

ProcessorEditorBase::DragStopHelper::~DragStopHelper()
{
}

void ProcessorEditorBase::DragStopHelper::mouseDown(const MouseEvent &)
{
}

void ProcessorEditorBase::DragStopHelper::mouseDrag(const MouseEvent &)
{
}

void ProcessorEditorBase::DragStopHelper::mouseEnter(const MouseEvent &)
{
  owner.setViewPortDragging(false);
  owner.setComponentDragging(false);
}

void ProcessorEditorBase::DragStopHelper::mouseExit(const MouseEvent &)
{
  owner.setViewPortDragging(true);
  owner.setComponentDragging(true);
}

void ProcessorEditorBase::DragStopHelper::mouseUp(const MouseEvent &)
{
}

