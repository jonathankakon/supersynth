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
    sawUp,
    sawDown,
    square,
    triangle
  };
  
  void setWaveform(waveform newWaveform);
  
  inline void setBlepOn(double on) {oscillator->setBlepOn(on);}
  
  

private:
  
  AudioParameterFloat* frequencyParam;
  AudioParameterFloat* volumeParam;
  AudioParameterChoice* waveformParam;
  
  AudioParameterInt* octaveParam;
  AudioParameterInt* semitonesParam;
  AudioParameterInt* centsParam;
  
  double currentSampleRate;
  
  waveform currentWaveform;
  
  ScopedPointer<VAOscillator> oscillator;
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveGeneratorProcessor);
};



#endif  // WAVEGENERATORPROCESSOR_H_INCLUDED
