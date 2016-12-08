/*
  ==============================================================================

    DelayProcessor.cpp
    Created: 5 Dec 2016 4:46:42pm
    Author:  jonathan kakon

  ==============================================================================
*/

#include "DelayProcessor.h"
#include "ProcessorEditorWithConnectors.h"
#include "DelayProcessorEditor.h"

DelayProcessor::DelayProcessor() : AudioProcessor(BusesProperties()
  .withOutput("Audio Output", AudioChannelSet::mono())
  .withInput("Audio Input", AudioChannelSet::mono())),
  delayPosition(0)
{
  // dont change the order of the parameters here, because the Editor depends on it!
  addParameter(delayParam = new AudioParameterFloat("delay", "Delay", 0, 1, 0.2));
  addParameter(decayParam = new AudioParameterFloat("decay", "Decay", 0, 1, 0.3));

  AudioProcessor::addListener(this);
}

DelayProcessor::~DelayProcessor()
{

}

void DelayProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  rate = sampleRate;
  bufferSize = samplesPerBlock;

  delayBufferSize = *delayParam * 2 * sampleRate;
  delayBuffer.setSize(1, 2*sampleRate);
  delayBuffer.clear();
}

void DelayProcessor::releaseResources()
{

}

void DelayProcessor::audioProcessorParameterChanged(AudioProcessor* /*processor*/, int parameterIndex, float /*newValue*/)
{
  if (parameterIndex == 0) 
  {
    delayBufferSize = *delayParam * 2 * rate;
  }
}

void DelayProcessor::processBlock(AudioSampleBuffer &buffer, juce::MidiBuffer &/*midiBuffer*/)
{
  applyDelay(buffer);
  setCurrentRMS(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
}

AudioProcessorEditor* DelayProcessor::createEditor()
{
  return new ProcessorEditorWithConnectors<DelayProcessor, DelayProcessorEditor>(this);
}


//Apply delay fnction from JuceDemoPlugin... easy enough
void DelayProcessor::applyDelay(AudioBuffer<float>& buffer)
{
  const int numSamples = buffer.getNumSamples();
  const float delayLevel = *decayParam;

  int delayPos = 0;

  float* const channelData = buffer.getWritePointer(0);
  float* const delayData = delayBuffer.getWritePointer(0);
  delayPos = delayPosition;

  for (int i = 0; i < numSamples; ++i)
  {
    const float in = channelData[i];
    channelData[i] += delayData[delayPos];
    delayData[delayPos] = (delayData[delayPos] + in) * delayLevel;

    if (++delayPos >= delayBufferSize)
    {
      delayPos = 0;
    }
  }

  delayPosition = delayPos;
}
