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


class FilterProcessorEditor  : public AudioProcessorEditor, Slider::Listener, ToggleButton::Listener, ComboBox::Listener
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
  void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  FilterProcessor& processor;
  ProcessorEditorBase& parent;
  
  ScopedPointer<Slider> frequencySlider;
  ScopedPointer<Slider> qSlider;
  ScopedPointer<Slider> gainSlider;
  //ScopedPointer<Slider> filterTypeSlider;
  
  ScopedPointer<ComboBox> dropdownMenuFilterTypes;
  
  int numButtons = 9; //number of Buttons
  
  ScopedPointer<ToggleButton> buttonOnOff;
  ScopedPointer<ToggleButton> button0;
  ScopedPointer<ToggleButton> button1;
  ScopedPointer<ToggleButton> button2; //not active
  ScopedPointer<ToggleButton> button3; //not active
  ScopedPointer<ToggleButton> button4;
  ScopedPointer<ToggleButton> button5;
  ScopedPointer<ToggleButton> button6;
  ScopedPointer<ToggleButton> button7;
  ScopedPointer<ToggleButton> button8;
  ScopedPointer<ToggleButton> button9;
  
  Label frequencyLabel;
  Label qLabel;
  Label gainLabel;

  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessorEditor)
};


#endif  // FILTERPROCESSOREDITOR_H_INCLUDED
