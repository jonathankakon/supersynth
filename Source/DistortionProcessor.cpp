/*
  ==============================================================================

    DistortionProcessor.cpp
    Created: 2 Dec 2016 1:09:17pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "DistortionProcessor.h"
#include "DistortionProcessorEditor.h"

DistortionProcessor::DistortionProcessor() : AudioProcessor(BusesProperties()
                                                            .withOutput("Audio Output", AudioChannelSet::mono())
                                                            .withInput("Audio Input", AudioChannelSet::mono())),
                                             distorter(new Distorter())
{
  // dont change the order of the parameters here, because the Editor depends on it!
  addParameter(preGainParam = new AudioParameterFloat("preGain",
                                                     "PreGain",
                                                     NormalisableRange<float>(1.0f, 800.0f, 1.0f, 0.7f, false),
                                                     1.0f));
  addParameter(distortionTypeParam = new AudioParameterChoice("distType",
                                                      "DistortionType",
                                                      StringArray({"hardclip", "tanhApprox"}),
                                                      0));
  addParameter(postGainParam = new AudioParameterFloat("postGain",
                                                      "PostGain",
                                                      NormalisableRange<float>(0.0,1.0),
                                                      0.5f));
  
  currentDistortionType = hardclip;
  addListener(this);
  
}

DistortionProcessor::~DistortionProcessor()
{
  
}

void DistortionProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  distorter->setSampleRate(sampleRate);
  distorter->setBufferSize(samplesPerBlock);
}

void DistortionProcessor::releaseResources()
{
  
}

void DistortionProcessor::audioProcessorParameterChanged(AudioProcessor *processor, int parameterIndex, float newValue)
{
  ignoreUnused(processor);
  
  if(parameterIndex == 1)
  {
    setDistType(distortionTypeParam->getIndex());
  }
}

void DistortionProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiBuffer)
{
  AudioBuffer<float> inBuffer = getBusBuffer(buffer, true, 0);
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
  
  inBuffer.applyGain(preGainParam->get());
  
  if(currentDistortionType == hardclip)
  {
    distorter->processHardclip(inBuffer);
  }
  else if (currentDistortionType == tanhApprox)
  {
    distorter->processTanhAprx(inBuffer);
  }
  
  inBuffer.applyGain(postGainParam->get());
  
  outBuffer.copyFrom(0, 0, inBuffer, 0, 0, inBuffer.getNumSamples());

  setCurrentRMS(outBuffer.getRMSLevel(0, 0, outBuffer.getNumSamples()));
}

AudioProcessorEditor* DistortionProcessor::createEditor()
{
    return new ProcessorEditorWithConnectors<DistortionProcessor, DistortionProcessorEditor>(this);
}

bool DistortionProcessor::hasEditor() const
{
  return true;
}

bool DistortionProcessor::supportsDoublePrecisionProcessing() const
{
  return false;
}

const String DistortionProcessor::getName() const
{
  return "aDistortion";
}

bool DistortionProcessor::acceptsMidi() const
{
  return false;
}

bool DistortionProcessor::producesMidi() const
{
  return false;
}

double DistortionProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int DistortionProcessor::getNumPrograms()
{
  return 1;
}

int DistortionProcessor::getCurrentProgram()
{
  return 0;
}

void DistortionProcessor::setCurrentProgram(int index)
{
  
}

const String DistortionProcessor::getProgramName(int index)
{
  return "DistortionProcessor";
}

void DistortionProcessor::changeProgramName(int index, const juce::String &newName)
{
  
}

void DistortionProcessor::getStateInformation(juce::MemoryBlock &destData)
{
  
}

void DistortionProcessor::setStateInformation(const void *data, int sizeInBytes)
{
  
}

  //=======================================================================
  //privates

void DistortionProcessor::setDistType(int index)
{
  switch (index)
  {
    case 0:
      currentDistortionType = hardclip;
      break;
      
    case 1:
      currentDistortionType = tanhApprox;
      break;
  }
}






