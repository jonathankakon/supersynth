/*
  ==============================================================================

    PolyWavegeneratorEditor.h
    Created: 9 Dec 2016 11:09:22am
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef POLYWAVEGENERATOREDITOR_H_INCLUDED
#define POLYWAVEGENERATOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PolyWavegeneratorProcessor.h"
#include "ProcessorEditorWithConnectors.h"


class PolyWavegeneratorEditor : public AudioProcessorEditor, SliderListener
{
public:
  
  PolyWavegeneratorEditor(PolyWavegeneratorProcessor* p, ProcessorEditorBase* b);
  ~PolyWavegeneratorEditor();
  
  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;
  
  void sliderValueChanged(Slider* slider) override;
  
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  PolyWavegeneratorProcessor& processor;
  ProcessorEditorBase& parent;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PolyWavegeneratorEditor)
};



#endif  // POLYWAVEGENERATOREDITOR_H_INCLUDED
