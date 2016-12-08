/*
 ==============================================================================
  
  EnvelopeProcessorEditor.cpp
  Created: 18 Nov 2016 2:22:30pm
  Author:  Kian Hunziker
  
  ==============================================================================
  */

#include "EnvelopeProcessorEditor.h"

EnvelopeProcessorEditor::EnvelopeProcessorEditor (EnvelopeProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor(p), processor (*p), parent(*b)
{
    setSize(200, 50);
  
  //initialise Sliders for ADSR control
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();

  AudioParameterFloat* param = dynamic_cast< AudioParameterFloat*>(params[0]);
  attackSlider = new Slider(param->name);
  initialiseSlider(attackSlider, param, 0.0, 1, 0.0001, dynamic_cast<const AudioProcessorParameter*>(param)->getValue());
  attackSlider->setTextValueSuffix(" s");
  
  param = dynamic_cast< AudioParameterFloat*>(params[1]);
  decaySlider = new Slider(param->name);
  initialiseSlider(decaySlider, param, 0, 1, 0.0001, dynamic_cast<const AudioProcessorParameter*>(param)->getValue());
  decaySlider->setTextValueSuffix(" s");
  
  param = dynamic_cast<AudioParameterFloat*>(params[2]);
  sustainSlider = new Slider(param->name);
  initialiseSlider(sustainSlider, param, 0, 1, 0.001, dynamic_cast<const AudioProcessorParameter*>(param)->getValue());
  
  param = dynamic_cast<AudioParameterFloat*>(params[3]);
  releaseSlider = new Slider(param->name);
  initialiseSlider(releaseSlider, param, 0, 1, 0.0001, dynamic_cast<const AudioProcessorParameter*>(param)->getValue());
  
  attackSlider->setSkewFactor(0.5);
  decaySlider->setSkewFactor(0.5);
  releaseSlider->setSkewFactor(0.5);
  

  //LABELS
  addAndMakeVisible(attackLabel);
  addAndMakeVisible(decayLabel);
  addAndMakeVisible(sustainLabel);
  addAndMakeVisible(releaseLabel);
  
  attackLabel.setText("A", juce::dontSendNotification);
  decayLabel.setText("D", juce::dontSendNotification);
  sustainLabel.setText("S", juce::dontSendNotification);
  releaseLabel.setText("R", juce::dontSendNotification);
  
  attackLabel.attachToComponent(attackSlider, false);
  decayLabel.attachToComponent(decaySlider, false);
  sustainLabel.attachToComponent(sustainSlider, false);
  releaseLabel.attachToComponent(releaseSlider, false);
  
  
  }

EnvelopeProcessorEditor::~EnvelopeProcessorEditor()
{
  
  }


void EnvelopeProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::darkslateblue );
    g.setColour (Colours::black);
    g.setFont (15.0f);
    //g.drawFittedText ("Envelope!", Rectangle<int>(200,15), Justification::centred, 1);
  
  Rectangle<int> r(getLocalBounds());
  r.reduce(10, 10);
  
  int xoffset = r.getWidth()/4;
  
  attackSlider->setBounds(r.withWidth(xoffset).withX(r.getX()).withY(r.getY() + 7));
  decaySlider->setBounds(r.withWidth(xoffset).withX(r.getX()+xoffset).withY(r.getY() + 7));
  sustainSlider->setBounds(r.withWidth(xoffset).withX(r.getX()+2*xoffset).withY(r.getY() + 7));
  releaseSlider->setBounds(r.withWidth(xoffset).withX(r.getX()+3*xoffset).withY(r.getY() + 7));
  }

void EnvelopeProcessorEditor::resized()
{
    
  }

void EnvelopeProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();

  if(slider == attackSlider)
  {
    AudioProcessorParameter* param = params[0];
    if(slider->isMouseButtonDown())
      param->setValueNotifyingHost((float)slider->getValue());
    else
      param->setValue((float)slider->getValue());
  }
  else if (slider == decaySlider)
  {
    AudioProcessorParameter* param = params[1];
    if(slider->isMouseButtonDown())
      param->setValueNotifyingHost((float)slider->getValue());
    else
      param->setValue((float)slider->getValue());
  }
  else if (slider == sustainSlider)
  {
    AudioProcessorParameter* param = params[2];
    if(slider->isMouseButtonDown())
      param->setValueNotifyingHost((float)slider->getValue());
    else
      param->setValue((float)slider->getValue());
  }
  else if (slider == releaseSlider)
  {
    AudioProcessorParameter* param = params[3];
    if(slider->isMouseButtonDown())
      param->setValueNotifyingHost((float)slider->getValue());
    else
      param->setValue((float)slider->getValue());
  }
    
  }

void EnvelopeProcessorEditor::initialiseSlider(juce::Slider *slider, juce::AudioParameterFloat *param, double min, double max, double stepSize, double initialValue)
{
  slider->setRange(min, max, stepSize);
  slider->setSliderStyle(Slider::RotaryVerticalDrag);
  slider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox , false, 80, 20);
  slider->setValue(dynamic_cast<const AudioProcessorParameter*>(param)->getValue());
  slider->addListener(this);
  slider->setValue(initialValue);
  slider->setTextBoxIsEditable(false);
  slider->setPopupDisplayEnabled(true, this);
  slider->setDoubleClickReturnValue(true, initialValue);
  parent.registerImmobileObject(*slider);
  addAndMakeVisible(slider);
}