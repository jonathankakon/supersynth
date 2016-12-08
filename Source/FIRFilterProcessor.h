/*
  ==============================================================================

    FIRFilterProcessor.h
    Created: 8 Dec 2016 1:56:02pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#ifndef FIRFILTERPROCESSOR_H_INCLUDED
#define FIRFILTERPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "FIRFilter.h"
#include "RMSRequestable.h"

class FIRFilterProcessor: public AudioProcessor, public RMSRequestable, AudioProcessorListener
{
public:
  //==============================================================================
  FIRFilterProcessor();
  ~FIRFilterProcessor();
  
  //=================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  
  
  void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
  
  void audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue) override;
  void audioProcessorChanged(AudioProcessor*) override {return;}

  
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
  
  void changeFilterTaps(const float* taps)
  {
    filterFIR->changeTaps(taps, getTapsSize(taps));
  };
  void bypass(bool isBypassed)
  {
    bypassBool = isBypassed;
  }
  
  
private:
  
  int getTapsSize(const float* array);
  AudioParameterChoice* tapsSelector;
  bool bypassBool = true;
  
  ScopedPointer<FIRFilter> filterFIR;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FIRFilterProcessor)

  
};
#endif  // FIRFILTERPROCESSOR_H_INCLUDED
