/*
  ==============================================================================

    DistortionProcessorEditor.h
    Created: 2 Dec 2016 1:09:30pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef DISTORTIONPROCESSOREDITOR_H_INCLUDED
#define DISTORTIONPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "DistortionProcessor.h"
#include "ProcessorEditorWithConnectors.h"

class DistortionProcessorEditor  : public AudioProcessorEditor, Slider::Listener
{
public:
  DistortionProcessorEditor (DistortionProcessor* p, ProcessorEditorBase* b);
  ~DistortionProcessorEditor();
  
  //==============================================================================
  void paint (Graphics&) override;
  void resized() override;
  
  void sliderValueChanged(Slider* slider) override;
  
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  DistortionProcessor& processor;
  ProcessorEditorBase& parent;
  
  ScopedPointer<Slider> preGainSlider;
  ScopedPointer<Slider> distortionTypeSlider;
  ScopedPointer<Slider> postGainSlider;
  
  Label preGainLabel;
  Label typeLabel;
  Label postGainLabel;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionProcessorEditor)
};



#endif  // DISTORTIONPROCESSOREDITOR_H_INCLUDED
