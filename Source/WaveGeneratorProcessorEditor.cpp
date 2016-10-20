/*
  ==============================================================================

    WaveGeneratorProcessorEditor.cpp
    Created: 7 Oct 2016 3:43:18pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "WaveGeneratorProcessorEditor.h"
#include "WaveGeneratorProcessor.h"


//==============================================================================
WaveGeneratorProcessorEditor::WaveGeneratorProcessorEditor (WaveGeneratorProcessor& p)
: ProcessorEditorBase (&p), processor (p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (200, 100);
  
  
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  const AudioParameterFloat* frequencyParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  frequencySlider = new Slider(frequencyParam->name);
  frequencySlider->setRange(frequencyParam->range.start, frequencyParam->range.end);
  frequencySlider->setSliderStyle(Slider::LinearHorizontal);
  frequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  frequencySlider->setValue(dynamic_cast<const AudioProcessorParameter*>(frequencyParam)->getValue());
  frequencySlider->addListener(this);
  addAndMakeVisible(frequencySlider);
  
  const AudioParameterFloat* volumeParam = dynamic_cast<AudioParameterFloat*>(params[1]);
  volumeSlider = new Slider(volumeParam->name);
  volumeSlider->setRange(volumeParam->range.start, volumeParam->range.end);
  volumeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  volumeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  volumeSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(volumeParam)->getValue());
  volumeSlider->addListener(this);
  addAndMakeVisible(volumeSlider);
  
  
  
  
  //label
  
  
  
  
}

WaveGeneratorProcessorEditor::~WaveGeneratorProcessorEditor()
{
}

//==============================================================================
void WaveGeneratorProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::lightgreen);
  g.setColour (Colours::black);
  g.setFont (15.0f);
  g.drawFittedText ("Wave Generator!", Rectangle<int>(200,15), Justification::centred, 1);
  
  
  volumeSlider->setBounds(10, 40, 40, 40);
  frequencySlider->setBounds(60, 40, 120, 20);
  
}

void WaveGeneratorProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}


// copied from juce demo project GenericEditor.h
void WaveGeneratorProcessorEditor::sliderValueChanged (Slider* slider)
{
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  if (slider == frequencySlider)
  {
    AudioProcessorParameter* param = params[0];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  if (slider == volumeSlider)
  {
    AudioProcessorParameter* param = params[1];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
}


