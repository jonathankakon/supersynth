/*
  ==============================================================================

    FIRFilter.cpp
    Created: 25 Nov 2016 1:56:41pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FIRFilter.h"

FIRFilter::FIRFilter(AudioBuffer<float> &buffer,const Array<float> tapsArray){
  
  //INITIALISE FILTERBUFFER
  tapsLength = tapsArray.size();
  
  filterBuffer = new AudioBuffer<float>(1, tapsLength);
  filterBufferPointer = filterBuffer->getWritePointer(0);
  float* audioDataPointer = buffer.getWritePointer(0);
  
  for(int index = 0; index < tapsLength; index++)
  {
    filterBufferPointer[index] = audioDataPointer[index];
  }
  
  index = 0;
  
  
  
}

FIRFilter::~FIRFilter()
{
  
}

void FIRFilter::applyFIRFilter(AudioBuffer<float> &buffer, const Array<float> taps)
{
  float* audioDataPointer = buffer.getWritePointer(0);
  for (int i = 0; i < buffer.getNumSamples(); i++) {
    filterBufferPointer[index] = audioDataPointer[i];
    
  }
  
  
  
}

void FIRFilter::convolute(float* audioDataPointer, const Array<float> taps)
{
  
  
  
  
}