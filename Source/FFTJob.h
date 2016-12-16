/*
  ==============================================================================

    FFTJob.h
    Created: 11 Dec 2016 2:40:36pm
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef FFTJOB_H_INCLUDED
#define FFTJOB_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h";

class FFTJob : public ThreadPoolJob
{
public:
  FFTJob(String name, int length, float* samples, float* taps, FFT* forwardFFT, WaitableEvent* evt) : 
         ThreadPoolJob(name), length(length), samples(samples), taps(taps), forwardFFT(forwardFFT), evt(evt){};
  ~FFTJob() {};

  JobStatus runJob() override 
  {
    forwardFFT->performRealOnlyForwardTransform(samples);

    FFT::Complex* samplesComplexArray = reinterpret_cast<FFT::Complex*>(samples);
    FFT::Complex* tapsComplexArray = reinterpret_cast<FFT::Complex*>(taps);

    for (int i = 0; i < length; i++)
    {
      if(shouldExit())
      {
        evt->signal();
        return JobStatus::jobHasFinished;
      }
      double newReal = (samplesComplexArray[i].r * tapsComplexArray[i].r - samplesComplexArray[i].i*tapsComplexArray[i].i);
      double newImag = (samplesComplexArray[i].r * tapsComplexArray[i].i + samplesComplexArray[i].i * tapsComplexArray[i].r);
      samplesComplexArray[i].r = newReal;
      samplesComplexArray[i].i = newImag;
    }

    evt->signal();
    return JobStatus::jobHasFinished;
  }
private:
  int length;
  float* samples;
  float* taps;
  FFT* forwardFFT;
  WaitableEvent* evt;
};


#endif  // FFTJOB_H_INCLUDED
