/*
  ==============================================================================

    FilterProcessorEditor.cpp
    Created: 7 Oct 2016 3:18:49pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FilterProcessorEditor.h"


//==============================================================================
FilterProcessorEditor::FilterProcessorEditor (FilterProcessor& p)
: ProcessorEditorBase (&p, true, true, false), processor (p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(200, 100);
  
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
  
  const AudioParameterFloat* qParam = dynamic_cast<AudioParameterFloat*>(params[1]);
  qSlider = new Slider(qParam->name);
  qSlider->setRange(0.1, 3.0, 0.0001);
  qSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  qSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  qSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(qParam)->getValue());
  qSlider->addListener(this);
  registerImmobileObject(*qSlider);
  addAndMakeVisible(qSlider);
  

  
  setConnectors();
  
  
}


FilterProcessorEditor::~FilterProcessorEditor()
{
}
//==============================================================================
void FilterProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::lightblue);
  g.setColour (Colours::black);
  g.setFont (15.0f);
  g.drawFittedText ("Filter!", Rectangle<int>(200,15), Justification::centred, 1);
  
  
  Rectangle<int> r(getLocalBounds());
  r.reduce(10,10);
  int offset = r.getWidth()/4;
  

  frequencySlider->setBounds(r.withWidth(offset).withX(offset));
  qSlider->setBounds(r.withWidth(offset).withX(2.5*offset));

  
  
}


void FilterProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}

void FilterProcessorEditor::sliderValueChanged(Slider* slider)
{
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();

  if (slider == frequencySlider) {
    AudioProcessorParameter* param = params[0];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
    }
  else if (slider == qSlider) {
    AudioProcessorParameter* param = params[1];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }

  
}
