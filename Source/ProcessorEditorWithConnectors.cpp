/*
  ==============================================================================

    ProcessorEditorWithConnectors.cpp
    Created: 29 Oct 2016 10:44:41am
    Author:  jonathan kakon

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ProcessorEditorWithConnectors.h"

template<class AudioProcessorType, class EditorType>
ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::ProcessorEditorWithConnectors(AudioProcessorType * p) : ProcessorEditorBase(p),
processor(*p), processorEditor(new EditorType(p, this)),
audioInputConnector(new InputConnector()), outputConnector(new OutputConnector())
{
  ((Component*)processorEditor)->addMouseListener(this, false);
  addAndMakeVisible(processorEditor);

  registerImmobileObject(*audioInputConnector);
  addAndMakeVisible(audioInputConnector);

  registerImmobileObject(*outputConnector);
  addAndMakeVisible(outputConnector);

  Rectangle<int> r(((Component*)processorEditor)->getBounds());
  ((Component*)processorEditor)->setBounds(r.withX(r.getX() + 20));
  r.expand(20, 0);
  setSize(r.getWidth(), r.getHeight());
  setConnectors();
}

template<class AudioProcessorType, class EditorType>
ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::~ProcessorEditorWithConnectors()
{
}


template<class AudioProcessorType, class EditorType>
void ProcessorEditorWithConnectors<AudioProcessorType, EditorType>::setConnectors()
{

  Rectangle<int> r(getLocalBounds());

  audioInputConnector->setBounds(r.withWidth(20).withHeight(20).withY(r.getHeight() / 2 - 10));
  outputConnector->setBounds(r.withWidth(20).withHeight(20).withX(r.getWidth() - 20).withY(r.getHeight() / 2 - 10));
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
template class ProcessorEditorWithConnectors<EQFourProcessor, EQFourProcessorEditor>;
