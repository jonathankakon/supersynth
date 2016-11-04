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

class ProcessorEditorBase;

//==============================================================================
/*
*/
class InputConnector : public AudioProcessorEditor , SliderListener
{
public:
  InputConnector(AudioProcessor* p, ProcessorEditorBase* b);
  ~InputConnector();

  void paint(Graphics&) override;
  void resized() override;

  void sliderValueChanged(Slider* slider) override;
private:

  OwnedArray<Slider> sliders;

  AudioProcessor& processor;
  ProcessorEditorBase& parent;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputConnector)
};


#endif  // INPUTCONNECTOR_H_INCLUDED
