/*
  ==============================================================================

    FilterProcessorEditor.h
    Created: 7 Oct 2016 3:19:03pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#ifndef FILTERPROCESSOREDITOR_H_INCLUDED
#define FILTERPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterProcessor.h"
#include "ProcessorEditorWithConnectors.h"


class FilterProcessorEditor  : public AudioProcessorEditor
{
public:
  FilterProcessorEditor (FilterProcessor* p, ProcessorEditorBase* b);
  ~FilterProcessorEditor();
  //==============================================================================
  
  void paint(Graphics&) override;
  void resized() override;
  
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  FilterProcessor& processor;
  ProcessorEditorBase& parent;
  
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessorEditor)
};


#endif  // FILTERPROCESSOREDITOR_H_INCLUDED
