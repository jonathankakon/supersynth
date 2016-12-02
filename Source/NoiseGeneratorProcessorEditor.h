/*
  ==============================================================================

    NoiseGeneratorProcessorEditor.h
    Created: 27 Nov 2016 8:04:13pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef NOISEGENERATORPROCESSOREDITOR_H_INCLUDED
#define NOISEGENERATORPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "NoiseGeneratorProcessor.h"
#include "ProcessorEditorWithConnectors.h"

class NoiseGeneratorProcessorEditor  : public AudioProcessorEditor, SliderListener, ComboBox::Listener
{
public:
  
  NoiseGeneratorProcessorEditor (NoiseGeneratorProcessor* p, ProcessorEditorBase* b);
  ~NoiseGeneratorProcessorEditor();
  
  //==============================================================================
  void paint (Graphics&) override;
  void resized() override;
  
  void sliderValueChanged(Slider* slider) override;
  void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  NoiseGeneratorProcessor& processor;
  ProcessorEditorBase& parent;
  
  ScopedPointer<Slider> volumeSlider;
  ScopedPointer<Slider> noisetypeSlider;
  ScopedPointer<ComboBox> dropdownMenuNoiseTypes;

  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoiseGeneratorProcessorEditor)
};



#endif  // NOISEGENERATORPROCESSOREDITOR_H_INCLUDED
