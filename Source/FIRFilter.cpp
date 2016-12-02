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
  
  isInitialised = false;
  
//initialis taps buffer
  taps = new AudioBuffer<float>(1, tapsLength);
  tapsPointer = taps->getWritePointer(0);
  
  for (int i = 0; i < tapsLength; i++) {
    tapsPointer[i] = tapsArray[i];
  }
  
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
    convolute(audioDataPointer, i);
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
    result += tapsPointer[i] * filterBufferPointer[(index + tapsLength - i) % tapsLength];
  }
  audioDataPointer[audioDataIndex] = result;
  
}