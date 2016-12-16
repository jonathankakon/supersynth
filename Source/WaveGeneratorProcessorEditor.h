/*
  ==============================================================================

    WaveGeneratorProcessorEditor.h
    Created: 7 Oct 2016 3:43:18pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef WAVEGENERATORPROCESSOREDITOR_H_INCLUDED
#define WAVEGENERATORPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveGeneratorProcessor.h"
#include "ProcessorEditorWithConnectors.h"


//==============================================================================
/**
 */
class WaveGeneratorProcessorEditor  : public AudioProcessorEditor, Slider::Listener, Button::Listener
{
public:
  WaveGeneratorProcessorEditor (WaveGeneratorProcessor* p, ProcessorEditorBase* b);
  ~WaveGeneratorProcessorEditor();
  
  //==============================================================================
  void paint (Graphics&) override;
  void resized() override;
  
  void sliderValueChanged(Slider* slider) override;
  void buttonClicked(Button* button) override;
  
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  WaveGeneratorProcessor& processor;
  ProcessorEditorBase& parent;
  
  ScopedPointer<Slider> frequencySlider;
  ScopedPointer<Slider> volumeSlider;
  ScopedPointer<Slider> octavesSlider;
  ScopedPointer<Slider> semitonesSlider;
  ScopedPointer<Slider> centsSlider;
  ScopedPointer<Slider> waveformSlider;
  ScopedPointer<Slider> phaseOffsetSlider;

  ScopedPointer<ToggleButton> midiToggle;
  
  Label volumeLabel;
  Label waveformLabel;
  Label frequencyLabel;
  Label octaveLabel;
  Label semitoneLabel;
  Label centLabel;
  Label phaseOffsetLabel;
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveGeneratorProcessorEditor)
};




#endif  // WAVEGENERATORPROCESSOREDITOR_H_INCLUDED
