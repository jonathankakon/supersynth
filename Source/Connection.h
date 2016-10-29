/*
  ==============================================================================

    Connection.h
    Created: 28 Oct 2016 4:18:36pm
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Connection    : public Component
{
public:
    Connection();
    ~Connection();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connection)
};


#endif  // CONNECTION_H_INCLUDED
