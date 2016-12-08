
/*
  ==============================================================================

    FIRFilter.cpp
    Created: 25 Nov 2016 1:56:41pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FIRFilter.h"


FIRFilter::FIRFilter(const float* tapsArray, int size, int order, bool isInverse)//: FFT(order, isInverse), forwardTransform(9, false), inverseTransform(9, true)
{
  
  tapsLength = size;
  
  filterBuffer = new AudioBuffer<float>(1, maxLength);
  filterBufferPointer = filterBuffer->getWritePointer(0);
  filterBuffer->clear();
  
  fftSize = getClosestPowerOfTwo(tapsLength);
  
  fftBuffer = new AudioBuffer<float>(1, 4*fftSize);
  fftBufferPointer = fftBuffer->getWritePointer(0);
  fftBuffer->clear();
  
  processedBuffer = new AudioBuffer<float>(1, 4*fftSize);
  processedBufferPointer = processedBuffer->getWritePointer(0);
  processedBuffer->clear();
  
  tapsFFTBuffer = new AudioBuffer<float>(1, 4*fftSize);
  tapsFFTBufferPointer = tapsFFTBuffer->getWritePointer(0);
  tapsFFTBuffer->clear();
  
  audioBackupBuffer = new AudioBuffer<float>(1, fftSize);
  audioBackupBufferPointer = audioBackupBuffer->getWritePointer(0);
  audioBackupBuffer->clear();
  
  for(int i = 0; i < tapsLength; i++)
  {
    tapsFFTBufferPointer[fftSize - 1 + i] = tapsArray[i];
  }
//  order = fftSize;
//  isInverse = false;
//  
//  forwardTransform = new FFT(order, isInverse);
//  isInverse = true;
//  inverseTransform = new FFT(order, isInverse);
  //int a = forwardTransform.getSize();
  //
  //forwardTransform.performRealOnlyForwardTransform(tapsFFTBufferPointer);
  //
  
  isInitialised = false;
  
//initialis taps buffer
  taps = new AudioBuffer<float>(1, maxLength);
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
  
  for (int i = 0; i < buffer.getNumSamples(); i++)
  {
    filterBufferPointer[index] = audioDataPointer[i];
    vectorConvolution(audioDataPointer, i);
    //update index
    index++;
    if(!(index % tapsLength))
      index = 0;
  }

//  for (int i = 0; i < buffer.getNumSamples(); i++)
//  {
//    fftBufferPointer[fftSize - 1 + index] = audioDataPointer[i];
//    audioDataPointer[i] = processedBufferPointer[index];
//    audioBackupBufferPointer[index] = audioDataPointer[i];
//    //update index
//    index++;
//    
//    if(index == fftSize)
//    {
//      fftConvolution();
//      index = 0;
//    }
  
//    if(!(index % fftSize))
//      index = 0;
//  }
  
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
    audioDataPointer[audioDataIndex] = audioDataPointer[audioDataIndex] + resultPointer[i];
  }
  
}

void FIRFilter::fftConvolution()
{
  //forwardTransform.performRealOnlyForwardTransform(fftBufferPointer);
  //
  ////FloatVectorOperations::multiply(processedBufferPointer, tapsFFTBufferPointer, fftBufferPointer, 2*fftSize);
  //
  //processedBuffer->copyFrom(0, 0, fftBufferPointer, 4*fftSize, 1);
  //
  //inverseTransform.performRealOnlyInverseTransform(processedBufferPointer);
  //
  //fftBuffer->clear();
  //fftBuffer->copyFrom(0, 0, audioBackupBufferPointer, fftSize);

}


int FIRFilter::getClosestPowerOfTwo(int number)
{
//  if ( !(number % 2))
//      return number;
  
  int result = 0;
  for(int i = 1; result < number; i++)
      {
        result = powf(2, i);
      }
  return result;
}

void FIRFilter::changeTaps(const float *tapsArray, int size)
{
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
  
}






