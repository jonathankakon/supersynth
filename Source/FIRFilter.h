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
  FIRFilter(const float* tapsArray, int size);
  ~FIRFilter();
  
  void applyFIRFilter(AudioBuffer<float> &buffer);

  
private:
  ScopedPointer<AudioBuffer<float>> filterBuffer;
  ScopedPointer<float> filterBufferPointer;
  int index;
  int tapsLength;
  ScopedPointer<AudioBuffer<float>> taps;
  ScopedPointer<float> tapsPointer;
  bool isInitialised;
  
  void convolute(float* audioDataPointer, int audioDataIndex);
  
};



#endif  // FIRFILTER_H_INCLUDED
