/*
  ==============================================================================

    WaveGeneratorProcessor.h
    Created: 7 Oct 2016 3:42:56pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef WAVEGENERATORPROCESSOR_H_INCLUDED
#define WAVEGENERATORPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class WaveGeneratorProcessor: public AudioProcessor
{
public:
  //==============================================================================
  WaveGeneratorProcessor();
  ~WaveGeneratorProcessor();
  
  //==============================================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
 
  /*
#ifndef JucePlugin_PreferredChannelConfigurations
  bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
#endif
  */
  
  void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
  
  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;
  
  //==============================================================================
  
  bool supportsDoublePrecisionProcessing() const override;
  
  //==============================================================================
  
  void setFrequency(float newFrequency);


  

private:
  
  enum waveform
  {
    sine,
    saw,
    square
  };
  
  AudioParameterFloat* currentFrequency;
  AudioParameterFloat* currentVolume;
  
  double currentSampleRate;
  
  double currentPhase; //goes from 0 to 2*pi
  double phaseIncrement;
  
  waveform currentWaveform;
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveGeneratorProcessor);
};



#endif  // WAVEGENERATORPROCESSOR_H_INCLUDED
