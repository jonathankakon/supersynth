/*
  ==============================================================================

    EQFourProcessorEditor.h
    Created: 2 Nov 2016 4:22:13pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#ifndef EQFOURPROCESSOREDITOR_H_INCLUDED
#define EQFOURPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProcessorEditorWithConnectors.h"
#include "EQFourProcessor.h"


class EQFourProcessorEditor: public AudioProcessorEditor, Slider::Listener, ToggleButton::Listener
{
public:
  EQFourProcessorEditor(EQFourProcessor* p, ProcessorEditorBase* b);
  ~EQFourProcessorEditor();
  
  void paint(Graphics&) override;
  void resized() override;
  
  void sliderValueChanged(Slider* slider) override;
  void buttonClicked(Button* button) override;
  void buttonStateChanged(Button* button) override;
  
  void initialiseSlider(Slider* slider, AudioParameterFloat* param, double min, double max, double stepSize, double initialValue);

  
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  EQFourProcessor&  processor;
  ProcessorEditorBase& parent;
  
  
  ScopedPointer<Slider> cutoffFreqBand1;
  ScopedPointer<Slider> cutoffFreqBand2;
  ScopedPointer<Slider> cutoffFreqBand3;
  ScopedPointer<Slider> cutoffFreqBand4;
  
  ScopedPointer<Slider> qParamBand1;
  ScopedPointer<Slider> qParamBand2;
  ScopedPointer<Slider> qParamBand3;
  ScopedPointer<Slider> qParamBand4;
  
  ScopedPointer<Slider> gainBand1;
  ScopedPointer<Slider> gainBand2;
  ScopedPointer<Slider> gainBand3;
  ScopedPointer<Slider> gainBand4;
  
  ScopedPointer<ToggleButton> buttonLowCut1;
  ScopedPointer<ToggleButton> buttonLowCut2;
  ScopedPointer<ToggleButton> buttonLowShelf;
  
  ScopedPointer<ToggleButton> buttonBandpass2;
  ScopedPointer<ToggleButton> buttonNotch2;
  
  ScopedPointer<ToggleButton> buttonBandpass3;
  ScopedPointer<ToggleButton> buttonNotch3;
  
  ScopedPointer<ToggleButton> buttonHighCut1;
  ScopedPointer<ToggleButton> buttonHighCut2;
  ScopedPointer<ToggleButton> buttonHighShelf;
  
  Label frequencyLabel;
  Label qLabel;
  Label gainLabel;
  
  
};



#endif  // EQFOURPROCESSOREDITOR_H_INCLUDED
