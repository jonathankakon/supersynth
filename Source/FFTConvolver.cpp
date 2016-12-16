/*
  ==============================================================================

    FFTConvolver.cpp
    Created: 9 Dec 2016 1:23:50pm
    Author:  jonathan kakon

  ==============================================================================
*/

#include "FFTConvolver.h"

FFTConvolver::FFTConvolver(const float* taps, int length, int bufferSize) :
  bufferSize(bufferSize),
  output((size_t)pow(2, ceill(log2(bufferSize * 2))) * 2),
  tempSquared((size_t)pow(2, ceill(log2(bufferSize * 2))) * 2),
  tempFlipped((size_t)pow(2, ceill(log2(bufferSize * 2))) * 2),
  overlap((size_t)bufferSize),
  tapsData((size_t)pow(2, ceill(log2((length + bufferSize - 1)))) * 2),
  tapsDataFlipped((size_t)pow(2, ceill(log2((length + bufferSize - 1)))) * 2),
  samplesData((size_t)pow(2, ceill(log2((length + bufferSize - 1)))) * 2)
{  
  setNewTapsArray(taps, length);
}

FFTConvolver::~FFTConvolver()
{
}

void FFTConvolver::performConvolution(AudioBuffer<float> bufferIn)
{
  float* input = bufferIn.getWritePointer(0);

  FloatVectorOperations::clear(&samplesData[samplesPosition], fftLength * 2);
  FloatVectorOperations::copy(&samplesData[samplesPosition], input, bufferIn.getNumSamples());

  forwardFFT->performRealOnlyForwardTransform(&samplesData[samplesPosition]);

  output.clear(fftLength * 2);

  int tapsIndex = 0;
  int samplesIndex = samplesPosition;

  double norm = 1.0 / segments;

  for (int i = 0; i < segments; i++)
  {
    FloatVectorOperations::multiply(tempSquared, &samplesData[samplesIndex], &tapsData[tapsIndex], fftLength*2);
    FloatVectorOperations::multiply(tempFlipped, &samplesData[samplesIndex], &tapsDataFlipped[tapsIndex], fftLength*2);

    for (int k = 0; k < fftLength * 2; k+=2)
    {
      tempSquared[k] = tempSquared[k] - tempSquared[k + 1];
      tempSquared[k + 1] = tempFlipped[k] + tempFlipped[k + 1];
    }

    FloatVectorOperations::addWithMultiply(output, tempSquared, norm, fftLength*2);
    samplesIndex = (samplesIndex - 2 * fftLength < 0) ? tapsLength - 2 * fftLength: samplesIndex - 2 * fftLength;
    tapsIndex += 2 * fftLength;
  }

  reverseFFT->performRealOnlyInverseTransform(output);

  FloatVectorOperations::add(output, overlap, bufferIn.getNumSamples());
  bufferIn.copyFrom(0, 0, output, bufferIn.getNumSamples());
  FloatVectorOperations::copy(overlap, &output[bufferIn.getNumSamples()], bufferIn.getNumSamples());

  samplesPosition = (samplesPosition + fftLength * 2 >= tapsLength) ? 0 : samplesPosition + fftLength * 2;
}

void FFTConvolver::setNewTapsArray(const float* taps, int length)
{
  segments = ceil((double)length / (double)bufferSize);
  fftLength = pow(2, ceill(log2(bufferSize*2)));
  tapsLength = 2 * segments * pow(2, ceill(log2(bufferSize*2)));

  forwardFFT = new FFT(log2(fftLength), false);
  reverseFFT = new FFT(log2(fftLength), true);

  samplesData.free();
  tapsData.free();
  tapsDataFlipped.free();
  output.free();
  overlap.free();
  tempSquared.free();
  tempFlipped.free();

  samplesData.allocate(tapsLength, true);
  tapsData.allocate(tapsLength, true);
  tapsDataFlipped.allocate(tapsLength, true);
  output.allocate(fftLength * 2, true);
  overlap.allocate(bufferSize, true);
  tempSquared.allocate(fftLength * 2, true);
  tempFlipped.allocate(fftLength * 2, true);

  samplesData.clear(tapsLength);
  tapsData.clear(tapsLength);
  tapsDataFlipped.clear(tapsLength);
  output.clear(fftLength*2);
  overlap.clear(bufferSize);
  tempSquared.clear(fftLength * 2);
  tempFlipped.clear(fftLength * 2);

  int tapsPerSegment = bufferSize > length ? length : bufferSize;

  for (int i = 0; i < segments; i++)
  {
    for (int k = 0; k < tapsPerSegment; ++k)
    {
      tapsData[i*fftLength*2 + k] = (i*tapsPerSegment + k >= length) ? 0 : taps[i*tapsPerSegment + k];
    }

    forwardFFT->performRealOnlyForwardTransform(&tapsData[i*fftLength*2]);

    for(int k = 0; k < fftLength*2; k+=2)
    {
      tapsDataFlipped[i*fftLength * 2 + k] = tapsData[i*fftLength * 2 + k + 1];
      tapsDataFlipped[i*fftLength * 2 + k + 1] = tapsData[i*fftLength * 2 + k];
    }
  }

  samplesPosition = 0;
}
