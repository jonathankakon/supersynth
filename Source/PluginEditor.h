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

#include "WaveGeneratorProcessor.h"
#include "FilterProcessor.h"


//==============================================================================
/**
*/
class SupersynthAudioProcessorEditor  : 
	public AudioProcessorEditor,
	public ButtonListener,
	public DragAndDropContainer,
	public ComponentListener
{
public:
    SupersynthAudioProcessorEditor (SupersynthAudioProcessor&);
    ~SupersynthAudioProcessorEditor();
	
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void buttonClicked(Button* buttonThatWasClicked) override;
	void componentMovedOrResized(Component&	component, bool wasMoved, bool wasResized) override;

	void setViewPortDragScrolling(bool allow);
	void addAudioProcessor(int processorType);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	ScopedPointer<ToolboxComponent> toolbox;
	ScopedPointer<CollapseButton> collapseButton;
	ScopedPointer<Viewport> viewport;
	ScopedPointer<Worksheet> worksheet;
  SupersynthAudioProcessor& processor;

	bool open = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SupersynthAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
