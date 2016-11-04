/*
  ==============================================================================

    IOEditor.h
    Created: 1 Nov 2016 1:06:01pm
    Author:  Jonathan

  ==============================================================================
*/

#ifndef IOEDITOR_H_INCLUDED
#define IOEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "InternalIOProcessor.h"

class ProcessorEditorBase;

//==============================================================================
/*
*/
class IOEditor : public AudioProcessorEditor
{
public:
  IOEditor(InternalIOProcessor* p, ProcessorEditorBase* b);
  ~IOEditor();

  void paint(Graphics&) override;
  void resized() override;

private:
  InternalIOProcessor& processor;
  ProcessorEditorBase& parent;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IOEditor)
};


#endif  // IOEDITOR_H_INCLUDED
