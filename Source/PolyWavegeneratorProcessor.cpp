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

PolyWavegeneratorProcessor::Voice::Voice()
{
  
}

PolyWavegeneratorProcessor::Voice::~Voice()
{
  
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
  outBuffer.clear();
}

void PolyWavegeneratorProcessor::Voice::fillBufferEnvelope()
{
  envelope.processBlock(envelopeBuffer, midiInBuffer);
}

void PolyWavegeneratorProcessor::Voice::fillBufferAudio()
{
  // oscillator.fillBufferFallingSaw(outBuffer, null, envelopeBuffer, null);
}


// Polywavegenerator Class

PolyWavegeneratorProcessor::PolyWavegeneratorProcessor()
{
  
}

PolyWavegeneratorProcessor::~PolyWavegeneratorProcessor()
{
  
}

void PolyWavegeneratorProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  
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
  
  // Midi and Voice Handler
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
        ScopedPointer<Voice> tempVoice = voices.removeAndReturn(0);
        
        tempVoice->setIsOn(true);
        tempVoice->setMidiNote(message1.getNoteNumber());
        tempVoice->addMidiEvent(message1, sampleInBuffer);
        
        voices.add(tempVoice);
      }
      else if(message1.isNoteOff())
      {
        for(int index = 0; index < voices.size(); index++)
        {
          if(voices[index]->getMidiNote() == message1.getNoteNumber())
          {
            ScopedPointer<Voice> tempVoice = voices[index];
            
            tempVoice->addMidiEvent(message1, sampleInBuffer);
            
            break;
          }
        }
      }
    }
  }
  
  
  
  // Audio Handling of the voices
  for(int index = 0; index < voices.size(); index++)
  {
    ScopedPointer<Voice> tempVoice = voices.removeAndReturn(0);
    
    tempVoice->clearAudioBuffer();
    
    tempVoice->fillBufferEnvelope();
    tempVoice->fillBufferAudio();
  }
  
  
}

AudioProcessorEditor* PolyWavegeneratorProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<PolyWavegeneratorProcessor, PolyWavegeneratorEditor>(this);
}
