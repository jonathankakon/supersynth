/*
  ==============================================================================

    OutputConnector.cpp
    Created: 21 Oct 2016 4:02:11pm
    Author:  jonathan kakon

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OutputConnector.h"

//==============================================================================
OutputConnector::OutputConnector()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

OutputConnector::~OutputConnector()
{
}

void OutputConnector::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::red);   // clear the background
}

void OutputConnector::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
