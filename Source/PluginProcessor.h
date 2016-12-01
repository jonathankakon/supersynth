/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/**
*/
class SupersynthAudioProcessor : public AudioProcessor, public AsyncUpdater
{
public:
  //==============================================================================
  SupersynthAudioProcessor();
  ~SupersynthAudioProcessor();

  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  void prepareToPlay(double sr, int samplesPerBlock) override {
    setPlayConfigDetails(1, 1, sr, samplesPerBlock);

    graph->setPlayConfigDetails(1, 1, sr, samplesPerBlock);
    sampleRate = sr;
    blockSize = samplesPerBlock;

    this->triggerAsyncUpdate();
  };

  void releaseResources() override { return; };

  void processBlock(AudioSampleBuffer& audioBuffer, MidiBuffer& midiBuffer) override 
    { graph->processBlock(audioBuffer, midiBuffer); };

  void handleAsyncUpdate() override { graph->prepareToPlay(sampleRate, blockSize); };

  //==============================================================================

  const String getName() const override { return "Supersynth"; };
  bool supportsDoublePrecisionProcessing() const override { return false; };

  bool acceptsMidi() const override { return true; };
  bool producesMidi() const override { return false; };
  double getTailLengthSeconds() const override { return 0; };

  //==============================================================================
  int getNumPrograms() override { return 0; };
  int getCurrentProgram() override { return 0; };
  void setCurrentProgram(int) override { return; };
  const String getProgramName(int) override { return "Supersynth"; };
  void changeProgramName(int, const String& ) override { return; };

  //==============================================================================
  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  ScopedPointer<AudioProcessorGraph> graph;
  ScopedPointer<XmlElement> stateInformation;
private:
  int blockSize;
  double sampleRate;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SupersynthAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
