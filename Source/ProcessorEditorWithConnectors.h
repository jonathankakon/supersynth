/*
  ==============================================================================

    ProcessorEditorWithConnectors.h
    Created: 29 Oct 2016 10:44:41am
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef PROCESSOREDITORWITHCONNECTORS_H_INCLUDED
#define PROCESSOREDITORWITHCONNECTORS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProcessorEditorBase.h"
#include "WaveGeneratorProcessorEditor.h"
#include "FilterProcessorEditor.h"

#include "InputConnector.h"
#include "OutputConnector.h"

//==============================================================================
/*
*/
template<class AudioProcessorType, class EditorType>
class ProcessorEditorWithConnectors : public ProcessorEditorBase
{
public:
  explicit ProcessorEditorWithConnectors(AudioProcessorType* processor);
  ~ProcessorEditorWithConnectors();

  //==============================================================================
  void setConnectors() override;

  void resized() override;
  void paint(Graphics& g) override;

private:

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  int numControlSignals;
  bool takesGateSignal;
  bool takesAudioSignal;

  ScopedPointer<InputConnector> audioInputConnector;
  OwnedArray<InputConnector> controlInputConnectors;
  ScopedPointer<OutputConnector> outputConnector;

  ScopedPointer<EditorType> processorEditor;

  AudioProcessorType& processor;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditorWithConnectors)
};


#endif  // PROCESSOREDITORWITHCONNECTORS_H_INCLUDED
