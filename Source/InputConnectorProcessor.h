/*
  ==============================================================================

    InputConnectorProcessor.h
    Created: 4 Nov 2016 1:35:47am
    Author:  Jonathan

  ==============================================================================
*/

#ifndef INPUTCONNECTORPROCESSOR_H_INCLUDED
#define INPUTCONNECTORPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class InputConnectorProcessor : public AudioProcessor
{
public:
  //==============================================================================
  InputConnectorProcessor();
  ~InputConnectorProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  void processBlock(AudioSampleBuffer&, MidiBuffer&) override;

  //==============================================================================
  bool hasEditor() const override { return false; };
  AudioProcessorEditor* createEditor() override { return nullptr; };

  //==============================================================================
  const String getName() const override { return "processorInputMixer"; };

  bool acceptsMidi() const override { return false; };
  bool producesMidi() const override { return false; };
  double getTailLengthSeconds() const override { return 0.0; };

  //==============================================================================
  int getNumPrograms() override { return 1; };
  int getCurrentProgram() override { return 0; };
  void setCurrentProgram(int) override { };
  const String getProgramName(int) override { return String(); };
  void changeProgramName(int, const String&) override {};

  //==============================================================================
  void getStateInformation(MemoryBlock&) override {};
  void setStateInformation(const void*, int) override {};


private:
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputConnectorProcessor)
};


#endif  // INPUTCONNECTORPROCESSOR_H_INCLUDED
