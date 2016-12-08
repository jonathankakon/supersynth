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
  
  addListener(this);
  
  computeSteps();
  
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
  
  
  MidiMessage message = MidiMessage();
  int position = audioBuffer.getNumSamples();
  ScopedPointer<MidiBuffer::Iterator> iterator = new MidiBuffer::Iterator(midiBuffer);
  iterator->getNextEvent(message, position);
  
  for(int i = 0; i < audioBuffer.getNumSamples(); i++)
  {
    if(position == i)
    {
      iterator->getNextEvent(message, position);
    }
    slope = getNextSlopeStateFromLastSample(message, position, i);
    
    switch (slope)
    {
      case EnvelopeProcessor::EnvelopeSlopeState::attack:
        audioData[i] = nextSample;
        nextSample += attackStep;
        break;
      case EnvelopeProcessor::EnvelopeSlopeState::decay:
        audioData[i] = nextSample;
        nextSample -= decayStep;
        break;
      case EnvelopeProcessor::EnvelopeSlopeState::sustain:
        audioData[i] = nextSample;
        nextSample = *sustainParameter;
        break;
      case EnvelopeProcessor::EnvelopeSlopeState::release:
        audioData[i] = nextSample;
        nextSample -= releaseStep;
        break;
      default:
      case EnvelopeProcessor::EnvelopeSlopeState::none:
        audioData[i] = 0;
        nextSample = 0;
        break;
    }
  }
  
  setCurrentRMS(audioBuffer.getRMSLevel(0, 0, audioBuffer.getNumSamples()));
  expandRangeMinusOneToPlusOne(audioBuffer);
}

EnvelopeProcessor::EnvelopeSlopeState EnvelopeProcessor::getNextSlopeStateFromLastSample(MidiMessage message, int position, int i)
{
  bool isMidiPosition = (position == i);
  bool isSameNote = (message.getNoteNumber() == lastPlayingNote);
  
  bool biggerOne = (nextSample >= 1);
  nextSample = (nextSample >= 1) ? 1 : nextSample;
  
  bool smallerZero = (nextSample <= 0);
  nextSample = (nextSample <= 0) ? 0 : nextSample;
  
  bool smallerSustain = (nextSample <= *sustainParameter);
  bool hasDecay = (*sustainParameter < 1);
  
  
  switch (slope)
  {
    case EnvelopeProcessor::EnvelopeSlopeState::attack:
      if(message.isNoteOn() && isMidiPosition)
      {
        lastPlayingNote = message.getNoteNumber();
        return EnvelopeSlopeState::attack;
      }
      else if (message.isNoteOff() && isMidiPosition && isSameNote)
      {
        return EnvelopeSlopeState::release;
      }
      else if (biggerOne)
      {
        return hasDecay ? EnvelopeSlopeState::decay : EnvelopeSlopeState::sustain;
      }
      else
      {
        return EnvelopeSlopeState::attack;
      }
      break;
      
    case EnvelopeProcessor::EnvelopeSlopeState::decay:
      
      if(message.isNoteOn() && isMidiPosition)
      {
        lastPlayingNote =  message.getNoteNumber();
        return EnvelopeSlopeState::attack;
      }
      else if (message.isNoteOff() && isMidiPosition && isSameNote)
      {
        return EnvelopeSlopeState::release;
      }
      else if (smallerSustain)
      {
        nextSample = (nextSample <= *sustainParameter) ? *sustainParameter : nextSample;
        return EnvelopeSlopeState::sustain;
      }
      else
      {
        return EnvelopeSlopeState::decay;
      }
      break;
      
    case EnvelopeProcessor::EnvelopeSlopeState::sustain:
      
      if(message.isNoteOn() && isMidiPosition)
      {
        lastPlayingNote = message.getNoteNumber();
        return (hasDecay) ? EnvelopeSlopeState::attack : EnvelopeSlopeState::sustain;
      }
      else if (message.isNoteOff() && isMidiPosition && isSameNote)
      {
        return EnvelopeSlopeState::release;
      }
      else
      {
        return EnvelopeSlopeState::sustain;
      }
      break;
      
    case EnvelopeProcessor::EnvelopeSlopeState::release:
      
      if(message.isNoteOn() && isMidiPosition)
      {
        lastPlayingNote = message.getNoteNumber();
        return EnvelopeSlopeState::attack;
      }
      else if (smallerZero)
      {
        return EnvelopeSlopeState::none;
      }
      else
      {
        return EnvelopeSlopeState::release;
      }
      break;
      
    case EnvelopeProcessor::EnvelopeSlopeState::none:
      
      if (message.isNoteOn() && isMidiPosition) {
        lastPlayingNote = message.getNoteNumber();
        return EnvelopeSlopeState::attack;
      }
      break;
  }
  return EnvelopeProcessor::EnvelopeSlopeState::none;
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
  
  computeSteps();

}

void EnvelopeProcessor::computeSteps()
{
  attackStep = 1/(currentSampleRate * (*attackParameter));
  decayStep = (1- *sustainParameter)/(currentSampleRate * (*decayParameter));
  releaseStep = (*sustainParameter)/(currentSampleRate * (*releaseParameter));
  if(*sustainParameter == 0)
  {
    releaseStep = 1/(currentSampleRate * (*releaseParameter));
  }
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
