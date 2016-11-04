/*
  ==============================================================================

    Worksheet.h
    Created: 10 Oct 2016 6:15:42pm
    Author:  Jonathan

  ==============================================================================
*/

#ifndef WORKSHEET_H_INCLUDED
#define WORKSHEET_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Worksheet : public Component, public DragAndDropTarget
{
public:
    Worksheet();
    ~Worksheet();

    void paint (Graphics&) override;
    void resized() override;

	bool isInterestedInDragSource(const SourceDetails& sourceDetails) override;
	void itemDragEnter(const SourceDetails& sourceDetails) override;
	void itemDragMove(const SourceDetails& sourceDetails) override;
	void itemDragExit(const SourceDetails& sourceDetails) override;
	void itemDropped(const SourceDetails& sourceDetails) override;

	void addEditor(Component* editor);
	void addEditor(Component* editor, double x, double y);

private:
	bool somethingIsBeingDraggedOver;
	Point<int> dropPosition;
	String message;

	OwnedArray<Component> editors;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Worksheet)
};


#endif  // WORKSHEET_H_INCLUDED
