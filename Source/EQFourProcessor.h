/*
  ==============================================================================

    EQFourProcessor.h
    Created: 2 Nov 2016 4:05:01pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#ifndef EQFOURPROCESSOR_H_INCLUDED
#define EQFOURPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilterProcessor.h"
#include "GenericIIRFilter.h"


class EQFourProcessor: public AudioProcessor, AudioProcessorListener
{
public:
  //==============================================================================
  EQFourProcessor();
  ~EQFourProcessor();
  
  //==============================================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  
  
  void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
  
  void audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue) override;
  void audioProcessorChanged(AudioProcessor* ) override {return;}
  
  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;
  
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
  
  double currentSampleRate;


  void changeFilterTypeBand1(int newIndex) const;
  void changeFilterTypeBand2(int newIndex) const;
  void changeFilterTypeBand3(int newIndex) const;
  void changeFilterTypeBand4(int newIndex) const;
  
private:
  //==============================================================================
  
  ScopedPointer<GenericIIRFilter> filterBand1;
  ScopedPointer<GenericIIRFilter> filterBand2;
  ScopedPointer<GenericIIRFilter> filterBand3;
  ScopedPointer<GenericIIRFilter> filterBand4;
  
  AudioParameterFloat* cutoffFreq1;
  AudioParameterFloat* cutoffFreq2;
  AudioParameterFloat* cutoffFreq3;
  AudioParameterFloat* cutoffFreq4;
  
  AudioParameterFloat* qParam1;
  AudioParameterFloat* qParam2;
  AudioParameterFloat* qParam3;
  AudioParameterFloat* qParam4;
  
  AudioParameterFloat* gainParam1;
  AudioParameterFloat* gainParam2;
  AudioParameterFloat* gainParam3;
  AudioParameterFloat* gainParam4;
  
  AudioParameterChoice* filterTypeBand1;
  AudioParameterChoice* filterTypeBand2;
  AudioParameterChoice* filterTypeBand3;
  AudioParameterChoice* filterTypeBand4;
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQFourProcessor)
};





#endif  // EQFOURPROCESSOR_H_INCLUDED
