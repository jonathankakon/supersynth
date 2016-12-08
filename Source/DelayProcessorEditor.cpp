/*
  ==============================================================================

    DelayProcessorEditor.cpp
    Created: 5 Dec 2016 4:46:24pm
    Author:  jonathan kakon

  ==============================================================================
*/

#include "DelayProcessorEditor.h"

DelayProcessorEditor::DelayProcessorEditor(DelayProcessor* p, ProcessorEditorBase* b)
  : AudioProcessorEditor(p), processor(*p), parent(*b)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(110, 60);

  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  const AudioParameterFloat* delayParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  delaySlider = new Slider(delayParam->name);
  delaySlider->setRange(0, 1, 0.001);
  delaySlider->setSliderStyle(Slider::RotaryVerticalDrag);
  delaySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  delaySlider->setValue(dynamic_cast<const AudioProcessorParameter*>(delayParam)->getValue());
  delaySlider->addListener(this);
  delaySlider->setPopupDisplayEnabled(true, &parent);
  parent.registerImmobileObject(*delaySlider);
  addAndMakeVisible(delaySlider);

  const AudioParameterFloat* decayParam = dynamic_cast<AudioParameterFloat*>(params[1]);
  decaySlider = new Slider(decayParam->name);
  decaySlider->setRange(0, 1, 0.001);
  decaySlider->setSliderStyle(Slider::RotaryVerticalDrag);
  decaySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  decaySlider->setValue(dynamic_cast<const AudioProcessorParameter*>(decayParam)->getValue());
  decaySlider->addListener(this);
  decaySlider->setPopupDisplayEnabled(true, &parent);
  parent.registerImmobileObject(*decaySlider);
  addAndMakeVisible(decaySlider);

  addAndMakeVisible(delayLabel);
  addAndMakeVisible(decayLabel);

  delayLabel.setText("Time", juce::dontSendNotification);
  decayLabel.setText("Decay", juce::dontSendNotification);

  delayLabel.attachToComponent(delaySlider, false);
  decayLabel.attachToComponent(decaySlider, false);
}

DelayProcessorEditor::~DelayProcessorEditor()
{

}

void DelayProcessorEditor::paint(juce::Graphics &g)
{
  g.fillAll(Colours::cyan);

  Rectangle<int> r(5, 22, 50, 30);
  delaySlider->setBounds(r);

  r.setX(r.getX() + 40);
  decaySlider->setBounds(r);
}

void DelayProcessorEditor::resized()
{

}

// copied from juce demo project GenericEditor.h
void DelayProcessorEditor::sliderValueChanged(Slider* slider)
{
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();

  if (slider == delaySlider)
  {
    AudioProcessorParameter* param = params[0];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost((float)slider->getValue());
    else
      param->setValue((float)slider->getValue());
  }
  else if (slider == decaySlider)
  {
    AudioProcessorParameter* param = params[1];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost((float)decaySlider->getValue());
    else
      param->setValue((float)slider->getValue());
  }
}