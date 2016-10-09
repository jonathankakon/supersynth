/*
  ==============================================================================

    ToolboxComponent.h
    Created: 9 Oct 2016 11:03:18am
    Author:  Jonathan

  ==============================================================================
*/

#ifndef TOOLBOXCOMPONENT_H_INCLUDED
#define TOOLBOXCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ToolboxComponent    : public Component, public ButtonListener
{
public:
    ToolboxComponent();
    ~ToolboxComponent();

    void paint (Graphics&) override;
    void resized() override;
	void buttonClicked(Button* buttonThatWasClicked) override;

private:
	ScopedPointer<ListBox> moduleList;
	ScopedPointer<ToggleButton> collapseButton;

	bool open = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolboxComponent)
};


#endif  // TOOLBOXCOMPONENT_H_INCLUDED
