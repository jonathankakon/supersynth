/*
  ==============================================================================

    DistortionProcessorEditor.cpp
    Created: 2 Dec 2016 1:09:30pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "DistortionProcessorEditor.h"
#include "DistortionProcessor.h"

//==============================================================================
DistortionProcessorEditor::DistortionProcessorEditor (DistortionProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor (p), processor (*p), parent(*b)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (100, 120);
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  const AudioParameterFloat* preGainParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  preGainSlider = new Slider(preGainParam->name);
  preGainSlider->setRange(0, 1, 0.001);
  preGainSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  preGainSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  preGainSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(preGainParam)->getValue());
  preGainSlider->addListener(this);
  parent.registerImmobileObject(*preGainSlider);
  addAndMakeVisible(preGainSlider);
  
  const AudioParameterChoice* distortionTypeParam = dynamic_cast<AudioParameterChoice*>(params[1]);
  typeBox = new ComboBox(distortionTypeParam->name);
  typeBox->addItemList({"hardclip", "tanh"}, 1);
  typeBox->addListener(this);
  auto value = dynamic_cast<const AudioParameterChoice*>(params[1]);
  typeBox->setSelectedId(value->getIndex() + 1);
  parent.registerImmobileObject(*typeBox);
  addAndMakeVisible(typeBox);
  
  const AudioParameterFloat* postGainParam = dynamic_cast<AudioParameterFloat*>(params[2]);
  postGainSlider = new Slider(postGainParam->name);
  postGainSlider->setRange(0, 1, 0.001f);
  postGainSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  postGainSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  postGainSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(postGainParam)->getValue());
  postGainSlider->addListener(this);
  parent.registerImmobileObject(*postGainSlider);
  addAndMakeVisible(postGainSlider);
  
  //Labels
  preGainLabel.setText("Drive", juce::dontSendNotification);
  preGainLabel.attachToComponent(preGainSlider, false);
  preGainLabel.setJustificationType(juce::Justification::horizontallyCentred);
  addAndMakeVisible(preGainLabel);
  
  typeLabel.setText("Type", juce::dontSendNotification);
  typeLabel.attachToComponent(typeBox, false);
  typeLabel.setJustificationType(juce::Justification::horizontallyCentred);
  addAndMakeVisible(typeLabel);
  
  postGainLabel.setText("Vol", juce::dontSendNotification);
  postGainLabel.attachToComponent(postGainSlider, false);
  postGainLabel.setJustificationType(juce::Justification::horizontallyCentred);
  addAndMakeVisible(postGainLabel);

}

DistortionProcessorEditor::~DistortionProcessorEditor()
{
  
}

//==============================================================================
void DistortionProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::darkgrey);
  
  Rectangle<int> r(10, 25 , 30, 30);
  preGainSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  postGainSlider->setBounds(r);
  
  r.setX(10);
  r.setY(80);
  r.setWidth(70);
  typeBox->setBounds(r);
  
}

void DistortionProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}

// copied from juce demo project GenericEditor.h
void DistortionProcessorEditor::sliderValueChanged(Slider* slider)
{
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  if (slider == preGainSlider)
  {
    AudioProcessorParameter* param = params[0];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == postGainSlider)
  {
    AudioProcessorParameter* param = params[2];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }

}

void DistortionProcessorEditor::comboBoxChanged(juce::ComboBox *comboBox)
{
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  if(comboBox == typeBox)
  {
    AudioProcessorParameter* param = params[1];
    
    param->setValueNotifyingHost (comboBox->getSelectedItemIndex());
  }
}
