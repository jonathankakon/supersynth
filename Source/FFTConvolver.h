/*
  ==============================================================================

    FFTConvolver.h
    Created: 9 Dec 2016 1:23:50pm
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef FFTCONVOLVER_H_INCLUDED
#define FFTCONVOLVER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h";

class FFTConvolver
{
public:
  FFTConvolver(const float* taps, int length, int bufferSize);
  ~FFTConvolver();

  void performConvolution(AudioBuffer<float> bufferIn);
  void setNewTapsArray(const float* taps, int length);
private:
  int fftLength;
  int tapsLength;
  int samplesPosition;
  int segments;
  int bufferSize;

  ScopedPointer<FFT> forwardFFT;
  ScopedPointer<FFT> reverseFFT;

  HeapBlock<float> output;
  HeapBlock<float> overlap;

  HeapBlock<float> tempSquared;
  HeapBlock<float> tempFlipped;

  HeapBlock<float> tapsData;
  HeapBlock<float> tapsDataFlipped;
  HeapBlock<float> samplesData;
};



#endif  // FFTCONVOLVER_H_INCLUDED
