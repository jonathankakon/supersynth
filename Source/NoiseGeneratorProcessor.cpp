/*
  ==============================================================================

    NoiseGeneratorProcessor.cpp
    Created: 27 Nov 2016 8:03:55pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "NoiseGeneratorProcessor.h"
#include "NoiseGeneratorProcessorEditor.h"

NoiseGeneratorProcessor::NoiseGeneratorProcessor(): AudioProcessor(BusesProperties()
                                                                   .withOutput("Audio", AudioChannelSet::mono()))
{
  // dont change the order of the parameters here, because the Editor depends on it!
  addParameter(volumeParam = new AudioParameterFloat("volume",
                                                     "Volume",
                                                     NormalisableRange<float>(0.0,1),
                                                     0.2f));
  
  addParameter(noisetypeParam = new AudioParameterChoice("noisetype",
               "Noisetype",
               StringArray({"White","Pink"}),
               0) );
}

NoiseGeneratorProcessor::~NoiseGeneratorProcessor()
{
  
}

void NoiseGeneratorProcessor::prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/)
{
  
}

void NoiseGeneratorProcessor::releaseResources()
{
  
}

void NoiseGeneratorProcessor::audioProcessorParameterChanged(AudioProcessor* /*processor*/, int /*parameterIndex*/, float /*newValue*/)
{
  
}

void NoiseGeneratorProcessor::processBlock(AudioSampleBuffer &buffer, juce::MidiBuffer &/*midiBuffer*/)
{
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
  
  fillBufferWhiteNoise(outBuffer);
  
  outBuffer.applyGain(volumeParam->get());
}

AudioProcessorEditor* NoiseGeneratorProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<NoiseGeneratorProcessor, NoiseGeneratorProcessorEditor>(this);
}

bool NoiseGeneratorProcessor::hasEditor() const
{
  return true;
}

bool NoiseGeneratorProcessor::supportsDoublePrecisionProcessing() const
{
  return false;
}

const String NoiseGeneratorProcessor::getName() const
{
  return String();
}

bool NoiseGeneratorProcessor::acceptsMidi() const
{
  return false;
}

bool NoiseGeneratorProcessor::producesMidi() const
{
  return false;
}

double NoiseGeneratorProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int NoiseGeneratorProcessor::getNumPrograms()
{
  return 1;
}

int NoiseGeneratorProcessor::getCurrentProgram()
{
  return 0;
}

void NoiseGeneratorProcessor::setCurrentProgram(int /*index*/)
{
  
}

const String NoiseGeneratorProcessor::getProgramName(int /*index*/)
{
  return String();
}

void NoiseGeneratorProcessor::changeProgramName(int /*index*/, const juce::String &/*newName*/)
{
  
}

void NoiseGeneratorProcessor::getStateInformation(juce::MemoryBlock &/*destData*/)
{
  
}

void NoiseGeneratorProcessor::setStateInformation(const void* /*data*/, int /*sizeInBytes*/)
{
  
}


  //======================================================================
  //privates:

void NoiseGeneratorProcessor::fillBufferWhiteNoise(AudioBuffer<float> &buffer)
{
  float* const data = buffer.getWritePointer(0);

  for(int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
  {
    data[sampleIndex] = (2 * rand.nextFloat()) - 1;
  }
}



