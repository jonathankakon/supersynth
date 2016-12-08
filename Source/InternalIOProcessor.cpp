/*
  ==============================================================================

    InternalIOProcessor.cpp
    Created: 1 Nov 2016 12:32:38pm
    Author:  Jonathan

  ==============================================================================
*/

#include "InternalIOProcessor.h"
#include "ProcessorEditorWithConnectors.h"
#include "IOEditor.h"


InternalIOProcessor::InternalIOProcessor(IODeviceType type) : AudioGraphIOProcessor(type), IOType(type)
{
}

InternalIOProcessor::~InternalIOProcessor()
{
}

AudioProcessorEditor* InternalIOProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<InternalIOProcessor, IOEditor>(this);
}

bool InternalIOProcessor::hasEditor() const
{
  return true;
}

const String InternalIOProcessor::getName() const
{
  return "InternalIOProcessor";
}
