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
  .withOutput("Audio Output", AudioChannelSet::mono())
  .withInput("Phase", AudioChannelSet::mono())
  .withInput("Amplitude", AudioChannelSet::mono())
  .withInput("Pitch", AudioChannelSet::mono())),
  oscillator(new VAOscillator())
{
  
  // dont change the order of the parameters here, because the Editor depends on it!
  addParameter(volumeParam = new AudioParameterFloat("volume",
                            "Volume",
                            NormalisableRange<float>(0.0,1,0.001,0.5, false),
                            0.2f));
  
  addParameter(targetFreqParam = new AudioParameterFloat("currentFrequency",
                            "Frequency",
                            NormalisableRange<float>(1, 15000.0, 0.001f,1, false),
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
  
  addParameter(phaseOffsetParam = new AudioParameterFloat("phaseOffset",
                            "PhaseOffset",
                            NormalisableRange<float>(0.0, 2 * double_Pi, 0.001, 1.5, true),
                            double_Pi) );
  
  addParameter(glideParam = new AudioParameterFloat("glide",
                            "Glide",
                            NormalisableRange<float>(2, 0.05, 0.001, 1, false),
                            2) );
  

  AudioProcessor::addListener(this);
  
  currentFrequency = 439.9;
  targetFrequency = 440.0;
  
  currentWaveform = sawUp;
  targetWaveform = sawUp;
  
  waveformChanged = true;//just so that there is a ramp when putting the wavegenerator in
  
  takesMidi = true;
  
}

WaveGeneratorProcessor::~WaveGeneratorProcessor()
{
  
}



void WaveGeneratorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
  // we must set the sample rate of the oscillator object here
  
  blockSize = samplesPerBlock;

  currentSampleRate = sampleRate;
  
  oscillator->setSampleRate(sampleRate);
}

void WaveGeneratorProcessor::releaseResources()
{
}


//you cannot change the parameter values in here => loop
void WaveGeneratorProcessor::audioProcessorParameterChanged(AudioProcessor * processor, int parameterIndex, float /*newValue*/)
{
  ignoreUnused(processor);
  if(parameterIndex == 1 || parameterIndex == 2 || parameterIndex == 3 || parameterIndex == 4)
  {
    targetFrequency = targetFreqParam->get() * octaves[octaveParam->get()] * semitones[semitonesParam->get()] * cents[centsParam->get()];
  }
  else if(parameterIndex == 5)
  {
    setWaveform(waveformParam->getIndex());
  }
  else if(parameterIndex == 6)
  {
    oscillator->setPhaseOffset(phaseOffsetParam->get());
  }
}

void WaveGeneratorProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiBuffer) //fills channels 1 and 0
{
  AudioBuffer<float> outBuffer = getBusBuffer(buffer, false, 0);
  AudioBuffer<float> phaseModBuffer = getBusBuffer(buffer, true, 0);
  AudioBuffer<float> volumeModBuffer = getBusBuffer(buffer, true, 1);
  AudioBuffer<float> pitchModBuffer = getBusBuffer(buffer, true, 2);
  
  // kind of hackish the envelope should be between 0-1 and then everything is nice and doesnt work if sustain = 0.5 :'(
  if(pitchModBuffer.getMagnitude(0, 0, pitchModBuffer.getNumSamples()) == 0)
  {
    FloatVectorOperations::fill(pitchModBuffer.getWritePointer(0), -1, pitchModBuffer.getNumSamples());
  }

  
  Array<int> midiOns; //i know no arrays for primitives :)
  if(takesMidi && !midiBuffer.isEmpty())
  {
    MidiMessage& message1 = *new MidiMessage();
    ScopedPointer<MidiBuffer::Iterator> iterator = new MidiBuffer::Iterator(midiBuffer);
    int i = 0;
    while(iterator->getNextEvent(message1, i))
    {
      if(message1.isNoteOn()){
        if(int pos = message1.getNoteNumber())
        {
          targetFreqParam->setValueNotifyingHost(midiToFreq[pos]/15000.0f);
          midiOns.add(i);
        }
      }
    }
  }

  updateFrequency();
  
  if(currentWaveform == sine)
  {
    oscillator->fillBufferSine(outBuffer, phaseModBuffer, volumeModBuffer, pitchModBuffer, midiOns);
  }
  else if(currentWaveform == sawUp)
  {
    oscillator->fillBufferRisingSaw(outBuffer, phaseModBuffer, volumeModBuffer, pitchModBuffer);
  }
  else if(currentWaveform == sawDown)
  {
    oscillator->fillBufferFallingSaw(outBuffer, phaseModBuffer, volumeModBuffer, pitchModBuffer);
  }
  else if(currentWaveform == square)
  {
    oscillator->fillBufferSquarePulse(outBuffer, phaseModBuffer, volumeModBuffer, pitchModBuffer);
  }
  else // index == 4 => Triangle
  {
    oscillator->fillBufferTriangle(outBuffer, phaseModBuffer, volumeModBuffer, pitchModBuffer);
  }
  
  
  //smooth transitions when changing the waveform
  if(waveformChanged)
  {
    float* const data = outBuffer.getWritePointer(0);
    for(int sampleIndex = 1; sampleIndex <= 100; sampleIndex++)
    {
      data[sampleIndex] *= (sampleIndex - 1)/100.0f;
    }
    waveformChanged = false;
  }
  if(currentWaveform != targetWaveform)
  {
    float* const data = outBuffer.getWritePointer(0);
    for(int sampleIndex = 100; sampleIndex > 0; sampleIndex--)
    {
      data[outBuffer.getNumSamples() - sampleIndex] *= (sampleIndex - 1)/100.0f;
    }
    currentWaveform = targetWaveform;
    waveformChanged = true;
  }
  
  outBuffer.applyGain(volumeParam->get());
  setCurrentRMS(outBuffer.getRMSLevel(0, 0, outBuffer.getNumSamples()));
  //update parameters
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
  return "WaveGeneratorProcessor";
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
  return 0;
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
  targetWaveform = newWaveform;
}

void WaveGeneratorProcessor::setWaveform(int index)
{
  if(index == 0)
  {
    targetWaveform = sine;
  }
  else if(index == 1)
  {
    targetWaveform = sawUp;
  }
  else if(index == 2)
  {
    targetWaveform = sawDown;
  }
  else if(index == 3)
  {
    targetWaveform = square;
  }
  else // index == 4 => Triangle
  {
    targetWaveform = triangle;
  }
}

void WaveGeneratorProcessor::updateFrequency()
{
  //max step size == 1/500 * currentfrequency
  if(std::abs(currentFrequency - targetFrequency) < glideParam->get() * currentFrequency)
  {
    currentFrequency = targetFrequency;
  }
  else
  {
    currentFrequency += glideParam->get() * currentFrequency * ((currentFrequency - targetFrequency) > 0 ? -1 : 1);
  }
  
  oscillator->setFrequency(currentFrequency);
}
