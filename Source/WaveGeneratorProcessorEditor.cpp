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
: ProcessorEditorBase (&p, false, true, false), processor (p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (200, 100);
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  const AudioParameterFloat* frequencyParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  frequencySlider = new Slider(frequencyParam->name);
  frequencySlider->setRange(100, 5000, 0.0001);
  frequencySlider->setSkewFactor(0.5);
  frequencySlider->setSliderStyle(Slider::RotaryVerticalDrag);
  frequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  frequencySlider->setValue(dynamic_cast<const AudioProcessorParameter*>(frequencyParam)->getValue());
  frequencySlider->addListener(this);
  registerImmobileObject(*frequencySlider);
  addAndMakeVisible(frequencySlider);
  
  const AudioParameterFloat* volumeParam = dynamic_cast<AudioParameterFloat*>(params[1]);
  volumeSlider = new Slider(volumeParam->name);
  volumeSlider->setRange(volumeParam->range.start, volumeParam->range.end);
  volumeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  volumeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  volumeSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(volumeParam)->getValue());
  volumeSlider->addListener(this);
  registerImmobileObject(*volumeSlider);
  addAndMakeVisible(volumeSlider);
  
  
  setConnectors();
   
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
  
  Rectangle<int> r(getLocalBounds());
  volumeSlider->setBounds(10, 40, 40, 40);
  r.reduce(40, 5);
  frequencySlider->setBounds(r);
  
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
    {
      param->setValueNotifyingHost((float)slider->getValue());
    }
    else
    {
      param->setValue((float)slider->getValue());
    }
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


