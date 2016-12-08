/*
  ==============================================================================

    ProcessorEditorWithConnectors.cpp
    Created: 29 Oct 2016 10:44:41am
    Author:  jonathan kakon

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProcessorEditorWithConnectors.h"
#include "IOEditor.h"
#include "InternalIOProcessor.h"
#include "InputConnectorProcessor.h"
#include "DelayProcessorEditor.h"

template<class AudioProcessorType, class EditorType>
ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::ProcessorEditorWithConnectors(AudioProcessorType * p) : ProcessorEditorBase(p),
processorEditor(new EditorType(p, this)), deleteButton(new TextButton("X", "Delete Module")), mixerNodeIds(Array<int>()), mixerConnectionIds(StringPairArray()), processor(*p)
{
  static_cast<Component*>(processorEditor)->addMouseListener(this, false);
  addAndMakeVisible(processorEditor);

  Rectangle<int> r(static_cast<Component*>(processorEditor)->getBounds());
  static_cast<Component*>(processorEditor)->setBounds(r.withX(r.getX() + 32));
  r.expand(32, 0);
  setSize(r.getWidth(), r.getHeight());
}

template<class AudioProcessorType, class EditorType>
ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::~ProcessorEditorWithConnectors()
{
}


template <class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::createDeleteButton(Rectangle<int> r)
{
  registerImmobileObject(*deleteButton);
  deleteButton->setBounds(r.withX(r.getWidth() - 32 - 15)
                           .withY(0)
                           .withWidth(15)
                           .withHeight(15));
  deleteButton->addListener(this);
  addAndMakeVisible(deleteButton);
}

template <>
void ProcessorEditorWithConnectors<InternalIOProcessor, IOEditor>::createDeleteButton(Rectangle<int> r)
{
}

template<class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::setConnectors()
{
  int numInputs = processor.getBusCount(true);
  int numOutputs = processor.getBusCount(false);

  Rectangle<int> r(getLocalBounds());

  for (int i = 0; i < numInputs; ++i)
  {
    InputConnectorProcessor* inputMixer = new InputConnectorProcessor();
    int mixerId = addProcessorToGraph(inputMixer, nodeId, i);
    InputConnector* input = new InputConnector(inputMixer, this, mixerId);
    mixerNodeIds.add(mixerId);
    mixerConnectionIds.set(String(mixerId), String(i));
    inputConnectors.add(input);
    registerImmobileObject(*input);
    addAndMakeVisible(input);
    input->setTooltip(processor.getBus(true, i)->getName());
    
    int totHeight = numInputs * input->getHeight();

    input->setBounds(r.withY(r.getHeight() / 2 - totHeight / 2 + i*input->getHeight())
      .withWidth(input->getWidth())
      .withHeight(input->getHeight()));
  }

  if (numOutputs > 0)
  {
    outputConnector = new OutputConnector(nodeId);
    registerImmobileObject(*outputConnector);
    addAndMakeVisible(outputConnector);
    outputConnector->setTooltip(processor.getBus(false, 0)->getName());
    outputConnector->setBounds(r.withX(r.getWidth() - 32)
      .withY(r.getHeight() / 2 - outputConnector->getHeight() / 2)
      .withWidth(outputConnector->getWidth())
      .withHeight(outputConnector->getHeight()));
  }

  createDeleteButton(r);
}

template <class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::setConnector(AudioProcessor* oldInput, const int mixerId, const int connectedTo)
{
  int numInputs = processor.getBusCount(true);
  Rectangle<int> r(getLocalBounds());

  InputConnectorProcessor* inputMixer;
  if(oldInput == nullptr)
  {
    inputMixer = new InputConnectorProcessor();
    addProcessorToGraph(inputMixer, nodeId, mixerId, connectedTo);
  }
  else
  {
    inputMixer = reinterpret_cast<InputConnectorProcessor*>(oldInput);
  }

  InputConnector* input = new InputConnector(inputMixer, this, mixerId);
  mixerNodeIds.add(mixerId);
  mixerConnectionIds.set(String(mixerId), String(connectedTo));
  inputConnectors.add(input);
  input->setTooltip(processor.getBus(true, (inputConnectors.size() - 1))->getName());
  registerImmobileObject(*input);
  addAndMakeVisible(input);
  
  int totHeight = numInputs * input->getHeight();

  input->setBounds(r.withY(r.getHeight() / 2 - totHeight / 2 + (inputConnectors.size() - 1)*input->getHeight())
    .withWidth(input->getWidth())
    .withHeight(input->getHeight()));
}

template <class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::setOutputConnectors()
{
  int numOutputs = processor.getBusCount(false);
  Rectangle<int> r(getLocalBounds());

  if (numOutputs > 0)
  {
    outputConnector = new OutputConnector(nodeId);
    registerImmobileObject(*outputConnector);
    addAndMakeVisible(outputConnector);
    outputConnector->setTooltip(processor.getBus(false, 0)->getName());
    outputConnector->setBounds(r.withX(r.getWidth() - 32)
      .withY(r.getHeight() / 2 - outputConnector->getHeight() / 2)
      .withWidth(outputConnector->getWidth())
      .withHeight(outputConnector->getHeight()));
  }
  createDeleteButton(r);
}

template <class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::registerNodeConnectionListener(Connection* connection, int inputNodeId, int outputNodeId)
{
  for(InputConnector* connector : inputConnectors)
  {
    if (connector->getNodeId() == inputNodeId)
      this->addComponentListener(connection);
  }

  if (outputConnector->getNodeId() == outputNodeId)
    this->addComponentListener(connection);

}

template <class AudioProcessorType, class EditorType>
bool ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::hasInputWithId(int inputNodeId, int& x, int& y)
{
  for (InputConnector* connector : inputConnectors)
  {
    if (connector->getNodeId() == inputNodeId)
    {
      Point<int> point = connector->getClosestConnector(0, 0);
      x = point.getX() + connector->getX();
      y = point.getY() + connector->getY();
      return true;
    }
  }
  return false;
}

template <class AudioProcessorType, class EditorType>
bool ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::hasOutputWithId(int outputNodeId, int& x, int& y)
{
  if (outputConnector->getNodeId() == outputNodeId)
  {
    Point<int> point = outputConnector->getClosestConnector(0, 0);
    x = point.getX() + outputConnector->getX();
    y = point.getY() + outputConnector->getY();
    return true;
  }
  return false;
}

template <class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::buttonClicked(Button* button)
{
  if(button == deleteButton)
  {
    removeProcessor(nodeId, mixerNodeIds);
  }
}

template<class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::paint (Graphics& /*g*/)
{
}

template<class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::resized()
{
}

template class ProcessorEditorWithConnectors<WaveGeneratorProcessor, WaveGeneratorProcessorEditor>;
template class ProcessorEditorWithConnectors<FilterProcessor, FilterProcessorEditor>;
template class ProcessorEditorWithConnectors<InternalIOProcessor, IOEditor>;
template class ProcessorEditorWithConnectors<EQFourProcessor, EQFourProcessorEditor>;
template class ProcessorEditorWithConnectors<EnvelopeProcessor, EnvelopeProcessorEditor>;
template class ProcessorEditorWithConnectors<NoiseGeneratorProcessor, NoiseGeneratorProcessorEditor>;
template class ProcessorEditorWithConnectors<DistortionProcessor, DistortionProcessorEditor>;
template class ProcessorEditorWithConnectors<DelayProcessor, DelayProcessorEditor>;

