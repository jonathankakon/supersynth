/*
  ==============================================================================

    NoiseGeneratorProcessorEditor.cpp
    Created: 27 Nov 2016 8:04:13pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "NoiseGeneratorProcessorEditor.h"
#include "NoiseGeneratorProcessor.h"

NoiseGeneratorProcessorEditor::NoiseGeneratorProcessorEditor (NoiseGeneratorProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor (p), processor (*p), parent(*b)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (100, 50);

  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  const AudioParameterFloat* volumeParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  volumeSlider = new Slider(volumeParam->name);
  volumeSlider->setRange(0, 1);
  volumeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  volumeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  volumeSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(volumeParam)->getValue());
  volumeSlider->addListener(this);
  parent.registerImmobileObject(*volumeSlider);
  addAndMakeVisible(volumeSlider);
  
  const AudioParameterChoice* noisetypeParam = dynamic_cast<AudioParameterChoice*>(params[1]);
  noisetypeSlider = new Slider(noisetypeParam->name);
  noisetypeSlider->setRange(0, 1, 1);
  noisetypeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  noisetypeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  noisetypeSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(noisetypeParam)->getValue());
  noisetypeSlider->addListener(this);
  parent.registerImmobileObject(*noisetypeSlider);
  addAndMakeVisible(noisetypeSlider);
  
}

NoiseGeneratorProcessorEditor::~NoiseGeneratorProcessorEditor()
{
  
}

void NoiseGeneratorProcessorEditor::paint(juce::Graphics &g)
{
  g.fillAll (Colours::lightgreen);
  
  Rectangle<int> r(10, 5 , 30, 30);
  volumeSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  noisetypeSlider->setBounds(r);
}

void NoiseGeneratorProcessorEditor::resized()
{
  
}

// copied from juce demo project GenericEditor.h
void NoiseGeneratorProcessorEditor::sliderValueChanged(Slider* slider)
{
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  if (slider == volumeSlider)
  {
    DBG("volume 222: " << volumeSlider->getValue());
    AudioProcessorParameter* param = params[0];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == noisetypeSlider)
  {
    AudioProcessorParameter* param = params[1];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) noisetypeSlider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }

}
