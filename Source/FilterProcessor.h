/*
  ==============================================================================

    FilterProcessor.h
    Created: 20 Oct 2016 4:38:12pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef FILTERPROCESSOR_H_INCLUDED
#define FILTERPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "GenericIIRFilter.h"
#include "FIRFilter.h"
#include "RMSRequestable.h"

class FilterProcessor: public AudioProcessor, public RMSRequestable, AudioProcessorListener
{
public:
  //==============================================================================
  FilterProcessor();
  ~FilterProcessor();
  
  //==============================================================================
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
  void changeFilterType(int newIndex) const
  {
    *filterType = newIndex;
  };
  
  
private:
  //==============================================================================
  
  AudioParameterFloat* cutoffFreqency;
  AudioParameterFloat* qParameter;
  AudioParameterChoice* filterType;
  AudioParameterFloat* gainParameter;
  
  ScopedPointer<StringArray> types;
  
  double currentSampleRate;
  
  ScopedPointer<GenericIIRFilter> filterIIR;
  ScopedPointer<FIRFilter> filterFIR;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterProcessor)
};


#endif  // FILTERPROCESSOR_H_INCLUDED
