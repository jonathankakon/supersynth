/*
  ==============================================================================

    FilterProcessor.h
    Created: 7 Oct 2016 2:50:49pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef FILTERPROCESSOR_H_INCLUDED
#define FILTERPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class FilterProcessor: public AudioProcessor
{
public:
    
    
    
private:

  AudioParameterFloat currentFrequency;
  float bufferedFrequency; //buffered parameter for error checking


  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterProcessor);
};



#endif  // FILTERPROCESSOR_H_INCLUDED
