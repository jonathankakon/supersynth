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
#include "EQFourProcessor.h"
#include "EQFourProcessorEditor.h"
#include "EnvelopeProcessor.h"
#include "EnvelopeProcessorEditor.h"
#include "NoiseGeneratorProcessorEditor.h"

#include "InputConnector.h"
#include "OutputConnector.h"

//==============================================================================
/*
*/
template<class AudioProcessorType, class EditorType>
class ProcessorEditorWithConnectors : public ProcessorEditorBase, public ButtonListener
{
public:
  explicit ProcessorEditorWithConnectors(AudioProcessorType* processor);
  ~ProcessorEditorWithConnectors();
  
  void createDeleteButton(Rectangle<int> r);
  //==============================================================================
  void setConnectors() override; 
  void setConnector(AudioProcessor* oldInput, const int nodeId, const int connectedTo) override;
  void setOutputConnectors() override;
  void registerNodeConnectionListener(Connection* connection, int input_node_id, int output_node_id) override; 
  bool hasInputWithId(int inputNodeId, int& x, int& y) override;
  bool hasOutputWithId(int outputNodeId, int& x, int& y) override;

  void buttonClicked(Button* button) override;

  void setNodeId(int id) override { nodeId = id; };
  int getNodeId() override { return nodeId; };
  Array<int> getMixerNodeIds() override { return mixerNodeIds; }; 
  StringPairArray getMixerNodeConnectionIds() override { return mixerConnectionIds; };

  void resized() override;
  void paint(Graphics& g) override;

private:

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  int numControlSignals;
  bool takesGateSignal;
  bool takesAudioSignal;

  int nodeId;
  Array<int> mixerNodeIds;
  StringPairArray mixerConnectionIds;

  OwnedArray<InputConnector> inputConnectors;
  ScopedPointer<OutputConnector> outputConnector;
  ScopedPointer<Button> deleteButton;

  ScopedPointer<EditorType> processorEditor;

  AudioProcessorType& processor;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditorWithConnectors)
};


#endif  // PROCESSOREDITORWITHCONNECTORS_H_INCLUDED
