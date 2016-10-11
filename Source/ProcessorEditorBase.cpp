/*
  ==============================================================================

    ProcessorEditorBase.cpp
    Created: 11 Oct 2016 2:46:30pm
    Author:  Jonathan

  ==============================================================================
*/

#include "ProcessorEditorBase.h"
#include "PluginEditor.h"

ProcessorEditorBase::ProcessorEditorBase(AudioProcessor* p)
: AudioProcessorEditor (p), processor (*p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(200, 100);
}


ProcessorEditorBase::~ProcessorEditorBase()
{
}

void ProcessorEditorBase::mouseDown(const MouseEvent& e)
{
	dragger.startDraggingComponent(this, e);
	beginDragAutoRepeat(100);
}

void ProcessorEditorBase::mouseDrag(const MouseEvent& e)
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

void ProcessorEditorBase::mouseUp(const MouseEvent& e)
{
	beginDragAutoRepeat(0);
}

void ProcessorEditorBase::mouseExit(const MouseEvent& e)
{
	findParentComponentOfClass<SupersynthAudioProcessorEditor>()->setViewPortDragScrolling(true);
}

void ProcessorEditorBase::mouseEnter(const MouseEvent& e)
{
	findParentComponentOfClass<SupersynthAudioProcessorEditor>()->setViewPortDragScrolling(false);	
}

