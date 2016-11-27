/*
  ==============================================================================

    FIRFilter.h
    Created: 25 Nov 2016 1:56:52pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#ifndef FIRFILTER_H_INCLUDED
#define FIRFILTER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "math.h"
#include "constants.h"


class FIRFilter
{
public:
  FIRFilter(AudioBuffer<float> &buffer,const Array<float> taps);
  ~FIRFilter();
  
  
private:
  ScopedPointer<AudioBuffer<float>> filterBuffer;
  ScopedPointer<float> filterBufferPointer;
  int index;
  int tapsLength;
  
  void applyFIRFilter(AudioBuffer<float> &buffer, const Array<float> taps);
  void convolute(float* audioDataPointer, const Array<float> taps);
  
};



#endif  // FIRFILTER_H_INCLUDED