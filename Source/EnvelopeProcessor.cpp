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
  
  *attackParameter = 0.1;
  *decayParameter = 1;
  *sustainParameter =0.5;
  *releaseParameter = 3.0;
  
  resetEnvelopeState();
  state.wasNoteOn = false;

    
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
  
    float* const audioData = audioBuffer.getWritePointer(0);
    
    int firstIndex = midiBuffer.getFirstEventTime();
    
    MidiMessage& message1 = *new MidiMessage();
    int samplePosition1 = 0;
    MidiMessage& message2 = *new MidiMessage();
    int samplePosition2 = 0;
    
    ScopedPointer<MidiBuffer::Iterator> iterator = new MidiBuffer::Iterator(midiBuffer);
    
    //OVERLAP FROM PREVIOUS BUFFER
    if (state.wasNoteOn)
      {
        //DBG("last sample" << state.lastSample);
        //DBG("attackcounter" << state.attackDecayCounter);
        
        
        int attackLength = *attackParameter * currentSampleRate;
        int decayLength = *decayParameter * currentSampleRate;
        float attackGradient = (1.0)/((float)attackLength);
        float decayGradient = (1.0 - *sustainParameter)/((float)decayLength);
        
        
        //there is another sample in the buffer -> first index > 0
        if(firstIndex > 0 )
        {
          
          //initialise first sample of the buffer
          if(state.attackDecayCounter < attackLength)
          {
            audioData[0] = state.lastSample + attackGradient;
          }
          else if(state.attackDecayCounter < attackLength+decayLength){
            audioData[0] = state.lastSample - decayGradient;
          }
          else{
            audioData[0] = *sustainParameter;
          }
          
          //DBG("first sample" << audioData[0]);
          
          //calculate the remaining samples
          for (int index = 1; index < firstIndex; index++)
          {
            if(state.attackDecayCounter < attackLength)
            {
              audioData[index] = audioData[index-1] + attackGradient;
              state.attackDecayCounter++;
              
            }
            else if ( state.attackDecayCounter < attackLength + decayLength)
            {
              audioData[index] = audioData[index-1] - decayGradient;
              state.attackDecayCounter++;
              
            }
            else
            {
              audioData[index] = *sustainParameter;
            }
          }
          //update state
          if (attackLength+ decayLength < state.attackDecayCounter) {
            state.attackDecayCounter = 0;
          }
          state.lastSample = audioData[firstIndex - 1];
          state.wasNoteOn = true;
        }
        //the buffer is empty
        else
        {
          //initialise first sample of the buffer
          if(state.attackDecayCounter < attackLength)
          {
            audioData[0] = state.lastSample + attackGradient;
          }
          else if(state.attackDecayCounter < attackLength+decayLength){
            audioData[0] = state.lastSample - decayGradient;
          }
          else{
            audioData[0] = *sustainParameter;
          }
          

          
          for (int index = 1; index < audioBuffer.getNumSamples(); index++)
            {
              if(state.attackDecayCounter < attackLength)
                {
                  audioData[index] = audioData[index-1] + attackGradient;
                  state.attackDecayCounter++;
                }
              else if ( state.attackDecayCounter < attackLength + decayLength)
                {
                  audioData[index] = audioData[index-1] - decayGradient;
                  state.attackDecayCounter++;
                }
              else
                {
                  audioData[index] = *sustainParameter;
                }
            }
          //update state
          if (attackLength+ decayLength < state.attackDecayCounter) {
            state.attackDecayCounter = 0;
          }
          state.lastSample = audioData[audioBuffer.getNumSamples() - 1];
          state.wasNoteOn = true;
        }
        

      }
  else if(!state.wasNoteOn && state.releaseCounter > 0)
  {
    int releaseLength = *releaseParameter * currentSampleRate;
    float releaseGradient = state.initialReleaseValue / ((float)releaseLength);
    audioData[0] = state.lastSample - releaseGradient;
    
    if (firstIndex>0 && releaseLength - state.releaseCounter > firstIndex)
    {
      for (int index = 0; index < firstIndex; index++) {
        audioData[index] = audioData[index - 1] - releaseGradient;
        state.releaseCounter++;
      }
      state.releaseCounter = 0;
      state.lastSample = audioData[audioBuffer.getNumSamples() -1];
    }
    else if(releaseLength - state.releaseCounter < audioBuffer.getNumSamples())
    {
      int index = 1;
      while (state.releaseCounter < releaseLength) {
        audioData[index] = audioData[index - 1] - releaseGradient;
        state.releaseCounter++;
        index++;
      }
      state.releaseCounter = 0;
      state.lastSample = 0;
    }
    else
    {
      for (int index = 1; index < audioBuffer.getNumSamples(); index++) {
        audioData[index] = audioData[index - 1] - releaseGradient;
        state.releaseCounter++;
      }
      state.lastSample = audioData[audioBuffer.getNumSamples() -1];
    }
    
  }
  
  
//  //check if there are midi events
//  if(midiBuffer.isEmpty())
//    return;

  
    //FILL CURRENT BUFFER
    while (iterator->getNextEvent(message1, samplePosition1))
      {
        //NOTE ON as in ATTACK
        if(message1.isNoteOn())
          {
            int attackLength = *attackParameter * currentSampleRate;
            int decayLength = *decayParameter * currentSampleRate;
            float attackGradient = (1.0)/((float)attackLength);
            float decayGradient = (1.0 - *sustainParameter)/((float)decayLength);
            
            state.releaseCounter = 0;
            
            //there is a next midi message after the current one
            if(iterator->getNextEvent(message2, samplePosition2))
              {
                for(int index = samplePosition1; index < samplePosition2; index++)
                  {
                    if(state.attackDecayCounter < attackLength)
                      {
                        //attack
                        
                        audioData[index] = audioData[index-1] + attackGradient;
                        state.attackDecayCounter++;
                        
                      }
                    else if(state.attackDecayCounter < attackLength + decayLength)
                      {
                        //decay
                        
                        audioData[index]= audioData[index-1] - decayGradient;
                        state.attackDecayCounter++;
                        
                      }
                    else{
                        //set to sustain
                        audioData[index] = *sustainParameter;
                      }
                    
                  }
                
                //update state
                if(attackLength + decayLength <= state.attackDecayCounter)
                {
                  state.attackDecayCounter =0;
                }
                
                state.lastSample = audioData[samplePosition2 - 1];
                state.wasNoteOn = true;
              }
            
            // No next midi message in the buffer
            else{
                
                for(int index = samplePosition1; index < audioBuffer.getNumSamples(); index++)
                  {
                    if(state.attackDecayCounter - decayLength < attackLength)
                    {
                      //attack
                      DBG("first attack " << audioData[index]);
                      audioData[index] = audioData[index-1] + attackGradient;
                      state.attackDecayCounter++;
                      
                    }
                    else if(state.attackDecayCounter < attackLength + decayLength)
                    {
                      //decay
                      
                      audioData[index]= audioData[index-1] - decayGradient;
                      state.attackDecayCounter++;
                      
                    }
                    else{
                      //set to sustain
                      audioData[index] = *sustainParameter;
                    }
                      
                  }
                
                //update state
                if(attackLength + decayLength <= state.attackDecayCounter)
                  {
                    state.attackDecayCounter =0;
                  }
                state.lastSample = audioData[audioBuffer.getNumSamples() - 1];
                state.wasNoteOn = true;
              }
            
          }
        //NOTE OFF as in RELEASE
        else if(message1.isNoteOff())
          {
            int releaseLength = *releaseParameter * currentSampleRate;
            float releaseGradient = state.lastSample/((float)releaseLength);
            state.initialReleaseValue = state.lastSample;
            state.releaseCounter = 0;
            state.attackDecayCounter = 0;
            
            //there is a next midi message after the current one
            if(iterator->getNextEvent(message2, samplePosition2))
              {
                for(int index = samplePosition1; index < samplePosition2; index++)
                  {
                    if(state.releaseCounter < releaseLength)
                    {
                      audioData[index] = audioData[index-1] - releaseGradient;
                      state.releaseCounter++;
                    }
                  }
                //update state
                if(releaseLength <= state.releaseCounter)
                {
                  state.releaseCounter = 0;
                }
                state.lastSample = audioData[samplePosition2 - 1];
                state.wasNoteOn = false;
                
              }
            // No next midi message in the buffer
            else{
              
                for(int index = samplePosition1; index < audioBuffer.getNumSamples(); index++)
                  {
                    if(state.releaseCounter < releaseLength)
                    {
                      audioData[index] = audioData[index-1] - releaseGradient;
                      state.releaseCounter++;
                    }
                  }
                //update state
              if(releaseLength <= state.releaseCounter)
              {
                state.releaseCounter = 0;
              }
                state.lastSample = audioData[audioBuffer.getNumSamples() - 1];
                state.wasNoteOn = false;
              }
          }
      }
  
  float * pointer = audioBuffer.getWritePointer(0);
  //DBG("second sample" << pointer[1]);
  for (int i = 0; i < audioBuffer.getNumSamples(); i++)
  {
    //if(pointer[i]==0){
    //DBG("env" <<  2* (pointer[i]) - 1 << i);
    //}
    pointer[i] = 2* (pointer[i]) - 1;
//    if(i == 128)
//      DBG("envelope" << pointer[i]);
    
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
