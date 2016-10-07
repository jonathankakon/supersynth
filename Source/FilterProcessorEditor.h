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

class FilterProcessorEditor  : public AudioProcessorEditor
{
public:
  FilterProcessorEditor (FilterProcessor&);
  ~FilterProcessorEditor();
  
  void resized() override;
  
  //==============================================================================
  
  
  
private:
  
  
  
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessorEditor)
};


#endif  // FILTERPROCESSOREDITOR_H_INCLUDED
