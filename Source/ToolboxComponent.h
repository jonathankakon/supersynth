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
class ToolboxComponent    : public Component, public ButtonListener, public ListBoxModel
{
public:
	struct ModulesListElement
	{
		String name;
		String iconPath;
		String processorType;
	};

    ToolboxComponent();
    ~ToolboxComponent();

    void paint (Graphics&) override;
    void resized() override;
	void buttonClicked(Button* buttonThatWasClicked) override;

	int getNumRows() override;
	void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool isRowSeleected) override;
	var getDragSourceDescription(const SparseSet<int>& selectedrow) override;

	bool open = false;

private:
	ScopedPointer<ListBox> moduleList;
	ScopedPointer<ToggleButton> collapseButton;
	ScopedPointer<ResizableBorderComponent> resizeBorder;

	OwnedArray<ModulesListElement> modules;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolboxComponent)
};


#endif  // TOOLBOXCOMPONENT_H_INCLUDED
