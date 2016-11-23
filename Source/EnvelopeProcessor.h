/*
 ==============================================================================
  
  EnvelopeProcessor.h
  Created: 18 Nov 2016 2:22:14pm
  Author:  Kian Hunziker
  
  ==============================================================================
  */

#ifndef ENVELOPEPROCESSOR_H_INCLUDED
#define ENVELOPEPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class EnvelopeProcessor: public AudioProcessor, AudioProcessorListener
{
  public:
    EnvelopeProcessor();
    ~EnvelopeProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
    
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    
    void audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue) override;
    void audioProcessorChanged(AudioProcessor* processor) override {return;}
    
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
    
    struct EnvelopeState
    {
        float lastSample; int numSamplesSinceLastMidi; bool wasNoteOn;
      };
    
    EnvelopeState state;
    
    void resetEnvelopeState()
    {
        state.lastSample = 0.0;
        state.numSamplesSinceLastMidi = 0.0;
        state.wasNoteOn = true;
      }
    
    void updateEnvelopeState(float sample, int sinceLastMidi, bool noteOn){
        state.lastSample = sample;
        state.numSamplesSinceLastMidi = sinceLastMidi;
        state.wasNoteOn = noteOn;
      }
    
    AudioParameterFloat* attackParameter;
    AudioParameterFloat* decayParameter;
    AudioParameterFloat* sustainParameter;
    AudioParameterFloat* releaseParameter;
    
    double currentSampleRate;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeProcessor)
  };



#endif  // ENVELOPEPROCESSOR_H_INCLUDED
