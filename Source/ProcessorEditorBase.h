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

class ProcessorEditorBase : public AudioProcessorEditor
{
public:
  ProcessorEditorBase(AudioProcessor* processor);
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

  void registerImmobileObject(Component& component) const;

  void mouseDown(const MouseEvent& e) override;
  void mouseDrag(const MouseEvent& e) override;
  void mouseEnter(const MouseEvent& e) override;
  void mouseExit(const MouseEvent& e) override;
  void mouseUp(const MouseEvent& e) override;

  void setViewPortDragging(bool enableDragging) const;
  void setComponentDragging(bool enableDragging);
  int addProcessorToGraph(AudioProcessor* processor) const;
  virtual void setConnectors() {};

private:
  bool draggingEnabled;
  ComponentDragger dragger;
  AudioProcessor& processor;

  ScopedPointer<DragStopHelper> dragStop;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditorBase);
};


#endif  // PROCESSOREDITORBASE_H_INCLUDED
