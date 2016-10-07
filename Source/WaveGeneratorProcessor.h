/*
  ==============================================================================

    WaveGeneratorProcessor.h
    Created: 7 Oct 2016 3:42:56pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef WAVEGENERATORPROCESSOR_H_INCLUDED
#define WAVEGENERATORPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class WaveGeneratorProcessor: AudioProcessor
{
public:
  //==============================================================================
  WaveGeneratorProcessor();
  ~WaveGeneratorProcessor();
  
  //==============================================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
 
  /*
#ifndef JucePlugin_PreferredChannelConfigurations
  bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
#endif
  */
  
  void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
  
  //==============================================================================
  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;
  
  //==============================================================================
  
  bool supportsDoublePrecisionProcessing() const override;


  

private:
  
  AudioParameterFloat* currentFrequency;
  AudioParameterFloat* currentVolume;
  
  double currentSampleRate;
  
  double currentPhase;
  double phaseIncrement;
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveGeneratorProcessor);
};



#endif  // WAVEGENERATORPROCESSOR_H_INCLUDED
