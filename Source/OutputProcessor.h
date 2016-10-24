/*
  ==============================================================================

    OutputProcessor.h
    Created: 21 Oct 2016 3:37:25pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef OUTPUTPROCESSOR_H_INCLUDED
#define OUTPUTPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class OutputProcessor: public AudioProcessor
{
public:
  //==============================================================================
  OutputProcessor();
  ~OutputProcessor();
  
  //==============================================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
 
  
  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
  
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
private:
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputProcessor)
};


#endif  // OUTPUTPROCESSOR_H_INCLUDED
