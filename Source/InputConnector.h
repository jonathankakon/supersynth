/*
  ==============================================================================

    InputConnector.h
    Created: 21 Oct 2016 4:02:01pm
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef INPUTCONNECTOR_H_INCLUDED
#define INPUTCONNECTOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Worksheet.h"

class ProcessorEditorBase;

//==============================================================================
/*
*/
class InputConnector : public AudioProcessorEditor, public SliderListener, public SettableTooltipClient
{
public:
  InputConnector(AudioProcessor* p, ProcessorEditorBase* b, int mixerNodeId);
  ~InputConnector();

  void paint(Graphics&) override;
  void resized() override;

  void sliderValueChanged(Slider* slider) override;
  int getNodeId() const { return nodeId; };
  static Point<int> getClosestConnector(const int x, const int y);

  void mouseDown(const MouseEvent& e) override;
  void mouseDrag(const MouseEvent& e) override;
  void mouseUp(const MouseEvent& e) override;
private:
  Worksheet* getWorksheet() const { return findParentComponentOfClass<Worksheet>(); }
  
  int nodeId;
  OwnedArray<Slider> sliders;

  AudioProcessor& processor;
  ProcessorEditorBase& parent;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputConnector)
};


#endif  // INPUTCONNECTOR_H_INCLUDED
