/*
  ==============================================================================

    DelayProcessor.h
    Created: 5 Dec 2016 4:46:42pm
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef DELAYPROCESSOR_H_INCLUDED
#define DELAYPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "RMSRequestable.h"

class DelayProcessor : public AudioProcessor, public RMSRequestable, AudioProcessorListener
{
public:
  //==============================================================================
  DelayProcessor();
  ~DelayProcessor();

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
  const String getName() const override { return String("DelayProcessor"); };

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

  AudioParameterFloat* delayParam;
  AudioParameterFloat* decayParam;

  double rate;
  int bufferSize; 
  int delayPosition;

  void applyDelay(AudioBuffer<float>& buffer);
  AudioBuffer<float> delayBuffer;


  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayProcessor);
};

#endif  // DELAYPROCESSOR_H_INCLUDED
