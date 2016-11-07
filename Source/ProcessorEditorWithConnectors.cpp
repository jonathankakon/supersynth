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

template<class AudioProcessorType, class EditorType>
ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::ProcessorEditorWithConnectors(AudioProcessorType * p) : ProcessorEditorBase(p),
processorEditor(new EditorType(p, this)), processor(*p)
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


template<class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::setConnectors()
{
  InputConnectorProcessor* inputMixer = new InputConnectorProcessor();
  mixerNodeId = addProcessorToGraph(inputMixer, nodeId, 0);
  InputConnector* input = new InputConnector(inputMixer, this, mixerNodeId);
  inputConnectors.add(input);
  outputConnector = new OutputConnector(nodeId);

  registerImmobileObject(*input);
  addAndMakeVisible(input);

  registerImmobileObject(*outputConnector);
  addAndMakeVisible(outputConnector);

  Rectangle<int> r(getLocalBounds());

  input->setBounds(r.withY(r.getHeight() / 2 - input->getHeight() / 2)
                                  .withWidth(input->getWidth())
                                  .withHeight(input->getHeight()));
  outputConnector->setBounds(r.withX(r.getWidth() - 32)
                              .withY(r.getHeight() / 2 - outputConnector->getHeight()/2)
                              .withWidth(outputConnector->getWidth())
                              .withHeight(outputConnector->getHeight()));
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

template<class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::paint (Graphics& g)
{
}

template<class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::resized()
{
  if (processorEditor != nullptr && inputConnectors.size() > 0 && outputConnector != nullptr)
  {
    Rectangle<int> r(static_cast<Component*>(processorEditor)->getBounds());
  
    static_cast<Component*>(processorEditor)->setBounds(r.withX(r.getX() + 32));
    r.expand(32, 0);

    setSize(r.getWidth(), r.getHeight());

    r = getLocalBounds();

    InputConnector * in = inputConnectors.getFirst();
    in->setBounds(r.withY(r.getHeight() / 2 - in->getHeight() / 2)
      .withWidth(in->getWidth())
      .withHeight(in->getHeight()));
    outputConnector->setBounds(r.withX(r.getWidth() - 32)
      .withY(r.getHeight() / 2 - outputConnector->getHeight() / 2)
      .withWidth(outputConnector->getWidth())
      .withHeight(outputConnector->getHeight()));
  }
}

template class ProcessorEditorWithConnectors<WaveGeneratorProcessor, WaveGeneratorProcessorEditor>;
template class ProcessorEditorWithConnectors<FilterProcessor, FilterProcessorEditor>;
template class ProcessorEditorWithConnectors<InternalIOProcessor, IOEditor>;
template class ProcessorEditorWithConnectors<EQFourProcessor, EQFourProcessorEditor>;
