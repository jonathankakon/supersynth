/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "ToolboxComponent.h"
#include "Worksheet.h"
#include "CollapseButton.h"

class InternalIOProcessor;

//==============================================================================
/**
*/
class SupersynthAudioProcessorEditor :
  public AudioProcessorEditor,
  public ButtonListener,
  public DragAndDropContainer,
  public ComponentListener,
  public AudioProcessorListener
{
public:

  SupersynthAudioProcessorEditor(SupersynthAudioProcessor&);
  ~SupersynthAudioProcessorEditor();

  //==============================================================================
  void paint(Graphics&) override;
  void resized() override;
  void buttonClicked(Button* buttonThatWasClicked) override;
  void componentMovedOrResized(Component&	component, bool wasMoved, bool wasResized) override;

  void setViewPortDragScrolling(bool allow) const;
  void addAudioProcessor(ToolboxComponent::ModulesListElement*) const;
  int addAudioProcessor(AudioProcessor*, int, int) const;
  void addAudioProcessor(AudioProcessor*, int, int, int) const;
  void removeAudioProcessor(int, Array<int>) const;

  void addIOComponents();
  void enableAllInternalBuses(int outNode, int inNode) const;
  void addConnection(Connection* connection) const;
  void removeConnection(Connection& connection) const;
  bool testConnection(Connection& connection, int dest_id) const;

  void setUIStateInformation() const;

  void audioProcessorParameterChanged(AudioProcessor *processor, int parameterIndex, float newValue) override;
  void audioProcessorChanged(AudioProcessor *) override;
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  int addInternalProcessor(InternalIOProcessor* p, int x, int y, bool addToWorksheet) const;

  void SerializeProcessors(ValueTree processors) const;
  void SerializeConnections(ValueTree connections) const;
  void SerializeGraph(ValueTree parent) const;

  void DeserializeProcessors(ValueTree Processors);
  void DeserializeConnections(ValueTree Connections);
  void DeserializeGraph(ValueTree element);

  AudioProcessor* getProcessorFromClassName(String className) const;

  bool isMixerOrMidiConnection(const AudioProcessorGraph::Connection* connection) const;

  int midiInID = 0;

  ScopedPointer<ToolboxComponent> toolbox;
  ScopedPointer<CollapseButton> collapseButton;
  ScopedPointer<Viewport> viewport;
  ScopedPointer<Worksheet> worksheet;
  ScopedPointer<UndoManager> manager;

  SupersynthAudioProcessor& processor;

  bool open = false;
  bool isLoading = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SupersynthAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
