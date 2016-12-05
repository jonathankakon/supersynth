/*
  ==============================================================================

    DelayProcessorEditor.h
    Created: 5 Dec 2016 4:46:24pm
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef DELAYPROCESSOREDITOR_H_INCLUDED
#define DELAYPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayProcessor.h"
#include "ProcessorEditorWithConnectors.h"


class DelayProcessorEditor : public AudioProcessorEditor, SliderListener
{
public:

  DelayProcessorEditor(DelayProcessor* p, ProcessorEditorBase* b);
  ~DelayProcessorEditor();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;

  void sliderValueChanged(Slider* slider) override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  DelayProcessor& processor;
  ProcessorEditorBase& parent;

  ScopedPointer<Slider> delaySlider;
  ScopedPointer<Slider> decaySlider;

  Label delayLabel;
  Label decayLabel;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayProcessorEditor)
};


#endif  // DELAYPROCESSOREDITOR_H_INCLUDED
