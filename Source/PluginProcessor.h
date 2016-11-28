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
class SupersynthAudioProcessor : public AudioProcessor
{
public:
  //==============================================================================
  SupersynthAudioProcessor();
  ~SupersynthAudioProcessor();

  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override {
    setPlayConfigDetails(1, 1, sampleRate, samplesPerBlock);
    graph->setPlayConfigDetails(1, 1, sampleRate, samplesPerBlock);
    graph->prepareToPlay(sampleRate, samplesPerBlock);
  };
  void releaseResources() override { return; };

  void processBlock(AudioSampleBuffer& audioBuffer, MidiBuffer& midiBuffer) override { graph->processBlock(audioBuffer, midiBuffer); };

  //==============================================================================

  const String getName() const override { return "Supersynth"; };
  bool supportsDoublePrecisionProcessing() const override { return false; };

  bool acceptsMidi() const override { return true; };
  bool producesMidi() const override { return false; };
  double getTailLengthSeconds() const override { return 0; };

  //==============================================================================
  int getNumPrograms() override { return 0; };
  int getCurrentProgram() override { return 0; };
  void setCurrentProgram(int index) override { return; };
  const String getProgramName(int index) override { return "Supersynth"; };
  void changeProgramName(int index, const String& newName) override { return; };

  //==============================================================================
  void getStateInformation(MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  ScopedPointer<AudioProcessorGraph> graph;
  ScopedPointer<XmlElement> stateInformation;
private:
  AudioProcessorEditor* editor;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SupersynthAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
