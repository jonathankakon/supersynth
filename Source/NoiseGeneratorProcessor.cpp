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
                                                                   .withOutput("Audio", AudioChannelSet::mono())
                                                                   .withInput("volumeControl", AudioChannelSet::mono()))
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
  
  resetFilterState();
  setFilterCoefficients();
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
  AudioBuffer<float> volBuffer = getBusBuffer(buffer,true, 0);
  
  float* outPointer = outBuffer.getWritePointer(0);
  int numSamples = outBuffer.getNumSamples();
  
  ScopedPointer<AudioBuffer<float>> volModBuffer = new AudioBuffer<float>();
  volModBuffer->makeCopyOf(buffer);
  float* modPointer = volModBuffer->getWritePointer(0);
  FloatVectorOperations::add(modPointer, 1, numSamples);
  FloatVectorOperations::multiply(modPointer, 0.5, numSamples);
  
  switch(noisetypeParam->getIndex()){
case 0:
  fillBufferWhiteNoise(outBuffer);
      FloatVectorOperations::multiply(outPointer, outPointer, modPointer, numSamples);
  break;
case 1:
  fillBufferWhiteNoise(outBuffer);
  applyPinkNoiseFilter(outBuffer);
      FloatVectorOperations::multiply(outPointer, outPointer, modPointer, numSamples);
  break;
  
default:
  break;
  }
  
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

void NoiseGeneratorProcessor::applyPinkNoiseFilter(AudioBuffer<float> &buffer)
{
  float* const data = buffer.getWritePointer(0);
  
  for(int i = 0; i < buffer.getNumSamples(); i++)
  {
    state.xn0 = data[i] - coefficients.a1*state.xn1 - coefficients.a2*state.xn2 - coefficients.a3*state.xn3;
    data[i] = state.xn0 + coefficients.b1*state.xn1 + coefficients.b2*state.xn2 + coefficients.b3*state.xn3;
    
    state.xn3 = state.xn2;
    state.xn2 = state.xn1;
    state.xn1 = state.xn0;
    
    
    
    if(state.xn0 != state.xn0)
      DBG("NaN");
    
  }
}


