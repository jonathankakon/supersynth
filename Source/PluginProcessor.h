/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class SupersynthAudioProcessor : public AudioProcessorGraph
{
public:
    //==============================================================================
    SupersynthAudioProcessor();
    ~SupersynthAudioProcessor();

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SupersynthAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
