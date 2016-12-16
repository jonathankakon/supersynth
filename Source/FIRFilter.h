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
#include "constants.h"
#include "FFTConvolver.h"


class FIRFilter //: public FFT
{
public:
  FIRFilter(const float* tapsArray, int size, int bufferSize);
  ~FIRFilter();
  
  void applyFIRFilter(AudioBuffer<float> &buffer);
  void changeTaps(const float* tapsArray, int size);


  
private:
  ScopedPointer<AudioBuffer<float>> filterBuffer;
  ScopedPointer<AudioBuffer<float>> taps;
  ScopedPointer<AudioBuffer<float>> result;

  ScopedPointer<FFTConvolver> convolver;

  float* filterBufferPointer;
  float* resultPointer;
  float* tapsPointer;
  
  int maxLength = 4009;
  
  int index;
  int tapsLength;
  int fftSize;

  bool isInitialised;
  
  void convolute(float* audioDataPointer, int audioDataIndex);
  void vectorConvolution(float* audioDataPointer, int audioDataIndex);
};



#endif  // FIRFILTER_H_INCLUDED
