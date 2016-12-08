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
    keyboardState.reset();

    graph->setPlayConfigDetails(1, 1, sr, samplesPerBlock);
    sampleRate = sr;
    blockSize = samplesPerBlock;

    this->triggerAsyncUpdate();
  };

  void releaseResources() override { keyboardState.reset(); };

  void processBlock(AudioSampleBuffer& audioBuffer, MidiBuffer& midiBuffer) override {
    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    keyboardState.processNextMidiBuffer(midiBuffer, 0, audioBuffer.getNumSamples(), true);
    graph->processBlock(audioBuffer, midiBuffer); 
  };

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
  ValueTree stateInformation;
  // this is kept up to date with the midi messages that arrive, and the UI component
  // registers with it so it can represent the incoming messages
  MidiKeyboardState keyboardState;
private:
  int blockSize;
  double sampleRate;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SupersynthAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
