/*
  ==============================================================================

    FilterProcessorBase.h
    Created: 14 Oct 2016 3:57:59pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef FILTERPROCESSORBASE_H_INCLUDED
#define FILTERPROCESSORBASE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class FilterProcessorBase: public AudioProcessor

{
public:
  
  
private:
  
  AudioParameterFloat *currentCutOffFrequency, *currentBandwidth;
  AudioParameterFloat *firstOrderC0;
  AudioParameterFloat *secondOrderC0, *secondOrderC1;
  
  double sampleRate;
  
  struct AllpassState
  {
    double xh0,xh1,xh2; //xh0 current, xh1 at time n-1, xh2 at time n-2
  };
  
  AllpassState currentAPState;
  
  void resetAllpassState();
  void updateCoefficients();
  
  void applyFirstOrderAllpass(AudioBuffer<float>& buffer); //if we want our processor to support double precision we need to implement a second version of this
  
  void applySecondOrderAllpass(AudioBuffer<float>& buffer); //same here
  
  
  
};

#endif  // FILTERPROCESSORBASE_H_INCLUDED
