/*
  ==============================================================================

    ToolboxComponent.cpp
    Created: 9 Oct 2016 11:03:18am
    Author:  Jonathan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ToolboxComponent.h"

int ToolboxComponent::getNumRows()
{
	return modules.size();
}

void ToolboxComponent::paintListBoxItem(int rowNumber, Graphics& g,
	int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		g.fillAll(Colours::lightblue);

	g.setColour(Colours::black);
	g.setFont(height * 0.7f);

	g.drawText((modules[rowNumber])->name,
		5, 0, width, height,
		Justification::centredLeft, true);
}

var ToolboxComponent::getDragSourceDescription(const SparseSet<int>& selectedRows)
{
	// for our drag description, we'll just make a comma-separated list of the selected row
	// numbers - this will be picked up by the drag target and displayed in its box.
	StringArray rows;

	for (int i = 0; i < selectedRows.size(); ++i)
		rows.add(String(selectedRows[i] + 1));

	return rows.joinIntoString(", ");
}

//==============================================================================
ToolboxComponent::ToolboxComponent() : 
	collapseButton(new ToggleButton()), 
	moduleList(new ListBox("ModuleList", nullptr))
{
	modules.add(new ModulesListElement({ "Wave Generator", "waveGenerator.png", "WaveGeneratorProcessor" }));
	modules.add(new ModulesListElement({ "IIR Filter", "filter.png", "FilterProcessor" }));
	modules.add(new ModulesListElement({ "N-Channel Mixer", "mixer.png", "MixerProcessor" }));

    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	Rectangle<int> r(getLocalBounds());
	Rectangle<int> listBounds = r.withTrimmedRight(20);
	Rectangle<int> buttonBounds(listBounds.getRight(), listBounds.getY(), 20, r.getHeight());

	addAndMakeVisible(moduleList);
	moduleList->setBounds(listBounds);
	moduleList->setModel(this);
	moduleList->setMultipleSelectionEnabled(false);

	addAndMakeVisible(collapseButton);
	collapseButton->setBounds(buttonBounds);
	collapseButton->addListener(this);

	ComponentBoundsConstrainer* bounds = new ComponentBoundsConstrainer();
	bounds->setMaximumWidth(400);
	bounds->setMinimumWidth(150);
	resizeBorder = new ResizableBorderComponent(this, bounds);
	addAndMakeVisible(resizeBorder);
	resizeBorder->setBorderThickness(BorderSize<int>(0,0,0,3));
	resizeBorder->setBounds(r);
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
	resizeBorder->setBounds(r);
}

void ToolboxComponent::buttonClicked(Button* buttonThatWasClicked)
{

	if (buttonThatWasClicked == collapseButton)
	{
		open = collapseButton->getToggleState();

		if (open)
		{
			Desktop::getInstance().getAnimator().animateComponent(this, getLocalBounds().withX(-(getWidth() - 20)), 1, 100, true, 1, 0.01);
		}
		else
		{
			Desktop::getInstance().getAnimator().animateComponent(this, getLocalBounds().withX(0), 1, 100, true, 1, 0.01);
		}
	}
}
