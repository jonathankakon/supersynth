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
#include "RMSRequestable.h"

class WaveGeneratorProcessor: public AudioProcessor, public RMSRequestable, AudioProcessorListener
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
  
  void setMidiOn(bool on) {takesMidi = on;}

private:
  
  AudioParameterFloat* targetFreqParam;
  AudioParameterFloat* volumeParam;
  AudioParameterFloat* frequencyRollParam;
  AudioParameterChoice* waveformParam;
  
  AudioParameterInt* octaveParam;
  AudioParameterInt* semitonesParam;
  AudioParameterInt* centsParam;
  
  double currentSampleRate;
  int blockSize;
  
  double currentFrequency;
  double targetFrequency;

  bool takesMidi = true;
  
  waveform currentWaveform;
  waveform targetWaveform;
  
  bool waveformChanged;
  
  void setWaveform(waveform newWaveform);
  void setWaveform(int index);
  void updateFrequency();
  
  ScopedPointer<VAOscillator> oscillator;
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveGeneratorProcessor);
};



#endif  // WAVEGENERATORPROCESSOR_H_INCLUDED
