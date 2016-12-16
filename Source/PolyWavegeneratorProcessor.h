/*
  ==============================================================================

    PolyWavegeneratorProcessor.h
    Created: 9 Dec 2016 11:08:47am
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef POLYWAVEGENERATORPROCESSOR_H_INCLUDED
#define POLYWAVEGENERATORPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "VAOscillator.h"
#include "EnvelopeProcessor.h"

class PolyWavegeneratorProcessor : public AudioProcessor, AudioProcessorListener
{
public:
  //==============================================================================
  PolyWavegeneratorProcessor();
  ~PolyWavegeneratorProcessor();
  
  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  
  /*
   #ifndef JucePlugin_PreferredChannelConfigurations
   bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif
   */
  
  void audioProcessorParameterChanged(AudioProcessor *processor, int parameterIndex, float newValue) override;
  inline void audioProcessorChanged(AudioProcessor *) override { return; }
  
  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;
  
  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override { return true; };
  
  //==============================================================================
  
  bool supportsDoublePrecisionProcessing() const override { return false; };
  
  //==============================================================================
  const String getName() const override { return String("PolyWavegeneratorProcessor"); };
  
  bool acceptsMidi() const override { return false; };
  bool producesMidi() const override { return false; };
  double getTailLengthSeconds() const override { return 0; };
  
  //==============================================================================
  int getNumPrograms() override { return 0; };
  int getCurrentProgram() override { return 0; };
  void setCurrentProgram(int /*index*/) override { return; };
  const String getProgramName(int /*index*/) override { return String(); };
  void changeProgramName(int /*index*/, const String& /*newName*/) override { return; };
  
  //==============================================================================
  void getStateInformation(MemoryBlock& /*destData*/) override { return; };
  void setStateInformation(const void* /*data*/, int /*sizeInBytes*/) override { return; };
  
private:
  
  struct Voice
  {
    VAOscillator oscillator;
    EnvelopeProcessor envelope;
  };
  
  
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PolyWavegeneratorProcessor);
};


#endif  // POLYWAVEGENERATORPROCESSOR_H_INCLUDED
