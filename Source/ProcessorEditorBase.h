/*
  ==============================================================================

    ProcessorEditorBase.h
    Created: 11 Oct 2016 2:46:30pm
    Author:  Jonathan

  ==============================================================================
*/

#ifndef PROCESSOREDITORBASE_H_INCLUDED
#define PROCESSOREDITORBASE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "InputConnector.h"
#include "OutputConnector.h"

class ProcessorEditorBase  : public AudioProcessorEditor
{
public:
  ProcessorEditorBase(AudioProcessor* processor, bool hasAudioInput, bool hasControlInput, bool hasGateInput);
  ~ProcessorEditorBase();

  class DragStopHelper : public MouseListener
  {
  public:
    DragStopHelper(ProcessorEditorBase& baseEditor);
    ~DragStopHelper();

    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    void mouseEnter(const MouseEvent& e) override;
    void mouseExit(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
  private:
    ProcessorEditorBase& owner;
  };

  //==============================================================================
  void mouseDown(const MouseEvent& e) override;
  void mouseDrag(const MouseEvent& e) override;
  void mouseEnter(const MouseEvent& e) override;
  void mouseExit(const MouseEvent& e) override;
  void mouseUp(const MouseEvent& e) override;

  void setViewPortDragging(bool enableDragging);
  void setComponentDragging(bool enableDragging);

  void registerImmobileObject(Component& component);
  void setConnectors();
  

private:
  
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  bool takesControlSignal;
  bool takesGateSignal;
  bool takesAudioSignal;

  bool draggingEnabled;
	ComponentDragger dragger;
	AudioProcessor& processor;
  
  ScopedPointer<DragStopHelper> dragStop;
  ScopedPointer<InputConnector> inputConnector;
  ScopedPointer<OutputConnector> outputConnector;
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorEditorBase)
};


#endif  // PROCESSOREDITORBASE_H_INCLUDED
