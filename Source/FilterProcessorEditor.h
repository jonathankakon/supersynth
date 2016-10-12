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
#include "ProcessorEditorBase.h"

class FilterProcessorEditor  : public ProcessorEditorBase
{
public:
  FilterProcessorEditor (FilterProcessor&);
  ~FilterProcessorEditor();
  //==============================================================================
  
  void paint(Graphics&) override;
  void resized() override;
  
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  FilterProcessor& processor;
  
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessorEditor)
};


#endif  // FILTERPROCESSOREDITOR_H_INCLUDED
