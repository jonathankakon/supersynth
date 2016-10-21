/*
  ==============================================================================

    OutputProcessorEditor.h
    Created: 21 Oct 2016 3:43:21pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef OUTPUTPROCESSOREDITOR_H_INCLUDED
#define OUTPUTPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "OutputProcessor.h"
#include "ProcessorEditorBase.h"

class OutputProcessorEditor: public ProcessorEditorBase
{
public:
  OutputProcessorEditor(OutputProcessor&);
};


#endif  // OUTPUTPROCESSOREDITOR_H_INCLUDED
