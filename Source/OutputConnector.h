/*
  ==============================================================================

    OutputConnector.h
    Created: 21 Oct 2016 4:02:11pm
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef OUTPUTCONNECTOR_H_INCLUDED
#define OUTPUTCONNECTOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class OutputConnector    : public Component
{
public:
    OutputConnector();
    ~OutputConnector();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputConnector)
};


#endif  // OUTPUTCONNECTOR_H_INCLUDED
