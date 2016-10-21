/*
  ==============================================================================

    ProcessorEditorBase.cpp
    Created: 11 Oct 2016 2:46:30pm
    Author:  Jonathan

  ==============================================================================
*/

#include "ProcessorEditorBase.h"
#include "PluginEditor.h"

ProcessorEditorBase::ProcessorEditorBase(AudioProcessor* p, bool hasAudioInput, bool hasControlInput, bool hasGateInput)
: AudioProcessorEditor (p), processor (*p), dragStop(new DragStopHelper(*this)),
  takesAudioSignal(hasAudioInput), takesControlSignal(takesControlSignal), takesGateSignal(hasGateInput),
  inputConnector(new InputConnector()), outputConnector(new OutputConnector())
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.

  registerImmobileObject(*inputConnector);
  addAndMakeVisible(inputConnector);

  registerImmobileObject(*outputConnector);
  addAndMakeVisible(outputConnector);
}


ProcessorEditorBase::~ProcessorEditorBase()
{
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

      viewport->autoScroll(relativeX, relativeY, 50, 7);																																		  // ... based on the displayed area, paint just what's visible ... //
    }
  }
}

void ProcessorEditorBase::mouseUp(const MouseEvent& e)
{
	beginDragAutoRepeat(0);
}

void ProcessorEditorBase::setViewPortDragging(bool enableDragging)
{
  findParentComponentOfClass<SupersynthAudioProcessorEditor>()->setViewPortDragScrolling(enableDragging);
}

void ProcessorEditorBase::setComponentDragging(bool enableDragging)
{
  draggingEnabled = enableDragging;
}

void ProcessorEditorBase::registerImmobileObject(Component & component)
{
  component.addMouseListener(dragStop, false);
}

void ProcessorEditorBase::setConnectors()
{
  Rectangle<int> r(getLocalBounds());

  inputConnector->setBounds(r.withWidth(20).withHeight(20).withY(r.getHeight() / 2 - 10));
  outputConnector->setBounds(r.withWidth(20).withHeight(20).withX(r.getWidth() - 20).withY(r.getHeight() / 2 - 10));
}

void ProcessorEditorBase::mouseExit(const MouseEvent& e)
{
  setViewPortDragging(true);
}

void ProcessorEditorBase::mouseEnter(const MouseEvent& e)
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

void ProcessorEditorBase::DragStopHelper::mouseDown(const MouseEvent & e)
{
}

void ProcessorEditorBase::DragStopHelper::mouseDrag(const MouseEvent & e)
{
}

void ProcessorEditorBase::DragStopHelper::mouseEnter(const MouseEvent & e)
{
  owner.setViewPortDragging(false);
  owner.setComponentDragging(false);
}

void ProcessorEditorBase::DragStopHelper::mouseExit(const MouseEvent & e)
{
  owner.setViewPortDragging(true);
  owner.setComponentDragging(true);
}

void ProcessorEditorBase::DragStopHelper::mouseUp(const MouseEvent & e)
{
}
