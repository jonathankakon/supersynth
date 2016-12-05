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


EnvelopeProcessor::EnvelopeProcessor() : AudioProcessor(BusesProperties().withOutput("Envelope", AudioChannelSet::mono()))
{
  addParameter(attackParameter = new AudioParameterFloat("Attack", "Attack",0, 1, 0.001f));
  addParameter(decayParameter= new AudioParameterFloat("Decay", "Decay", 0, 1, 0.001f));
  addParameter(sustainParameter = new AudioParameterFloat("Sustain", "Sustain", 0, 1, 0.9f));
  addParameter(releaseParameter = new AudioParameterFloat("Release", "Release", 0, 1, 0.001f));
  
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
  DBG("last sample " << state.lastSample);
  
    float* const audioData = audioBuffer.getWritePointer(0);
    
    int firstIndex = midiBuffer.getFirstEventTime();
    
    MidiMessage& message1 = *new MidiMessage();
    int samplePosition1 = 0;
    MidiMessage& message2 = *new MidiMessage();
    int samplePosition2 = 0;
    
    ScopedPointer<MidiBuffer::Iterator> iterator = new MidiBuffer::Iterator(midiBuffer);
    
  //OVERLAP FROM PREVIOUS BUFFER
  
  //attack decay & sustain
    if (state.wasNoteOn)
      {
        //calculateLenghtAndGradient();
        
        //there is another sample in the buffer -> first index > gsa0
//        if(firstIndex > 0 )
//        {
//          //initialise first sample of the buffer
//          initialiseFirstSample(audioData);
//          
//          //calculate the remaining samples
//          computeAttackDecaySustain(audioData, firstIndex);
//          
//          //update state
//          if (attackLength+ decayLength < state.attackDecayCounter) {
//            state.attackDecayCounter = 0;
//          }
//          state.lastSample = audioData[firstIndex - 1];
//          state.wasNoteOn = true;
//        }
//        //the buffer is empty
//        else
       // {
          //initialise first sample of the buffer
          initialiseFirstSample(audioData);
          
          //calculate remaining samples
          computeAttackDecaySustain(audioData, audioBuffer.getNumSamples());
          
          //update state
          if (attackLength+ decayLength < state.attackDecayCounter) {
            state.attackDecayCounter = 0;
          }
        
          if(firstIndex > 0)
          {
            state.lastSample = audioData[firstIndex - 1];
          }
          else{
            state.lastSample = audioData[audioBuffer.getNumSamples() - 1];
          }
          state.wasNoteOn = true;
       // }
        

      }
  //release
  else if(!state.wasNoteOn && state.releaseCounter > 0)
  {
    //calculateLenghtAndGradient();
    audioData[0] = state.lastSample - releaseGradient;
    
//    if (firstIndex > 0 && releaseLength - state.releaseCounter > firstIndex)
//    {
//      for (int index = 1; index < firstIndex; index++) {
//        audioData[index] = audioData[index - 1] - releaseGradient;
//        state.releaseCounter++;
//      }
//      state.releaseCounter = 0;
//      state.lastSample = audioData[audioBuffer.getNumSamples() -1];
//    }
    if(releaseLength - state.releaseCounter < audioBuffer.getNumSamples())
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
  
  
  //check if there are midi events
  if(midiBuffer.isEmpty()){
    expandRangeMinusOneToPlusOne(audioBuffer);
    return;
  }
  
    //FILL CURRENT BUFFER
    while (iterator->getNextEvent(message1, samplePosition1))
      {
        //NOTE ON as in ATTACK
        if(message1.isNoteOn())
          {
            calculateLenghtAndGradient();
          
            state.releaseCounter = 0;
            state.attackDecayCounter = 0;
            state.lastNote = message1.getNoteNumber();
            
            //there is a next midi message after the current one
            if(iterator->getNextEvent(message2, samplePosition2))
              {
                computeAttackDecaySustain(audioData, samplePosition2);
                
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
                
              computeAttackDecaySustain(audioData, audioBuffer.getNumSamples());
              
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
        else if(message1.isNoteOff() && state.lastNote == message1.getNoteNumber())
          {
            
            state.initialReleaseValue = state.lastSample;
            
            calculateLenghtAndGradient();
            
            
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
  
  expandRangeMinusOneToPlusOne(audioBuffer);
  
}

void EnvelopeProcessor::initialiseFirstSample(float* audioData){
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
}

void EnvelopeProcessor::calculateLenghtAndGradient(){
  //calculate attack parameters
  if(attackGradient > 0){
  attackLength = (int)(*attackParameter * currentSampleRate) - static_cast<int>(state.lastSample / attackGradient);
  }
  else{
    attackLength = (int)(*attackParameter * currentSampleRate);
  }
  attackGradient = (1.0f-state.lastSample)/static_cast<float>(attackLength);
  
  //calculate decay parameters
  decayLength = (int)(*decayParameter * currentSampleRate);
  decayGradient = (1.0f - *sustainParameter)/static_cast<float>(decayLength);
  
  //calculate release parameters
  releaseLength = (int)(*releaseParameter * currentSampleRate);
  releaseGradient = state.initialReleaseValue/static_cast<float>(releaseLength); //maybe releaseLength - state.releaseCounter
}

void EnvelopeProcessor::computeAttackDecaySustain(float *audioData, int lastIndexForLoop){
  for (int index = 1; index < lastIndexForLoop; index++)
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
  
}

void EnvelopeProcessor::expandRangeMinusOneToPlusOne(AudioBuffer<float> &audioBuffer)
{
  
  
  float * pointer = audioBuffer.getWritePointer(0);
  
  FloatVectorOperations::multiply(pointer, pointer, pointer, audioBuffer.getNumSamples());
  //FloatVectorOperations::multiply(pointer, pointer, pointer, audioBuffer.getNumSamples());
  
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

void EnvelopeProcessor::audioProcessorParameterChanged(juce::AudioProcessor* /*processor*/, int parameterIndex, float newValue)
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
    return "EnvelopeProcessor";
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
