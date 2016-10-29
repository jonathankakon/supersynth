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


class FilterProcessorEditor  : public ProcessorEditorBase, Slider::Listener, ToggleButton::Listener
{
public:
  FilterProcessorEditor (FilterProcessor* p, ProcessorEditorBase* b);
  ~FilterProcessorEditor();
  //==============================================================================
  
  void paint(Graphics&) override;
  void resized() override;
  
  void sliderValueChanged(Slider* slider) override;
  void buttonClicked(Button* button) override;
  void buttonStateChanged(Button* button) override;
  
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  FilterProcessor& processor;
  ProcessorEditorBase& parent;
  
  ScopedPointer<Slider> frequencySlider;
  ScopedPointer<Slider> qSlider;
  //ScopedPointer<Slider> filterTypeSlider;
  
  int numButtons = 9; //number of Buttons
  
  ScopedPointer<ToggleButton> buttonOnOff;
  ScopedPointer<ToggleButton> button0;
  ScopedPointer<ToggleButton> button1;
  ScopedPointer<ToggleButton> button2;
  ScopedPointer<ToggleButton> button3;
  ScopedPointer<ToggleButton> button4;
  ScopedPointer<ToggleButton> button5;
  ScopedPointer<ToggleButton> button6;
  ScopedPointer<ToggleButton> button7;

  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessorEditor)
};


#endif  // FILTERPROCESSOREDITOR_H_INCLUDED
