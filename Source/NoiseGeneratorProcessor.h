/*
  ==============================================================================

    NoiseGeneratorProcessor.h
    Created: 27 Nov 2016 8:03:55pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#ifndef NOISEGENERATORPROCESSOR_H_INCLUDED
#define NOISEGENERATORPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class NoiseGeneratorProcessor: public AudioProcessor, AudioProcessorListener
{
public:
  //==============================================================================
  NoiseGeneratorProcessor();
  ~NoiseGeneratorProcessor();
  
  //==============================================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  
  /*
   #ifndef JucePlugin_PreferredChannelConfigurations
   bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif
   */
  
  void audioProcessorParameterChanged(AudioProcessor *processor, int parameterIndex, float newValue) override;
  inline void audioProcessorChanged(AudioProcessor *) override { return; }
  
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
  
  void changeNoiseType(int newIndex) const
  {
    *noisetypeParam = newIndex;
  };
  
private:
  
  AudioParameterFloat* volumeParam;
  AudioParameterChoice* noisetypeParam;
  
  Random rand;
  
  void fillBufferWhiteNoise(AudioBuffer<float> &buffer);
  void applyPinkNoiseFilter(AudioBuffer<float> &buffer);
  
  struct filterCoefficients{
    float a1, a2, a3, b1, b2, b3;
  };
  
  struct filterState{
    float xn0, xn1, xn2, xn3;
  };
  
  filterState state;
  filterCoefficients coefficients;
  
  void setFilterCoefficients()
  {
    coefficients.a1 = -2.47931;
    coefficients.a2 = 1.98501;
    coefficients.a3 = -0.5056;
    coefficients.b1 = -1.89404;
    coefficients.b2 = 0.958565;
    coefficients.b3 = -0.0621323;
  };
  
  void resetFilterState()
  {
    state.xn0 = 0;
    state.xn1 = 0;
    state.xn2 = 0;
    state.xn3 = 0;
  }
  
  
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoiseGeneratorProcessor);
};

#endif  // NOISEGENERATORPROCESSOR_H_INCLUDED
