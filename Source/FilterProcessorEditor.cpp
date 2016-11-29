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
  frequencySlider->setValue(50);
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
  qSlider->setValue(0.72);
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
  gainSlider->setValue(0);
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
  dropdownMenuFilterTypes->setSelectedId(1);
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
  
  //BUTTONS
//  buttonOnOff = new ToggleButton("Bypass");
//  button0 = new ToggleButton("1st Lowpass" );
//  button1 = new ToggleButton("1st Highpass");
//  button2 = new ToggleButton("Bandpass");
//  button3 = new ToggleButton("Bandstop");
//  button4 = new ToggleButton("2nd Lowpass");
//  button5 = new ToggleButton("2nd Highpass");
//  button6 = new ToggleButton("Can. Bandpass");
//  button7 = new ToggleButton("Can. Bandstop");
//  button8 = new ToggleButton("1st Lowshelf");
//  button9 = new ToggleButton("1st Highshelf");
  
//  buttonOnOff->setRadioGroupId(1);
//  button0->setRadioGroupId(1);
//  button1->setRadioGroupId(1);
//  button2->setRadioGroupId(1);
//  button3->setRadioGroupId(1);
//  button4->setRadioGroupId(1);
//  button5->setRadioGroupId(1);
//  button6->setRadioGroupId(1);
//  button7->setRadioGroupId(1);
//  button8->setRadioGroupId(1);
//  button9->setRadioGroupId(1);
  
  //setting default button
  //buttonOnOff->setToggleState(true, juce::dontSendNotification); //buttonOnOff is on by default

  /*
  buttonOnOff->addListener(this);
  addAndMakeVisible(buttonOnOff);
  
  button0->addListener(this);
  addAndMakeVisible(button0);
  
  button1->addListener(this);
  addAndMakeVisible(button1);
  
  button2->addListener(this);
  //addAndMakeVisible(button2);
  
  button3->addListener(this);
  //addAndMakeVisible(button3);
  
  button4->addListener(this);
  addAndMakeVisible(button4);
  
  button5->addListener(this);
  addAndMakeVisible(button5);
  
  button6->addListener(this);
  addAndMakeVisible(button6);
  
  button7->addListener(this);
  addAndMakeVisible(button7);

  button8->addListener(this);
  addAndMakeVisible(button8);
  
  button9->addListener(this);
  addAndMakeVisible(button9);
  */
  
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
  
//  buttonOnOff->setBounds(r.withWidth(1.8*offset).withHeight(yoffset));
//  button0->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(1*yoffset));
//  button1->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(2*yoffset));
//  button4->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(3*yoffset));
//  button5->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(4*yoffset));
//  button6->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(5*yoffset));
//  button7->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(6*yoffset));
//  button8->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(7*yoffset));
//  button9->setBounds(r.withWidth(1.8*offset).withHeight(yoffset).withY(8*yoffset));
  
  
  
  
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
