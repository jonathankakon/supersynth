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
#include "VAOscillator.h"

class WaveGeneratorProcessor: public AudioProcessor, AudioProcessorListener
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

  void audioProcessorParameterChanged(AudioProcessor *processor, int parameterIndex, float newValue) override;
  void audioProcessorChanged(AudioProcessor *) override { return; }
  
  void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
  
  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;
  
  //==============================================================================
  
  bool supportsDoublePrecisionProcessing() const override;

  //==============================================================================
  const String getName() const override;
  
  bool acceptsMidi() const override;
  bool producesMidi() const override;
  double getTailLengthSeconds() const override;
  
  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram (int index) override;
  const String getProgramName (int index) override;
  void changeProgramName (int index, const String& newName) override;
  
  //==============================================================================
  void getStateInformation (MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;
  
  
  //==============================================================================
  
  enum waveform
  {
    sine,
    saw,
    square
  };
  
  void setWaveform(waveform newWaveform);
  

private:
  
  AudioParameterFloat* frequencyParam;
  AudioParameterFloat* volumeParam;
  AudioParameterChoice* currentWaveformParam;
  
  AudioParameterInt* octaveParam;
  AudioParameterInt* semitonesParam;
  AudioParameterInt* centsParam;
  
  double currentSampleRate;
  
  waveform currentWaveform;
  
  VAOscillator* oscillator;
  
  
  // factors to multiply when changing the frequency the middle value is always 1 => no change
  const float octaves[7] = {0.125, 0.25, 0.5, 1, 2, 4, 8};
  const float semitones[1] = {1.0};
  const float cents[1] = {1.0};
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveGeneratorProcessor);
};



#endif  // WAVEGENERATORPROCESSOR_H_INCLUDED
