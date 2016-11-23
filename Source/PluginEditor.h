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

#include "EQFourProcessor.h"

class InternalIOProcessor;

//==============================================================================
/**
*/
class SupersynthAudioProcessorEditor :
  public AudioProcessorEditor,
  public ButtonListener,
  public DragAndDropContainer,
  public ComponentListener
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

  void addIOComponents();
  void enableAllInternalBuses(int outNode, int inNode) const;
  void addConnection(Connection* connection) const;
  void removeConnection(Connection& connection) const;
  bool testConnection(Connection& connection, int dest_id) const;
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  int addInternalProcessor(InternalIOProcessor* p, int x, int y, bool addToWorksheet) const;
  int midiInID = 0;

  ScopedPointer<ToolboxComponent> toolbox;
  ScopedPointer<CollapseButton> collapseButton;
  ScopedPointer<Viewport> viewport;
  ScopedPointer<Worksheet> worksheet;

  SupersynthAudioProcessor& processor;

  bool open = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SupersynthAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
