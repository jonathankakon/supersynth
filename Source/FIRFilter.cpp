
/*
  ==============================================================================

    FIRFilter.cpp
    Created: 25 Nov 2016 1:56:41pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FIRFilter.h"


FIRFilter::FIRFilter(const float* tapsArray, int size, int bufferSize)
{
  /*
  tapsLength = size;
  
  filterBuffer = new AudioBuffer<float>(1, maxLength);
  filterBufferPointer = filterBuffer->getWritePointer(0);
  filterBuffer->clear();

  isInitialised = false;
  
//initialis taps buffer
  taps = new AudioBuffer<float>(1, maxLength);
  tapsPointer = taps->getWritePointer(0);
  
  result = new AudioBuffer<float>(1, maxLength);
  resultPointer = result->getWritePointer(0);
  
  for (int i = 0; i < tapsLength; i++) {
    tapsPointer[i] = tapsArray[tapsLength -1 - i];
  }

  index = 0;
*/
  convolver = new FFTConvolver(tapsArray, size, bufferSize);
}

FIRFilter::~FIRFilter()
{
  
}

void FIRFilter::applyFIRFilter(AudioBuffer<float> &buffer)
{
  convolver->performConvolution(buffer);
  /*
  float* audioDataPointer = buffer.getWritePointer(0);
  
  for (int i = 0; i < buffer.getNumSamples(); i++)
  {
    filterBufferPointer[index] = audioDataPointer[i];
    vectorConvolution(audioDataPointer, i);
    //update index
    index++;
    if(!(index % tapsLength))
      index = 0;
  }
*/
  
}

void FIRFilter::convolute(float* audioDataPointer, int audioDataIndex)
{
  float result = 0;
  for (int i = 0; i < tapsLength; i++) {
    int tapsIndex = (index + tapsLength - i);
    int tapsIndexAdj = (tapsIndex < tapsLength) ?  tapsIndex : tapsIndex - tapsLength;
    result += tapsPointer[i] * filterBufferPointer[tapsIndexAdj];
  }
  audioDataPointer[audioDataIndex] = result;
  
}

void FIRFilter::vectorConvolution(float *audioDataPointer, int audioDataIndex)
{
  float * startPointerFilterBuffer;
  startPointerFilterBuffer =  filterBufferPointer + index + 1;
  float * startPointerTaps;
  startPointerTaps = tapsPointer + tapsLength - 1 - index;
  
  result->clear();
  
  
  FloatVectorOperations::multiply(resultPointer, filterBufferPointer, startPointerTaps, index + 1);
  FloatVectorOperations::multiply(resultPointer + index, startPointerFilterBuffer, tapsPointer, tapsLength - 1 - index);

  audioDataPointer[audioDataIndex] = 0;
  for(int i = 0; i < tapsLength; i++)
  {
    audioDataPointer[audioDataIndex] += resultPointer[i];
  }
  
}


void FIRFilter::changeTaps(const float *tapsArray, int size)
{
  convolver->setNewTapsArray(tapsArray, size);
  /*
//  taps->setSize(0, size);
  taps->clear();
  
//  filterBuffer->setSize(0, size);
  filterBuffer->clear();
  
  tapsLength = size;
  index = 0;

  for (int i = 0; i < tapsLength; i++)
  {
    tapsPointer[i] = tapsArray[tapsLength -1 - i];
  }
  */
}






