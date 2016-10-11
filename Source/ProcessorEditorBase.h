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

class ProcessorEditorBase  : public AudioProcessorEditor, MouseListener
{
public:
  ProcessorEditorBase(AudioProcessor* processor);
  ~ProcessorEditorBase();

  //==============================================================================
  void mouseDown(const MouseEvent& e) override;
  void mouseDrag(const MouseEvent& e) override;
  void mouseEnter(const MouseEvent& e) override;
  void mouseExit(const MouseEvent& e) override;
  void mouseUp(const MouseEvent& e) override;
private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
	ComponentDragger dragger;
	AudioProcessor& processor;
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorEditorBase)
};


#endif  // PROCESSOREDITORBASE_H_INCLUDED
