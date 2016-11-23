/*
 ==============================================================================
  
  EnvelopeProcessor.cpp
  Created: 18 Nov 2016 2:22:00pm
  Author:  Kian Hunziker
  
  ==============================================================================
  */

#include "EnvelopeProcessor.h"
#include "ProcessorEditorWithConnectors.h"
#include "EnvelopeProcessorEditor.h"

EnvelopeProcessor::EnvelopeProcessor() : AudioProcessor(BusesProperties().withInput("midi", AudioChannelSet::mono()).withOutput("Envelope", AudioChannelSet::mono()))
{
  addParameter(attackParameter = new AudioParameterFloat("Attack", "Attack",0, 10, 0.05));
  addParameter(decayParameter= new AudioParameterFloat("Decay", "Decay", 0, 10, 0.05));
  addParameter(sustainParameter = new AudioParameterFloat("Sustain", "Sustain", 0, 1, 0));
  addParameter(releaseParameter = new AudioParameterFloat("Release", "Release", 0, 10, 0.05));
  
  *attackParameter = 0.050;
  *decayParameter = 0.090;
  *sustainParameter =0.5;
  *releaseParameter = 0.5;
    
  }

EnvelopeProcessor::~EnvelopeProcessor()
{
  }

void EnvelopeProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    ignoreUnused(sampleRate, samplesPerBlock);
    currentSampleRate = sampleRate;
  }

void EnvelopeProcessor::processBlock(AudioSampleBuffer & audioBuffer, juce::MidiBuffer & midiBuffer)
{
    audioBuffer.clear();
    
    //check if there are midi events
    if(midiBuffer.isEmpty())
        return;
    
    float* const audioData = audioBuffer.getWritePointer(0);
    
    int firstIndex = midiBuffer.getFirstEventTime();
    int lastIndex = midiBuffer.getLastEventTime();
    
    MidiMessage& message1 = *new MidiMessage();
    int samplePosition1 = 0;
    MidiMessage& message2 = *new MidiMessage();
    int samplePosition2 = 0;
    
    ScopedPointer<MidiBuffer::Iterator> iterator = new MidiBuffer::Iterator(midiBuffer);
    
    //OVERLAP FROM PREVIOUS BUFFER
    if (state.wasNoteOn)
      {
        int attackLength = *attackParameter * currentSampleRate - state.numSamplesSinceLastMidi;
        int decayLength = *decayParameter * currentSampleRate;
        float attackGradient = (1.0 - state.lastSample)/((float)attackLength);
        float decayGradient = (1.0 - *sustainParameter)/((float)decayLength);
        
        for (int index = 0; index < firstIndex; index++)
          {
            if(state.numSamplesSinceLastMidi < attackLength)
              {
                audioData[index] += attackGradient;
                break;
              }
            else if ( state.numSamplesSinceLastMidi < attackLength + decayLength)
              {
                audioData[index] -= decayGradient;
                break;
              }
            else
              {
                audioData[index] = *sustainParameter;
              }
          }
        
        //update state
        state.lastSample = audioData[firstIndex - 1];
        state.wasNoteOn = true;
      }
    else if (*releaseParameter * currentSampleRate > state.numSamplesSinceLastMidi)
      {
        int releaseLength = *releaseParameter * currentSampleRate;
        float releaseGradient = state.lastSample/((float)releaseLength - state.numSamplesSinceLastMidi);
        if(firstIndex < releaseLength - state.numSamplesSinceLastMidi)
          {
            for(int index = 0; index < firstIndex; index++)
              {
                audioData[index] -= releaseGradient;
              }
            //update state
            state.lastSample = audioData[firstIndex - 1];
            state.wasNoteOn = false;
          }
        else
          {
            for(int index = 0; index < releaseLength - state.numSamplesSinceLastMidi; index++)
              {
                audioData[index] -= releaseGradient;
              }
            //update state
            state.lastSample = audioData[releaseLength - state.numSamplesSinceLastMidi - 1];
            state.wasNoteOn = false;
          }
      }
    
    
    //FILL CURRENT BUFFER
    while (iterator->getNextEvent(message1, samplePosition1))
      {
        //NOTE ON as in ATTACK
        if(message1.isNoteOn())
          {
            int attackLength = *attackParameter * currentSampleRate - state.numSamplesSinceLastMidi;
            int decayLength = *decayParameter * currentSampleRate;
            float attackGradient = (1.0 - state.lastSample)/((float)attackLength);
            float decayGradient = (1.0 - *sustainParameter)/((float)decayLength);
            
            //there is a next midi message after the current one
            if(iterator->getNextEvent(message2, samplePosition2))
              {
                for(int index = samplePosition1; index < samplePosition2; index++)
                  {
                    if(index < attackLength)
                      {
                        //attack
                        
                        audioData[index] += attackGradient;
                        
                        break;
                      }
                    else if(index < attackLength + decayLength)
                      {
                        //decay
                        
                        audioData[index]-= decayGradient;
                        
                        break;
                      }
                    else{
                        //set to sustain
                        audioData[index] = *sustainParameter;
                      }
                    
                  }
                
                //update state
                state.lastSample = audioData[samplePosition2 - 1];
                state.wasNoteOn = true;
              }
            // No next midi message in the buffer
            else{
                
                for(int index = samplePosition1; index < audioBuffer.getNumSamples(); index++)
                  {
                    if(index < attackLength)
                      {
                        //attack
                        
                        audioData[index] += attackGradient;
                        
                        break;
                      }
                    else if(index < attackLength + decayLength)
                      {
                        //decay
                        
                        audioData[index]-= decayGradient;
                        
                        break;
                      }
                    else{
                        //set to sustain
                        audioData[index] = *sustainParameter;
                      }
                  }
                
                //update state
                state.lastSample = audioData[audioBuffer.getNumSamples() - 1];
                state.wasNoteOn = true;
                state.numSamplesSinceLastMidi = audioBuffer.getNumSamples() - samplePosition1;
              }
            
          }
        //NOTE OFF as in RELEASE
        else if(message1.isNoteOff())
          {
            int releaseLength = *releaseParameter * currentSampleRate;
            float releaseGradient = state.lastSample/((float)releaseLength);
            
            //there is a next midi message after the current one
            if(iterator->getNextEvent(message2, samplePosition2))
              {
                for(int index = samplePosition1; index < samplePosition2; index++)
                  {
                    audioData[index] -= releaseGradient;
                  }
                //update state
                state.lastSample = audioData[samplePosition2 - 1];
                state.wasNoteOn = false;
                break;
              }
            // No next midi message in the buffer
            else{
                
                for(int index = samplePosition1; index < audioBuffer.getNumSamples(); index++)
                  {
                    audioData[index] -= releaseGradient;
                  }
                //update state
                state.lastSample = audioData[samplePosition2 - 1];
                state.wasNoteOn = false;
                state.numSamplesSinceLastMidi = audioBuffer.getNumSamples() - samplePosition1;
              }
          }
      }
  }

void EnvelopeProcessor::releaseResources()
{
  }

void EnvelopeProcessor::audioProcessorParameterChanged(juce::AudioProcessor *processor, int parameterIndex, float newValue)
{
  }

AudioProcessorEditor* EnvelopeProcessor::createEditor(){
    return new ProcessorEditorWithConnectors<EnvelopeProcessor, EnvelopeProcessorEditor>(this);
  }

bool EnvelopeProcessor::hasEditor() const
{
    return true;
  }

const String EnvelopeProcessor::getName() const
{
    return "anEnvelope";
  }

bool EnvelopeProcessor::acceptsMidi() const
{
    return true;
  }

bool EnvelopeProcessor::producesMidi() const
{
    return false;
  }

double EnvelopeProcessor::getTailLengthSeconds() const
{
    return 0.0;
  }

int EnvelopeProcessor::getNumPrograms()
{
    return 1;
  }

int EnvelopeProcessor::getCurrentProgram()
{
    return 0;
  }

void EnvelopeProcessor::setCurrentProgram(int index)
{
    ignoreUnused(index);
  }

const String EnvelopeProcessor::getProgramName(int index)
{
    ignoreUnused(index);
    return String();
  }

void EnvelopeProcessor::changeProgramName(int index, const juce::String &newName)
{
    ignoreUnused(index, newName);
  }

void EnvelopeProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    ignoreUnused(destData);
  }

void EnvelopeProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    ignoreUnused(data, sizeInBytes);
  }
