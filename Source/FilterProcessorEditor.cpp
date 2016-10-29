/*
  ==============================================================================

    FilterProcessorEditor.cpp
    Created: 7 Oct 2016 3:18:49pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FilterProcessorEditor.h"


//==============================================================================
FilterProcessorEditor::FilterProcessorEditor (FilterProcessor* p, ProcessorEditorBase* b)
: ProcessorEditorBase(p) , processor (*p), parent(*b)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(250, 300);
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  const AudioParameterFloat* frequencyParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  frequencySlider = new Slider(frequencyParam->name);
  frequencySlider->setRange(100, 10000, 0.0001);
  frequencySlider->setSkewFactor(0.5);
  frequencySlider->setSliderStyle(Slider::RotaryVerticalDrag);
  frequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 20);
  //frequencySlider->setPopupDisplayEnabled(true, this);
  frequencySlider->setValue(dynamic_cast<const AudioProcessorParameter*>(frequencyParam)->getValue());
  frequencySlider->addListener(this);
  parent.registerImmobileObject(*frequencySlider);
  addAndMakeVisible(frequencySlider);
  
  const AudioParameterFloat* qParam = dynamic_cast<AudioParameterFloat*>(params[1]);
  qSlider = new Slider(qParam->name);
  qSlider->setRange(0.1, 6.0, 0.0001);
  //qSlider->setDoubleClickReturnValue(true, 0.72);
  qSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  qSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 20);
  //qSlider->setPopupDisplayEnabled(true, this);
  qSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(qParam)->getValue());
  qSlider->addListener(this);
  parent.registerImmobileObject(*qSlider);
  addAndMakeVisible(qSlider);
  
//  const AudioParameterChoice* typeParam = dynamic_cast<AudioParameterChoice*>(params[2]);
//  filterTypeSlider = new Slider (typeParam->name);
//  filterTypeSlider->setRange(0, 7, 1);
//  filterTypeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
//  filterTypeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
//  filterTypeSlider->setPopupDisplayEnabled(true, this);
//  filterTypeSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(typeParam)->getValue());
//  filterTypeSlider->addListener(this);
//  registerImmobileObject(*filterTypeSlider);
//  addAndMakeVisible(filterTypeSlider);
  
  buttonOnOff = new ToggleButton("Bypass");
  button0 = new ToggleButton("1st Lowpass" );
  button1 = new ToggleButton("1st Highpass");
  button2 = new ToggleButton("Bandpass");
  button3 = new ToggleButton("Bandstop");
  button4 = new ToggleButton("2nd Lowpass");
  button5 = new ToggleButton("2nd Highpass");
  button6 = new ToggleButton("Can. Bandpass");
  button7 = new ToggleButton("Can. Bandstop");
  
  buttonOnOff->setRadioGroupId(1);
  button0->setRadioGroupId(1);
  button1->setRadioGroupId(1);
  button2->setRadioGroupId(1);
  button3->setRadioGroupId(1);
  button4->setRadioGroupId(1);
  button5->setRadioGroupId(1);
  button6->setRadioGroupId(1);
  button7->setRadioGroupId(1);
  
  buttonOnOff->addListener(this);
  addAndMakeVisible(buttonOnOff);
  
  button0->addListener(this);
  addAndMakeVisible(button0);
  button0->setToggleState(true, juce::dontSendNotification);
  
  button1->addListener(this);
  addAndMakeVisible(button1);
  
  button2->addListener(this);
  addAndMakeVisible(button2);
  
  button3->addListener(this);
  addAndMakeVisible(button3);
  
  button4->addListener(this);
  addAndMakeVisible(button4);
  
  button5->addListener(this);
  addAndMakeVisible(button5);
  
  button6->addListener(this);
  addAndMakeVisible(button6);
  
  button7->addListener(this);
  addAndMakeVisible(button7);

  
  
  
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
  int yoffset = r.getHeight()/numButtons;
  
  //filterTypeSlider->setBounds(r.withWidth(offset).withX(offset));
  frequencySlider->setBounds(r.withWidth(offset).withX(2*offset).withHeight(2*offset));
  qSlider->setBounds(r.withWidth(offset).withX(3*offset).withHeight(2*offset));
  
  buttonOnOff->setBounds(r.withWidth(1.8*offset).withHeight(yoffset));
  button0->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(1*yoffset));
  button1->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(2*yoffset));
  button2->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(3*yoffset));
  button3->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(4*yoffset));
  button4->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(5*yoffset));
  button5->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(6*yoffset));
  button6->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(7*yoffset));
  button7->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(8*yoffset));
  
  
  
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
//  else if (slider == filterTypeSlider){
//    AudioProcessorParameter* param = params[2];
//    DBG(dynamic_cast<const AudioProcessorParameter*>(params[2])->name);
//    if (slider->isMouseButtonDown())
//      param->setValueNotifyingHost ((float) slider->getValue());
//    else
//      param->setValue ((float) slider->getValue());
//  }

  
}

void FilterProcessorEditor::buttonClicked(Button* button)
{
  if(button == buttonOnOff){
    if(buttonOnOff->getState())
      processor.changeFilterType(0);
  }
  else if (button == button0) {
    if (button0->getState()) {
      processor.changeFilterType(1);
    }
  }
  else if(button == button1){
    if (button1->getState()) {
      processor.changeFilterType(2);
    }
  }
  else if(button == button2){
    if (button2->getState()) {
      processor.changeFilterType(3);
    }
  }
  else if(button == button3){
    if (button3->getState()) {
      processor.changeFilterType(4);
    }
  }
  else if(button == button4){
    if (button4->getState()) {
      processor.changeFilterType(5);
    }
  }
  else if(button == button5){
    if (button5->getState()) {
      processor.changeFilterType(6);
    }
  }
  else if(button == button6){
    if (button6->getState()) {
      processor.changeFilterType(7);
    }
  }
  else if(button == button7){
    if (button7->getState()) {
      processor.changeFilterType(8);
    }
  }

}

void FilterProcessorEditor::buttonStateChanged(Button *button)
{

}
