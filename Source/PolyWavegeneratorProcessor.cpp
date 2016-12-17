/*
  ==============================================================================

    PolyWavegeneratorProcessor.cpp
    Created: 9 Dec 2016 11:08:47am
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "PolyWavegeneratorProcessor.h"
#include "PolyWavegeneratorEditor.h"

// Voice Class

PolyWavegeneratorProcessor::Voice::Voice(int bufferSize)
{
  outBuffer = new AudioBuffer<float>(1, bufferSize);
  envelopeBuffer = AudioBuffer<float>(1, bufferSize);
  
  oscillator = new VAOscillator();
  envelope = new EnvelopeProcessor();
  
  isOn = false;
  
}

PolyWavegeneratorProcessor::Voice::~Voice()
{
  
}

AudioBuffer<float>& PolyWavegeneratorProcessor::Voice::getOutBuffer()
{
  return *outBuffer;
}

void PolyWavegeneratorProcessor::Voice::addMidiEvent(const MidiMessage& message, int sampleNumber)
{
  midiInBuffer.addEvent(message, sampleNumber);
}

void PolyWavegeneratorProcessor::Voice::clearMidiBuffer()
{
  midiInBuffer.clear();
}

void PolyWavegeneratorProcessor::Voice::clearAudioBuffer()
{
  outBuffer->clear();
}

void PolyWavegeneratorProcessor::Voice::fillBufferEnvelope()
{
  envelope->processBlock(envelopeBuffer, midiInBuffer);
}

void PolyWavegeneratorProcessor::Voice::fillBufferAudio()
{
  Array<int> midiOns; //i know no arrays for primitives :)
  if(!midiInBuffer.isEmpty())
  {
    MidiMessage& message1 = *new MidiMessage();
    ScopedPointer<MidiBuffer::Iterator> iterator = new MidiBuffer::Iterator(midiInBuffer);
    int i = 0;
    while(iterator->getNextEvent(message1, i))
    {
      if(message1.isNoteOn()){
        if(int pos = message1.getNoteNumber())
        {
          oscillator->setFrequency(midiToFreq[pos]);
          midiOns.add(i);
        }
      }
    }
  }
  
  oscillator->fillBufferSine(*outBuffer, envelopeBuffer, midiOns);
  
}

void PolyWavegeneratorProcessor::Voice::prepareTheEnvelope(double sampleRate, int samplesPerBlock)
{
  envelope->prepareToPlay(sampleRate, samplesPerBlock);
}

// Polywavegenerator Class

PolyWavegeneratorProcessor::PolyWavegeneratorProcessor()
{
  voices = OwnedArray<Voice>();
  voices.clear();
}

PolyWavegeneratorProcessor::~PolyWavegeneratorProcessor()
{
  
}

void PolyWavegeneratorProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  for(int index = 0; index < 4; index++)
  {
    voices.add(new Voice(samplesPerBlock));
  }
  for(int index = 0; index < voices.size(); index++)
  {
    voices[index]->prepareTheEnvelope(sampleRate, samplesPerBlock);
  }
}


void PolyWavegeneratorProcessor::releaseResources()
{
  
}

void PolyWavegeneratorProcessor::audioProcessorParameterChanged(AudioProcessor *processor, int parameterIndex, float newValue)
{
  
}

void PolyWavegeneratorProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiBuffer)
{
  // clear all the midibuffers of the voices because they still contain the events from the last process Block
  for(int index = 0; index < voices.size(); index++)
  {
    voices[index]->clearMidiBuffer();
  }
  
  // Midi and Voice Handler this is not correct yet i need to watch out for different note ons of the same note in one buffer and other stuff
  if(takesMidi && !midiBuffer.isEmpty())
  {
    MidiMessage& message1 = *new MidiMessage();
    ScopedPointer<MidiBuffer::Iterator> iterator = new MidiBuffer::Iterator(midiBuffer);
    int sampleInBuffer = 0;
    while(iterator->getNextEvent(message1, sampleInBuffer))
    {
      if(message1.isNoteOn())
      {
        // always take the first one and move it to the back => the oldest voice will be "overwritten"
        voices[0]->setIsOn(true);
        voices[0]->setMidiNote(message1.getNoteNumber());
        voices[0]->addMidiEvent(message1, sampleInBuffer);
        
        voices.move(0, voices.size()-1);
      }
      else if(message1.isNoteOff())
      {
        for(int index = 0; index < voices.size(); index++)
        {
          if(voices[index]->getMidiNote() == message1.getNoteNumber())
          {
            ScopedPointer<Voice> tempVoice = voices[index];
            
            tempVoice->setIsOn(false);
            tempVoice->addMidiEvent(message1, sampleInBuffer);
            tempVoice->setMidiNote(-1); // this should be removed but just in case for now
            
            break;
          }
        }
      }
    }
  }
  
  
  
  // Audio Handling of the voices
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
  
  int numActive = 0; // eventually this could be a member variable
  for(int index = 0; index < voices.size(); index++)
  {
    if(voices[index]->getIsOn())
    {
      numActive++;
      
      voices[index]->clearAudioBuffer();
      voices[index]->fillBufferEnvelope();
      voices[index]->fillBufferAudio();
      
      outBuffer.addFrom(0, 0, voices[index]->getOutBuffer(), 0, 0, outBuffer.getNumSamples());
    }
  }
  
  outBuffer.applyGain(1.0f/numActive);
  
  
}

AudioProcessorEditor* PolyWavegeneratorProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<PolyWavegeneratorProcessor, PolyWavegeneratorEditor>(this);
}
