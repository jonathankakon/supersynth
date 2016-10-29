/*
  ==============================================================================

    FilterProcessor.cpp
    Created: 20 Oct 2016 4:38:12pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "FilterProcessor.h"
#include "FilterProcessorEditor.h"
#include "GenericIIRFilter.h"

FilterProcessor::FilterProcessor() : AudioProcessor(BusesProperties()
  .withInput("Control", AudioChannelSet::mono())
  .withOutput("Audio", AudioChannelSet::mono())
  .withInput("Audio", AudioChannelSet::mono()))
{
  addParameter(cutoffFreqency = new AudioParameterFloat("cutoffFrequency", "Cutoff", 100, 5000, 1000));
  addParameter(qParameter = new AudioParameterFloat("qParameter", "Q", 0.1, 3, 0.72));
  filterIIR = new GenericIIRFilter(*cutoffFreqency, *qParameter);
  
  types->add("lowpass");
  types->add("highpass");
  types->add("bandpass");
  types->add("bandstop");
  
  addParameter(filterType = new AudioParameterChoice("filterType", "Filter Type", *types, 0));
  
  addListener(this);
}

FilterProcessor::~FilterProcessor()
{
}

void FilterProcessor::prepareToPlay(double sampleRate , int samplesPerBlock)
{
  ignoreUnused(sampleRate, samplesPerBlock);
  currentSampleRate = sampleRate;
  filterIIR->updateSampleRate(sampleRate);
  
}

void FilterProcessor::releaseResources()
{
}

void FilterProcessor::processBlock(AudioSampleBuffer & buffer, juce::MidiBuffer & midiBuffer)
{
  
  /* types: 0 = IIR lowpass
            1 = IIR highpass
            2 = IIR bandpass
            3 = IIR bandstop
   */
  
  ignoreUnused(midiBuffer);
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
//  *filterType = 1;
  
  if(filterType->getIndex() == 0)
  {
    filterIIR->firstOrderLowPass(outBuffer);
  }
  else if(filterType->getIndex() == 1)
  {
    filterIIR->firstOrderHighPass(outBuffer);
  }
  else if (filterType->getIndex() == 2)
  {
    filterIIR->bandpass(outBuffer);
  }
  else if (filterType->getIndex() == 3)
  {
    filterIIR->bandstop(outBuffer);
  }
}

void FilterProcessor::audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue)
{
  switch (parameterIndex) {
    case 0:
      filterIIR->setCutoff(newValue);
      break;
    case 1:
      filterIIR->setQ(newValue);
      DBG("q: " << newValue);
      break;
    default:
      break;
  }
}

AudioProcessorEditor* FilterProcessor::createEditor()
{
  return new FilterProcessorEditor(* this);
}

bool FilterProcessor::hasEditor() const
{
  return true;
}

const String FilterProcessor::getName() const
{
  return "aFilter";
}

bool FilterProcessor::acceptsMidi() const
{
  return false;
}

bool FilterProcessor::producesMidi() const
{
  return false;
}

double FilterProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int FilterProcessor::getNumPrograms()
{
  return 1;
}

int FilterProcessor::getCurrentProgram()
{
  return 0;
}

void FilterProcessor::setCurrentProgram(int index)
{
  ignoreUnused(index);
}

const String FilterProcessor::getProgramName(int index)
{
  ignoreUnused(index);
  return String();
}

void FilterProcessor::changeProgramName(int index, const juce::String &newName)
{
  ignoreUnused(index, newName);
}

void FilterProcessor::getStateInformation(juce::MemoryBlock &destData)
{
  ignoreUnused(destData);
}

void FilterProcessor::setStateInformation(const void *data, int sizeInBytes)
{
  ignoreUnused(data, sizeInBytes);
}





