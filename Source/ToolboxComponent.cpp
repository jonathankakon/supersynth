/*
  ==============================================================================

    ToolboxComponent.cpp
    Created: 9 Oct 2016 11:03:18am
    Author:  Jonathan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ToolboxComponent.h"

//==============================================================================
ToolboxComponent::ToolboxComponent() : 
	collapseButton(new ToggleButton()), 
	moduleList(new ListBox("ModuleList", nullptr))
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	Rectangle<int> r(getLocalBounds());
	Rectangle<int> listBounds = r.withTrimmedRight(20);
	Rectangle<int> buttonBounds(listBounds.getRight(), listBounds.getY(), 20, r.getHeight());

	addAndMakeVisible(moduleList);
	moduleList->setBounds(listBounds);

	addAndMakeVisible(collapseButton);
	collapseButton->setBounds(buttonBounds);
	collapseButton->addListener(this);
}

ToolboxComponent::~ToolboxComponent()
{
	moduleList = nullptr;
	collapseButton = nullptr;
}

void ToolboxComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::teal);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
	g.setFont(14.0f);
    g.drawText ("Toolbox", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void ToolboxComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	Rectangle<int> r(getLocalBounds());
	Rectangle<int> listBounds = r.withTrimmedRight(20);
	Rectangle<int> buttonBounds(listBounds.getRight(), listBounds.getY(), 20, r.getHeight());


	moduleList->setBounds(listBounds);
	collapseButton->setBounds(buttonBounds);
}

void ToolboxComponent::buttonClicked(Button* buttonThatWasClicked)
{
	//[UserbuttonClicked_Pre]
	//[/UserbuttonClicked_Pre]

	if (buttonThatWasClicked == collapseButton)
	{
		open = collapseButton->getToggleState();

		if (open)
		{
			Desktop::getInstance().getAnimator().animateComponent(this, getLocalBounds().withX(-180), 1, 100, true, 1, 0.01);
		}
		else
		{
			Desktop::getInstance().getAnimator().animateComponent(this, getLocalBounds().withX(0), 1, 100, true, 1, 0.01);
		}

		//[UserButtonCode_toggleButton] -- add your button handler code here..
		//[/UserButtonCode_toggleButton]
	}

	//[UserbuttonClicked_Post]
	//[/UserbuttonClicked_Post]
}
