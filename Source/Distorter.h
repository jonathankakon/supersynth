/*
  ==============================================================================

    Distorter.h
    Created: 28 Nov 2016 8:45:22am
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef DISTORTER_H_INCLUDED
#define DISTORTER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FIRFilter.h"

class Distorter
{
public:
  Distorter();
  ~Distorter();
  
  
  void processHardclip(AudioBuffer<float>& buffer);
  void processTanhAprx(AudioBuffer<float>& buffer);
  
  inline double getSampleRate(){return currentSampleRate;}
  void setSampleRate(double newSampleRate);
  
private:
  
  ScopedPointer<FIRFilter> preFilter;
  ScopedPointer<FIRFilter> postFilter;
  
  double currentSampleRate;
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distorter)
};



#endif  // DISTORTER_H_INCLUDED
