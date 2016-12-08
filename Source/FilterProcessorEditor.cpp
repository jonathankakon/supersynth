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
: AudioProcessorEditor(p) , processor (*p), parent(*b)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(150, 100);
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  const AudioParameterFloat* frequencyParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  frequencySlider = new Slider(frequencyParam->name);
  frequencySlider->setRange(50, 15000, 0.0001);
  frequencySlider->setSkewFactor(0.5);
  frequencySlider->setSliderStyle(Slider::RotaryVerticalDrag);
  frequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 80, 20);
  frequencySlider->setPopupDisplayEnabled(true, this);
  frequencySlider->setValue(dynamic_cast<const AudioProcessorParameter*>(frequencyParam)->getValue());
  frequencySlider->addListener(this);
  frequencySlider->setTextValueSuffix(" Hz");
  parent.registerImmobileObject(*frequencySlider);
  addAndMakeVisible(frequencySlider);
  
  const AudioParameterFloat* qParam = dynamic_cast<AudioParameterFloat*>(params[1]);
  qSlider = new Slider(qParam->name);
  qSlider->setRange(0.1, 6.0, 0.0001);
  //qSlider->setDoubleClickReturnValue(true, 0.72);
  qSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  qSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 80, 20);
  qSlider->setPopupDisplayEnabled(true, this);
  qSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(qParam)->getValue());
  qSlider->setSkewFactor(0.5);
  qSlider->addListener(this);
  parent.registerImmobileObject(*qSlider);
  addAndMakeVisible(qSlider);
  
  const AudioParameterFloat* gainParam = dynamic_cast<AudioParameterFloat*>(params[2]);
  gainSlider = new Slider(gainParam->name);
  gainSlider->setRange(-12, 12,0.001);
  gainSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  gainSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 80, 20);
  gainSlider->setPopupDisplayEnabled(true, this);
  gainSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(gainParam)->getValue());
  gainSlider->addListener(this);
  parent.registerImmobileObject(*gainSlider);
  addAndMakeVisible(gainSlider);
  
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
  
  //COMBOBOX
  dropdownMenuFilterTypes = new ComboBox("dropdownMenuFilterType");
  dropdownMenuFilterTypes->addItemList({"Bypass","1st Lowpass", "1st Highpass", "Bandpass", "Bandstop", "2nd Lowpass",  "2nd Highpass", "Can. Bandpass", "Can. Bandstop", "1st Lowshelf", "1st Highshelf", "FIR"}, 1);
  dropdownMenuFilterTypes->addListener(this);
  auto value = dynamic_cast<const AudioParameterChoice*>(params[3]);
  dropdownMenuFilterTypes->setSelectedId(value->getIndex() + 1);
  parent.registerImmobileObject(*dropdownMenuFilterTypes);
  addAndMakeVisible(dropdownMenuFilterTypes);
  
  //LABELS
  //LABELS
  addAndMakeVisible(frequencyLabel);
  addAndMakeVisible(qLabel);
  addAndMakeVisible(gainLabel);
  
  frequencyLabel.setText("Freq.", juce::dontSendNotification);
  qLabel.setText("Q", juce::dontSendNotification);
  gainLabel.setText("Gain", juce::dontSendNotification);
  
  frequencyLabel.attachToComponent(frequencySlider, false);
  qLabel.attachToComponent(qSlider, false);
  gainLabel.attachToComponent(gainSlider, false);
  
  frequencyLabel.setJustificationType(juce::Justification::horizontallyCentred);
  qLabel.setJustificationType(juce::Justification::horizontallyCentred);
  gainLabel.setJustificationType(juce::Justification::horizontallyCentred);
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
  //g.drawFittedText ("      Filter!", Rectangle<int>(200,15), Justification::centredLeft, 1);

  
  Rectangle<int> r(getLocalBounds());
  r.reduce(20,20);
  int offset = r.getWidth()/3;
  int yoffset = r.getHeight()/2;
  
  
  //filterTypeSlider->setBounds(r.withWidth(offset).withX(offset));
  frequencySlider->setBounds(r.withWidth(offset).withX(r.getX()+0*offset).withHeight(yoffset));
  qSlider->setBounds(r.withWidth(offset).withX(r.getX()+1*offset).withHeight(yoffset));
  gainSlider->setBounds(r.withWidth(offset).withX(r.getX()+2*offset).withHeight(yoffset));
  
  dropdownMenuFilterTypes->setBounds(r.withWidth(3*offset).withHeight(r.getHeight()/3).withY((int)(r.getY()+1.4*yoffset)));
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
  else if (slider == gainSlider){
    AudioProcessorParameter* param = params[2];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }

  
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
  else if(button == button8){
    if (button8->getState()) {
      processor.changeFilterType(9);
    }
  }
  else if (button == button9){
    if (button9->getState()) {
      processor.changeFilterType(10);
    }
  }

}

void FilterProcessorEditor::buttonStateChanged(Button* /*button*/)
{
}

void FilterProcessorEditor::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
  switch (comboBoxThatHasChanged->getSelectedId()) {
    case 1:
      processor.changeFilterType(0);
      break;
    
    case 2:
      processor.changeFilterType(1);
      break;
      
    case 3:
      processor.changeFilterType(2);
      break;
      
    case 4:
      processor.changeFilterType(3);
      break;
      
    case 5:
      processor.changeFilterType(4);
      break;
      
    case 6:
      processor.changeFilterType(5);
      break;
      
    case 7:
      processor.changeFilterType(6);
      break;
      
    case 8:
      processor.changeFilterType(7);
      break;
      
    case 9:
      processor.changeFilterType(8);
      break;
      
    case 10:
      processor.changeFilterType(9);
      break;
      
    case 11:
      processor.changeFilterType(10);
      break;
      
    case 12:
      processor.changeFilterType(11);
      break;
      
    default:
      break;
  }
}
