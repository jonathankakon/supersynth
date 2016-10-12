/*
  ==============================================================================

    CollapseButton.h
    Created: 10 Oct 2016 5:42:54pm
    Author:  Jonathan

  ==============================================================================
*/

#ifndef COLLAPSEBUTTON_H_INCLUDED
#define COLLAPSEBUTTON_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class CollapseButton    : public ToggleButton
{
public:
    CollapseButton();
    ~CollapseButton();

    void paintButton (Graphics& g, bool isMouseOver, bool isButtonDown) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CollapseButton)
};


#endif  // COLLAPSEBUTTON_H_INCLUDED
