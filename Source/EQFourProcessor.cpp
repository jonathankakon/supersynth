/*
  ==============================================================================

    EQfourProcessor.cpp
    Created: 2 Nov 2016 4:04:39pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "EQfourProcessor.h"
#include "FilterProcessor.h"
#include "GenericIIRFilter.h"
#include "EQFourProcessorEditor.h"

EQFourProcessor::EQFourProcessor() : AudioProcessor(BusesProperties()
                                                    .withInput("Control", AudioChannelSet::mono())
                                                    .withOutput("Audio", AudioChannelSet::mono())
                                                    .withInput("Audio", AudioChannelSet::mono()))
{
  addParameter(cutoffFreq1 = new AudioParameterFloat("Cutoff Frequency 1", "Cutoff", 50, 10000, 50));
  addParameter(cutoffFreq2 = new AudioParameterFloat("Cutoff Frequency 2", "Cutoff", 50, 10000, 1000));
  addParameter(cutoffFreq3 = new AudioParameterFloat("Cutoff Frequency 3", "Cutoff", 50, 15000, 5000));
  addParameter(cutoffFreq4 = new AudioParameterFloat("Cutoff Frequency 4", "Cutoff", 50, 15000, 10000));
  
  addParameter(qParam1 = new AudioParameterFloat("Q 1", "Q", 0.1, 6, 0.72));
  addParameter(qParam2 = new AudioParameterFloat("Q 2", "Q", 0.1, 6, 0.72));
  addParameter(qParam3 = new AudioParameterFloat("Q 3", "Q", 0.1, 6, 0.72));
  addParameter(qParam4 = new AudioParameterFloat("Q 4", "Q", 0.1, 6, 0.72));
  
  addParameter(gainParam1 = new AudioParameterFloat("Gain 1", "Gain", -12, 12, 0));
  addParameter(gainParam2 = new AudioParameterFloat("Gain 2", "Gain", -12, 12, 0));
  addParameter(gainParam3 = new AudioParameterFloat("Gain 3", "Gain", -12, 12, 0));
  addParameter(gainParam4 = new AudioParameterFloat("Gain 4", "Gain", -12, 12, 0));

//  *gainParam1 = 0;
//  *gainParam2 = 0;
//  *gainParam3 = 0;
//  *gainParam4 = 0;
  
  addParameter(filterTypeBand1 = new AudioParameterChoice("filterType 1", "Filter Type", {"1st Lowcut", "2nd Lowcut", "Lowshelf"}, 2));
  addParameter(filterTypeBand2 = new AudioParameterChoice("filterType 2", "Filter Type", {"Peak", "Notch"},0));
  addParameter(filterTypeBand3 = new AudioParameterChoice("filterType 3", "Filter Type", {"Peak", "Notch"},0));
  addParameter(filterTypeBand4 = new AudioParameterChoice("filterType 4", "Filter Type", {"1st Highcut", "2nd Highcut", "Highshelf"}, 2));
  
  filterBand1 = new GenericIIRFilter(*cutoffFreq1, *qParam1, *gainParam1);
  filterBand2 = new GenericIIRFilter(*cutoffFreq2, *qParam2, *gainParam2);
  filterBand3 = new GenericIIRFilter(*cutoffFreq3, *qParam3, *gainParam3);
  filterBand4 = new GenericIIRFilter(*cutoffFreq4, *qParam4, *gainParam4);
  
  addListener(this);
}

EQFourProcessor::~EQFourProcessor()
{
}

void EQFourProcessor::prepareToPlay(double sampleRate , int samplesPerBlock)
{
  ignoreUnused(sampleRate, samplesPerBlock);
  currentSampleRate = sampleRate;
  filterBand1->updateSampleRate(sampleRate);
  filterBand2->updateSampleRate(sampleRate);
  filterBand3->updateSampleRate(sampleRate);
  filterBand4->updateSampleRate(sampleRate);
}

void EQFourProcessor::releaseResources()
{
}

void EQFourProcessor::processBlock(AudioSampleBuffer & buffer, juce::MidiBuffer &midiBuffer)
{
  ignoreUnused(midiBuffer);
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
  
  switch (filterTypeBand1->getIndex()) {
    case 0:
      filterBand1->firstOrderHighPass(outBuffer);
      break;
      
    case 1:
      filterBand1->secondOrderHighPass(outBuffer);
      break;
      
    case 2:
      filterBand1->lowShelf(outBuffer);
      break;
      
    default:
      break;
  }
  
  switch (filterTypeBand2->getIndex()) {
    case 0:
      filterBand2->peak(outBuffer);
      //applyEQBand(outBuffer, filterBand2, gainParam2);
      break;
      
    case 1:
      filterBand2->canonicalBandstop(outBuffer);
      break;
      
    default:
      break;
  }
  
  switch (filterTypeBand3->getIndex()) {
    case 0:
      filterBand3->peak(outBuffer);
      //applyEQBand(outBuffer, filterBand3, gainParam3);
      break;
      
    case 1:
      filterBand3->canonicalBandstop(outBuffer);
      break;
      
    default:
      break;
  }
  
  switch (filterTypeBand4->getIndex()) {
    case 0:
      filterBand4->firstOrderLowPass(outBuffer);
      break;
      
    case 1:
      filterBand4->secondOrderLowPass(outBuffer);
      break;
      
    case 2:
      filterBand4->highShelf(outBuffer);
      break;
      
    default:
      break;
  }
  
  
}

void EQFourProcessor::applyEQBand(AudioBuffer<float> &buffer, GenericIIRFilter* filter, AudioParameterFloat* gain)
{
  ScopedPointer<AudioBuffer<float>> filtered = new AudioBuffer<float>();
  filtered->makeCopyOf(buffer);
  filter->canonicalBandPass(buffer);
  
  float* filterPointer = filtered->getWritePointer(0);
  
  if(filter == filterBand2)
  {
  for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
  {
    *pointer = *pointer + ((*gainParam2)*(*filterPointer));
    filterPointer++;
  }
  }
  if (filter == filterBand3) {
    for(float* pointer = buffer.getWritePointer(0); pointer < (buffer.getWritePointer(0)+buffer.getNumSamples()); pointer++)
    {
      *pointer = *pointer + ((*gainParam3)*(*filterPointer));
      filterPointer++;
    }
  }
}

void EQFourProcessor::audioProcessorParameterChanged(AudioProcessor* processor, int parameterIndex, float newValue)
{
  switch (parameterIndex) {
    case 0:
      filterBand1->setCutoff(newValue);
      break;
      
    case 1:
      filterBand2->setCutoff(newValue);
      break;
      
    case 2:
      filterBand3->setCutoff(newValue);
      break;
      
    case 3:
      filterBand4->setCutoff(newValue);
      break;
      
    case 4:
      filterBand1->setQ(newValue);
      break;
      
    case 5:
      filterBand2->setQ(newValue);
      break;
      
    case 6:
      filterBand3->setQ(newValue);
      break;
      
    case 7:
      filterBand4->setQ(newValue);
      break;
      
    case 8:
      filterBand1->setGain(newValue);
      break;
      
    case 9:
      filterBand2->setGain(newValue);
      break;
      
    case 10:
      filterBand3->setGain(newValue);
      break;
      
    case 11:
      filterBand4->setGain(newValue);
      break;
      
    default:
      break;
  }
}

AudioProcessorEditor* EQFourProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<EQFourProcessor, EQFourProcessorEditor>(this);
}

bool EQFourProcessor::hasEditor() const
{
  return true;
}

void EQFourProcessor::changeFilterTypeBand1(int newIndex){
  *filterTypeBand1 = newIndex;
}

void EQFourProcessor::changeFilterTypeBand2(int newIndex){
  *filterTypeBand2 = newIndex;
}

void EQFourProcessor::changeFilterTypeBand3(int newIndex){
  *filterTypeBand3 = newIndex;
}

void EQFourProcessor::changeFilterTypeBand4(int newIndex){
  *filterTypeBand4 = newIndex;
}

const String EQFourProcessor::getName() const
{
  return "EQ four";
}

bool EQFourProcessor::acceptsMidi() const
{
  return false;
}

bool EQFourProcessor::producesMidi() const
{
  return false;
}

double EQFourProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int EQFourProcessor::getNumPrograms()
{
  return 1;
}

int EQFourProcessor::getCurrentProgram()
{
  return 0;
}

void EQFourProcessor::setCurrentProgram(int index)
{
  ignoreUnused(index);
}

const String EQFourProcessor::getProgramName(int index)
{
  ignoreUnused(index);
  return String();
}

void EQFourProcessor::changeProgramName(int index, const juce::String &newName)
{
  ignoreUnused(index, newName);
}

void EQFourProcessor::getStateInformation(juce::MemoryBlock &destData)
{
  ignoreUnused(destData);
}

void EQFourProcessor::setStateInformation(const void *data, int sizeInBytes)
{
  ignoreUnused(data, sizeInBytes);
}