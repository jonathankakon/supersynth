/*
 ==============================================================================
  
  EnvelopeProcessorEditor.h
  Created: 18 Nov 2016 2:22:45pm
  Author:  Kian Hunziker
  
  ==============================================================================
  */

#ifndef ENVELOPEPROCESSOREDITOR_H_INCLUDED
#define ENVELOPEPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProcessorEditorWithConnectors.h"
#include "EnvelopeProcessor.h"

class EnvelopeProcessorEditor: public AudioProcessorEditor, Slider::Listener
{
  public:
    EnvelopeProcessorEditor(EnvelopeProcessor* p, ProcessorEditorBase* b);
    ~EnvelopeProcessorEditor();
    
    void paint(Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider* slider) override;
    
    
    
  private:
    EnvelopeProcessor& processor;
    ProcessorEditorBase& parent;
    
    
    ScopedPointer<Slider> attackSlider;
    ScopedPointer<Slider> decaySlider;
    ScopedPointer<Slider> sustainSlider;
    ScopedPointer<Slider> releaseSlider;
    
    
  };



#endif  // ENVELOPEPROCESSOREDITOR_H_INCLUDED
