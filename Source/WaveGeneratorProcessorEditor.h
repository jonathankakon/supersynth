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
#include "ProcessorEditorBase.h"


//==============================================================================
/**
 */
class WaveGeneratorProcessorEditor  : public ProcessorEditorBase, Slider::Listener, MouseListener
{
public:
  WaveGeneratorProcessorEditor (WaveGeneratorProcessor&);
  ~WaveGeneratorProcessorEditor();
  
  //==============================================================================
  void paint (Graphics&) override;
  void resized() override;
  
  void sliderValueChanged(Slider* slider) override;
  
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  WaveGeneratorProcessor& processor;
  
  Slider* frequencySlider;
  Slider* volumeSlider;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveGeneratorProcessorEditor)
};




#endif  // WAVEGENERATORPROCESSOREDITOR_H_INCLUDED
