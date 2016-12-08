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


class FIRFilter //: public FFT
{
public:
  FIRFilter(const float* tapsArray, int size, int order, bool isInverse);
  ~FIRFilter();
  
  void applyFIRFilter(AudioBuffer<float> &buffer);
  void changeTaps(const float* tapsArray, int size);


  
private:
  ScopedPointer<AudioBuffer<float>> filterBuffer;
  ScopedPointer<AudioBuffer<float>> taps;
  ScopedPointer<AudioBuffer<float>> result;
  ScopedPointer<AudioBuffer<float>> fftBuffer;
  ScopedPointer<AudioBuffer<float>> processedBuffer;
  ScopedPointer<AudioBuffer<float>> tapsFFTBuffer;
  ScopedPointer<AudioBuffer<float>> audioBackupBuffer;
  float* filterBufferPointer;
  float* resultPointer;
  float* tapsPointer;
  float* fftBufferPointer;
  float* processedBufferPointer;
  float* tapsFFTBufferPointer;
  float* audioBackupBufferPointer;
  
  
  int maxLength = 801;
  
  //FFT forwardTransform;
  //FFT inverseTransform;
  
  int index;
  int tapsLength;
  int fftSize;

  bool isInitialised;
  
  void convolute(float* audioDataPointer, int audioDataIndex);
  void vectorConvolution(float* audioDataPointer, int audioDataIndex);
  int getClosestPowerOfTwo(int number);
  void fftConvolution();
};



#endif  // FIRFILTER_H_INCLUDED
