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
  addProcessorToGraph(inputMixer);
  audioInputConnector = new InputConnector(inputMixer, this);
  outputConnector = new OutputConnector();

  registerImmobileObject(*audioInputConnector);
  addAndMakeVisible(audioInputConnector);

  registerImmobileObject(*outputConnector);
  addAndMakeVisible(outputConnector);

  Rectangle<int> r(getLocalBounds());

  audioInputConnector->setBounds(r.withY(r.getHeight() / 2 - audioInputConnector->getHeight() / 2)
                                  .withWidth(audioInputConnector->getWidth())
                                  .withHeight(audioInputConnector->getHeight()));
  outputConnector->setBounds(r.withX(r.getWidth() - 32)
                              .withY(r.getHeight() / 2 - outputConnector->getHeight()/2)
                              .withWidth(outputConnector->getWidth())
                              .withHeight(outputConnector->getHeight()));
}

template<class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::paint (Graphics& g)
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
