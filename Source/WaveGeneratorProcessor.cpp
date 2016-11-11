/*
  ==============================================================================

    WaveGeneratorProcessor.cpp
    Created: 7 Oct 2016 3:42:56pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "WaveGeneratorProcessor.h"
#include "WaveGeneratorProcessorEditor.h"

#include "Constants.h"


WaveGeneratorProcessor::WaveGeneratorProcessor() : AudioProcessor(BusesProperties()
  .withOutput("Audio", AudioChannelSet::mono())
  .withInput("FrequencyControl", AudioChannelSet::mono())
  .withInput("VolumeControl", AudioChannelSet::mono())),
  oscillator(new VAOscillator())
{
  
  // dont change the order of the parameters here, because the Editor depends on it!
  addParameter(volumeParam = new AudioParameterFloat("volume",
                                                       "Volume",
                                                       NormalisableRange<float>(0.0,1.0),
                                                       0.2));
  
  addParameter(frequencyParam = new AudioParameterFloat("currentFrequency",
                                                        "Frequency",
                                                        NormalisableRange<float>(100.0, 5000.0, 0.01, 0.7, false),
                                                        440.0f));

  addParameter(octaveParam = new AudioParameterInt("octaves",
                                                   "Octaves",
                                                   0,
                                                   6,
                                                   3)); // from -3 to 3
  
  addParameter(semitonesParam = new AudioParameterInt("semitones",
                                                      "Semitones",
                                                      0,
                                                      24,
                                                      12)); // from -12 to 12
  
  addParameter(centsParam = new AudioParameterInt("cents",
                                                  "Cents",
                                                  0,
                                                  200,
                                                  100)); // from -100 to 100
  
  addParameter(waveformParam = new AudioParameterChoice("waveform",
                                                        "Waveform",
                                                        StringArray({"Sine","SawUp","SawDown","Square","Triangle"}),
                                                        1) );
  
  
  
  addListener(this);
  
  currentWaveform = sawUp;
  
}

WaveGeneratorProcessor::~WaveGeneratorProcessor()
{
  
}



void WaveGeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  // we must set the sample rate of the oscillator object here
  
  ignoreUnused(samplesPerBlock);

  currentSampleRate = sampleRate;
  
  oscillator->setSampleRate(sampleRate);
}

void WaveGeneratorProcessor::releaseResources()
{
}


//you cannot change the parameter values in here => loop
void WaveGeneratorProcessor::audioProcessorParameterChanged(AudioProcessor * processor, int parameterIndex, float newValue)
{
  ignoreUnused(processor);

  if(parameterIndex != 0 && parameterIndex != 5)
  {
    oscillator->setFrequency(frequencyParam->get() * octaves[octaveParam->get()] * semitones[semitonesParam->get()] * cents[centsParam->get()] );
  }
}

void WaveGeneratorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiBuffer) //fills channels 1 and 0
{
  ignoreUnused(midiBuffer);
  
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
  if(waveformParam->getIndex() == 0)
  {
    oscillator->fillBufferSine(outBuffer);
  }
  else if(waveformParam->getIndex() == 1)
  {
    oscillator->fillBufferRisingSaw(outBuffer);
  }
  else if(waveformParam->getIndex() == 2)
  {
    oscillator->fillBufferFallingSaw(outBuffer);
  }
  else if(waveformParam->getIndex() == 3)
  {
    oscillator->fillBufferSquarePulse(outBuffer);
  }
  else // index == 4 => Triangle
  {
    oscillator->fillBufferTriangle(outBuffer);
  }
  
  outBuffer.applyGain(volumeParam->get());
  
}// End processBlock

AudioProcessorEditor* WaveGeneratorProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<WaveGeneratorProcessor, WaveGeneratorProcessorEditor>(this);
}

bool WaveGeneratorProcessor::hasEditor() const
{
  return true;
}


bool WaveGeneratorProcessor::supportsDoublePrecisionProcessing() const
{
  return false;
}

const String WaveGeneratorProcessor::getName() const
{
  return "aWave";
}

bool WaveGeneratorProcessor::acceptsMidi() const
{
  return true;
}

bool WaveGeneratorProcessor::producesMidi() const
{
  return false;
}

double WaveGeneratorProcessor::getTailLengthSeconds() const
{
  return 0.0;
}

int WaveGeneratorProcessor::getNumPrograms()
{
  return 1;
}

int WaveGeneratorProcessor::getCurrentProgram()
{
  return 1;
}

void WaveGeneratorProcessor::setCurrentProgram(int index)
{
  ignoreUnused(index);
}

const String WaveGeneratorProcessor::getProgramName(int index)
{
  ignoreUnused(index);
  return String();
}

void WaveGeneratorProcessor::changeProgramName(int index, const String& newName)
{
  ignoreUnused(index, newName);
}

void WaveGeneratorProcessor::getStateInformation(MemoryBlock& destData)
{
  ignoreUnused(destData);
}

void WaveGeneratorProcessor::setStateInformation(const void* data, int sizeInBytes)
{
  ignoreUnused(data, sizeInBytes);
}

void WaveGeneratorProcessor::setWaveform(waveform newWaveform)
{
  currentWaveform = newWaveform;
}
