/*
  ==============================================================================

    DistortionProcessor.h
    Created: 2 Dec 2016 1:09:17pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef DISTORTIONPROCESSOR_H_INCLUDED
#define DISTORTIONPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "Distorter.h"

class DistortionProcessor: public AudioProcessor, AudioProcessorListener
{
public:
  //==============================================================================
  DistortionProcessor();
  ~DistortionProcessor();
  
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
  
  enum distortionType
  {
    hardclip,
    tanhApprox
  };
  
private:
  
  ScopedPointer<Distorter> distorter;
  
  AudioParameterFloat* preGainParam;
  AudioParameterChoice* distortionTypeParam;
  AudioParameterFloat* postGainParam;
  
  distortionType currentDistortionType;
  
  void setDistType(int index);
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionProcessor);
};

#endif  // DISTORTIONPROCESSOR_H_INCLUDED
