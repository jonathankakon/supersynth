/*
  ==============================================================================

    FIRFilter.cpp
    Created: 25 Nov 2016 1:56:41pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FIRFilter.h"

FIRFilter::FIRFilter(const float* tapsArray, int size){
  
  tapsLength = size;
  
  filterBuffer = new AudioBuffer<float>(1, tapsLength);
  filterBufferPointer = filterBuffer->getWritePointer(0);
  filterBuffer->clear();
  
  
  isInitialised = false;
  
//initialis taps buffer
  taps = new AudioBuffer<float>(1, tapsLength);
  tapsPointer = taps->getWritePointer(0);
  
  result = new AudioBuffer<float>(1, tapsLength);
  resultPointer = result->getWritePointer(0);
  
  for (int i = 0; i < tapsLength; i++) {
    tapsPointer[i] = tapsArray[tapsLength -1 - i];
  }
 // taps->reverse(0, size);
  
//  for(int i = 0; i < size/2; i++){
//    float temp = tapsPointer[i];
//    tapsPointer[i] = tapsPointer[size -1 -i];
//    tapsPointer[size -1 -i] = temp;
//  }
  
  index = 0;
  
}

FIRFilter::~FIRFilter()
{
  
}

void FIRFilter::applyFIRFilter(AudioBuffer<float> &buffer)
{
  
  float* audioDataPointer = buffer.getWritePointer(0);
  
  //initialise filterBuffer
//  if(!isInitialised){
//  for(int index = 0; index < tapsLength; index++)
//  {
//    filterBufferPointer[index] = audioDataPointer[index];
//  }
//    isInitialised = true;
//  }
  
  
  for (int i = 0; i < buffer.getNumSamples(); i++) {
    filterBufferPointer[index] = audioDataPointer[i];
    fastConvolution(audioDataPointer, i);
    //update index
    index++;
    if(!(index % tapsLength))
      index = 0;
  }

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

void FIRFilter::fastConvolution(float *audioDataPointer, int audioDataIndex)
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
    audioDataPointer[audioDataIndex] = audioDataPointer[audioDataIndex] + resultPointer[i];
  }
  
}