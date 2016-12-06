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

  bool findConnectorAt(const bool isInput, int x, int y, Point<int>& outPosition, int& nodeId);

  void mouseDown(const MouseEvent& e) override;
  void mouseDrag(const MouseEvent& e) override;
  void mouseEnter(const MouseEvent& e) override;
  void mouseExit(const MouseEvent& e) override;
  void mouseUp(const MouseEvent& e) override;

  void setViewPortDragging(bool enableDragging) const;
  void setComponentDragging(bool enableDragging);
  int addProcessorToGraph(AudioProcessor* processor, int nodeIdToConnect, int channelNumberToConnect) const;
  void addProcessorToGraph(AudioProcessor* processor, int nodeIdToConnect, int mixerId, int channelNumberToConnect) const;
  void removeProcessor(int nodeId, Array<int> mixerNodeIds) const;

  virtual void setConnectors() {};
  virtual void setConnector(AudioProcessor* oldInput, const int nodeId, const int connectedTo) {};
  virtual void setOutputConnectors() {};
  virtual void setNodeId(int) {};
  virtual void registerNodeConnectionListener(Connection*, int, int) {};
  virtual bool hasInputWithId(int, int&, int&) { return false; };
  virtual bool hasOutputWithId(int, int&, int&) { return false; };
  virtual int getNodeId() { return 0; };
  virtual Array<int> getMixerNodeIds() { return Array<int>(); };
  virtual StringPairArray getMixerNodeConnectionIds() { return StringPairArray(); };

  AudioProcessor& getProcessor() const;
  bool hasProcessor() const
  {
    return (&processor != nullptr);
  };

private:
  bool draggingEnabled;
  ComponentDragger dragger;
  AudioProcessor& processor;

  ScopedPointer<DragStopHelper> dragStop;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorEditorBase);
};
#endif  // PROCESSOREDITORBASE_H_INCLUDED
