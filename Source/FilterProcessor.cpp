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
#include "Constants.h"

FilterProcessor::FilterProcessor() : AudioProcessor(BusesProperties()
  .withInput("Audio", AudioChannelSet::mono())
  .withOutput("Audio", AudioChannelSet::mono())
  .withInput("Control", AudioChannelSet::mono())), types(new StringArray)
{
  addParameter(cutoffFreqency = new AudioParameterFloat("cutoffFrequency", "Cutoff", 50, 15000, 100));
  addParameter(qParameter = new AudioParameterFloat("qParameter", "Q", 0.1f, 6, 0.72f));
  addParameter(gainParameter = new AudioParameterFloat("gainParameter2", "Gain", -12, 12, 0));
  filterIIR = new GenericIIRFilter(*cutoffFreqency, *qParameter, *gainParameter);
  
  
  int size = sizeof(church)/sizeof(*church);
  
  filterFIR = new FIRFilter(church, size);
  
  
  
  types->add("bypass");
  types->add("lowpass");
  types->add("highpass");
  types->add("bandpass");
  types->add("bandstop");
  types->add("second order lowpass");
  types->add("second order highpass");
  types->add("canonical bandpass");
  types->add("canonical bandstop");
  types->add("lowshelf");
  types->add("highshelf");
  types->add("FIR");
  
  
  addParameter(filterType = new AudioParameterChoice("filterType", "Filter Type", *types, 0));

  AudioProcessor::addListener(this);
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
  delete[] types;
}

void FilterProcessor::processBlock(AudioSampleBuffer & buffer, juce::MidiBuffer & midiBuffer)
{
  
  /* types: 0 = Bypass
            1 = IIR lowpass
            2 = IIR highpass
            3 = IIR bandpass
            4 = IIR bandstop
            5 = IIR canonical second order lowpass
            6 = IIR canonical second order highpass
            7 = IIR canonical bandpass
            8 = IIR canonical bandstop
            9 = IIR first order lowshelf
           10 = IIR first order highshelf
   */
  
  ignoreUnused(midiBuffer);
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
  AudioBuffer<float> modBuffer = getBusBuffer(buffer, true, 1);
  
  
  //*filterType = 2;
  
  switch (filterType->getIndex()) {
    case 0:
      break;
      
    case 1:
      filterIIR->firstOrderLowPass(outBuffer, modBuffer);
      break;
      
    case 2:
      filterIIR->firstOrderHighPass(outBuffer, modBuffer);
      break;
      
    case 3:
      filterIIR->bandpass(outBuffer, modBuffer);
      break;
      
    case 4:
      filterIIR->bandstop(outBuffer, modBuffer);
      break;
      
    case 5:
      filterIIR->secondOrderLowPass(outBuffer, modBuffer);
      break;
      
    case 6:
      filterIIR->secondOrderHighPass(outBuffer, modBuffer);
      break;
      
    case 7:
      filterIIR->canonicalBandPass(outBuffer, modBuffer);
      break;
      
    case 8:
      filterIIR->canonicalBandstop(outBuffer, modBuffer);
      break;
      
    case 9:
      filterIIR->lowShelf(outBuffer, modBuffer);
      break;
      
    case 10:
      filterIIR->highShelf(outBuffer, modBuffer);
      break;
      
    case 11:
      filterFIR->applyFIRFilter(outBuffer);
      
    default:
      break;
  }
  
  

  setCurrentRMS(outBuffer.getRMSLevel(0, 0, outBuffer.getNumSamples()));
//  if(filterType->getIndex() == 0)
//  {
//    filterIIR->firstOrderLowPass(outBuffer);
//  }
//  else if(filterType->getIndex() == 1)
//  {
//    filterIIR->firstOrderHighPass(outBuffer);
//  }
//  else if (filterType->getIndex() == 2)
//  {
//    filterIIR->bandpass(outBuffer);
//  }
//  else if (filterType->getIndex() == 3)
//  {
//    filterIIR->bandstop(outBuffer);
//  }
}

void FilterProcessor::audioProcessorParameterChanged(AudioProcessor* /*processor*/, int parameterIndex, float newValue)
{
  switch (parameterIndex) {
    case 0:
      filterIIR->setCutoff(newValue);
      break;
    case 1:
      filterIIR->setQ(newValue);
      break;
    case 2:
      filterIIR->setGain(newValue);
      break;
    default:
      break;
  }
}

AudioProcessorEditor* FilterProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<FilterProcessor, FilterProcessorEditor>(this);
}

bool FilterProcessor::hasEditor() const
{
  return true;
}

const String FilterProcessor::getName() const
{
  return "FilterProcessor";
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