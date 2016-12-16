/*
  ==============================================================================

    FIRFilterProcessorEditor.h
    Created: 8 Dec 2016 1:55:31pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#ifndef FIRFILTERPROCESSOREDITOR_H_INCLUDED
#define FIRFILTERPROCESSOREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FIRFilterProcessor.h"
#include "ProcessorEditorWithConnectors.h"

class FIRFilterProcessorEditor : public AudioProcessorEditor, ToggleButton::Listener, ComboBox::Listener, ChangeListener
{
public:
  FIRFilterProcessorEditor(FIRFilterProcessor* p, ProcessorEditorBase* b);
  ~FIRFilterProcessorEditor();
  
  void paint(Graphics&) override;
  void resized() override;
  
  void buttonClicked(Button* button) override;
  void buttonStateChanged(Button* button) override;
  void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override; 
  void changeListenerCallback(ChangeBroadcaster* source) override;
  
private:
  FIRFilterProcessor& processor;
  ProcessorEditorBase& parent;

  bool hasFile = false;
  
  ScopedPointer<ComboBox> tapsSelectorComboBox;
  
  ScopedPointer<TextButton> bypassButton;
  ScopedPointer<TextButton> openButton;

  ScopedPointer<AudioFormatReaderSource> readerSource;
  AudioFormatManager formatManager;
  AudioThumbnailCache thumbnailCache;
  AudioThumbnail thumbnail;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FIRFilterProcessorEditor)
};

#endif  // FIRFILTERPROCESSOREDITOR_H_INCLUDED
