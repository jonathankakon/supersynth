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
#include "RMSRequestable.h"



class EnvelopeProcessor: public AudioProcessor, public RMSRequestable, AudioProcessorListener
{
  public:
  
  enum EnvelopeSlopeState
  {
    attack,     /**< In this mode, the processor has output channels
                         representing all the audio input channels that are
                         coming into its parent audio graph. */
    decay,    /**< In this mode, the processor has input channels
                         representing all the audio output channels that are
                         going out of its parent audio graph. */
    sustain,      /**< In this mode, the processor has a midi output which
                         delivers the same midi data that is arriving at its
                         parent graph. */
    release,
    none/**< In this mode, the processor has a midi input and
                         any data sent to it will be passed out of the parent
                         graph. */
  };
  
    EnvelopeProcessor();
    ~EnvelopeProcessor();
  
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
  
    
  
  private:
  
  //-------KianKakonTagTeam-------
  EnvelopeSlopeState getNextSlopeStateFromLastSample(MidiMessage , int , int);
  
  EnvelopeSlopeState slope = EnvelopeSlopeState::none;
  
  float nextSample = 0;
  int lastPlayingNote = 0;
  
  float attackStep = 0;
  float decayStep = 0;
  float releaseStep = 0;
  
  float attackGradient = 0;
  float decayGradient = 0;
  float releaseGradient = 0;
    
    AudioParameterFloat* attackParameter;
    AudioParameterFloat* decayParameter;
    AudioParameterFloat* sustainParameter;
    AudioParameterFloat* releaseParameter;
    
    double currentSampleRate;
  
  

  void expandRangeMinusOneToPlusOne(AudioBuffer<float>& audioBuffer);
  void computeSteps();
  
  
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeProcessor)
  };



#endif  // ENVELOPEPROCESSOR_H_INCLUDED
