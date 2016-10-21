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

//==============================================================================
/*
*/
class InputConnector    : public Component
{
public:
    InputConnector();
    ~InputConnector();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputConnector)
};


#endif  // INPUTCONNECTOR_H_INCLUDED
