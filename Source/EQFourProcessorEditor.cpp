/*
  ==============================================================================

    EQFourProcessorEditor.cpp
    Created: 2 Nov 2016 4:21:56pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "EQFourProcessorEditor.h"



EQFourProcessorEditor::EQFourProcessorEditor (EQFourProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor(p) , processor (*p), parent(*b)
{
  setSize(500, 300);
  
  //SLIDERS initialising and setting skew factors for frequency sliders
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  AudioParameterFloat* param = dynamic_cast< AudioParameterFloat*>(params[0]);
  cutoffFreqBand1 = new Slider(param->name);
  initialiseSlider(cutoffFreqBand1, param, 50, 10000, 0.001, 50);
  cutoffFreqBand1->setTextValueSuffix(" Hz");
  
  param = dynamic_cast< AudioParameterFloat*>(params[1]);
  cutoffFreqBand2 = new Slider(param->name);
  initialiseSlider(cutoffFreqBand2, param, 50, 10000, 0.001, 1000);
  cutoffFreqBand2->setTextValueSuffix(" Hz");
  
  param = dynamic_cast< AudioParameterFloat*>(params[2]);
  cutoffFreqBand3 = new Slider(param->name);
  initialiseSlider(cutoffFreqBand3, param, 50, 15000, 0.001, 5000);
  cutoffFreqBand3->setTextValueSuffix(" Hz");
  
  param = dynamic_cast< AudioParameterFloat*>(params[3]);
  cutoffFreqBand4 = new Slider(param->name);
  initialiseSlider(cutoffFreqBand4, param, 50, 15000, 0.001, 15000);
  cutoffFreqBand4->setTextValueSuffix(" Hz");
  
  param = dynamic_cast< AudioParameterFloat*>(params[4]);
  qParamBand1 = new Slider(param->name);
  initialiseSlider(qParamBand1, param, 0.1, 6, 0.0001, 0.72);
  param = dynamic_cast< AudioParameterFloat*>(params[5]);
  qParamBand2 = new Slider(param->name);
  initialiseSlider(qParamBand2, param, 0.1, 6, 0.0001, 0.72);
  param = dynamic_cast< AudioParameterFloat*>(params[6]);
  qParamBand3 = new Slider(param->name);
  initialiseSlider(qParamBand3, param, 0.1, 6, 0.0001, 0.72);
  param = dynamic_cast< AudioParameterFloat*>(params[7]);
  qParamBand4 = new Slider(param->name);
  initialiseSlider(qParamBand4, param, 0.1, 6, 0.0001, 0.72);
  
  param = dynamic_cast< AudioParameterFloat*>(params[8]);
  gainBand1 = new Slider(param->name);
  initialiseSlider(gainBand1, param, -12, 12, 0.0001, 0);
  param = dynamic_cast< AudioParameterFloat*>(params[9]);
  gainBand2 = new Slider(param->name);
  initialiseSlider(gainBand2, param, -12, 12, 0.0001, 0);
  param = dynamic_cast< AudioParameterFloat*>(params[10]);
  gainBand3 = new Slider(param->name);
  initialiseSlider(gainBand3, param, -12, 12, 0.0001, 0);
  param = dynamic_cast< AudioParameterFloat*>(params[11]);
  gainBand4 = new Slider(param->name);
  initialiseSlider(gainBand4, param, -12, 12, 0.0001, 0);
  
  cutoffFreqBand1->setSkewFactor(0.5);
  cutoffFreqBand2->setSkewFactor(0.5);
  cutoffFreqBand3->setSkewFactor(0.5);
  cutoffFreqBand4->setSkewFactor(0.5);
  
  qParamBand1->setSkewFactor(0.5);
  qParamBand2->setSkewFactor(0.5);
  qParamBand3->setSkewFactor(0.5);
  qParamBand4->setSkewFactor(0.5);
  
  
  
  //BUTTONS initialising, setting radio groups and adding listeners
  buttonLowCut1 = new ToggleButton("1st Lowcut");
  buttonLowCut2 = new ToggleButton("2nd Lowcut");
  buttonLowShelf = new ToggleButton("Lowshelf");
  
  buttonBandpass2 = new ToggleButton("Peak");
  buttonNotch2 = new ToggleButton("Notch");

  
  buttonBandpass3 = new ToggleButton("Peak");
  buttonNotch3 = new ToggleButton("Notch");
  
  buttonHighCut1 = new ToggleButton("1st Highcut");
  buttonHighCut2 = new ToggleButton("2nd Highcut");
  buttonHighShelf = new ToggleButton("Highshelf");
  
  buttonLowCut1->setRadioGroupId(1);
  buttonLowCut2->setRadioGroupId(1);
  buttonLowShelf->setRadioGroupId(1);
  
  buttonBandpass2->setRadioGroupId(2);
  buttonNotch2->setRadioGroupId(2);
  
  buttonBandpass3->setRadioGroupId(3);
  buttonNotch3->setRadioGroupId(3);
  
  buttonHighCut1->setRadioGroupId(4);
  buttonHighCut2->setRadioGroupId(4);
  buttonHighShelf->setRadioGroupId(4);
  
  buttonLowCut1->addListener(this);
  buttonLowCut2->addListener(this);
  buttonLowShelf->addListener(this);
  buttonNotch2->addListener(this);
  buttonNotch3->addListener(this);
  buttonHighCut1->addListener(this);
  buttonHighCut2->addListener(this);
  buttonHighShelf->addListener(this);
  buttonBandpass2->addListener(this);
  buttonBandpass3->addListener(this);
  
  
  //buttons setting defaults
  buttonLowShelf->setToggleState(true, juce::dontSendNotification);
  buttonBandpass2->setToggleState(true, juce::dontSendNotification);
  buttonBandpass3->setToggleState(true, juce::dontSendNotification);
  buttonHighShelf->setToggleState(true, juce::dontSendNotification);
  
  //Buttons addAndMakeVisible
  addAndMakeVisible(buttonBandpass2);
  addAndMakeVisible(buttonBandpass3);
  addAndMakeVisible(buttonHighCut1);
  addAndMakeVisible(buttonHighCut2);
  addAndMakeVisible(buttonHighShelf);
  addAndMakeVisible(buttonLowCut1);
  addAndMakeVisible(buttonLowCut2);
  addAndMakeVisible(buttonLowShelf);
  addAndMakeVisible(buttonNotch2);
  addAndMakeVisible(buttonNotch3);
  
  
  //LABELS
  addAndMakeVisible(frequencyLabel);
  addAndMakeVisible(qLabel);
  addAndMakeVisible(gainLabel);
  
  frequencyLabel.setText("Frequency", juce::dontSendNotification);
  qLabel.setText("Q", juce::dontSendNotification);
  gainLabel.setText("Gain", juce::dontSendNotification);
  
  frequencyLabel.attachToComponent(cutoffFreqBand1, true);
  qLabel.attachToComponent(qParamBand1, true);
  gainLabel.attachToComponent(gainBand1, true);
  
}


EQFourProcessorEditor::~EQFourProcessorEditor()
{
}

//==============================================================================
void EQFourProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::lightgoldenrodyellow);
  g.setColour (Colours::black);
  g.setFont (15.0f);
  g.drawFittedText ("Equalizer!", Rectangle<int>(200,15), Justification::centred, 1);

  Rectangle<int> topBar(getLocalBounds().withHeight(30));
//  g.drawRect(topBar);
  
  
  Rectangle<int> r(getLocalBounds());
  r.reduce(30, 30);
  r.setX(60);
  int xoffset = r.getWidth()/4;
  int yoffset = r.getHeight()/4;
  int yoffset12 = r.getHeight()/12;
  
  //SLIDERS
//  Rectangle<int> r1(getLocalBounds());
//  r1 = r.withWidth(xoffset).withHeight(yoffset);
  cutoffFreqBand1->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()).withY(r.getY()));
  cutoffFreqBand2->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+1*xoffset).withY(r.getY()));
  cutoffFreqBand3->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+2*xoffset).withY(r.getY()));
  cutoffFreqBand4->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+3*xoffset).withY(r.getY()));
  
  qParamBand1->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+0*xoffset).withY(r.getY()+yoffset));
  qParamBand2->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+1*xoffset).withY(r.getY()+yoffset));
  qParamBand3->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+2*xoffset).withY(r.getY()+yoffset));
  qParamBand4->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+3*xoffset).withY(r.getY()+yoffset));
  
  gainBand1->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+0*xoffset).withY(r.getY()+2*yoffset));
  gainBand2->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+1*xoffset).withY(r.getY()+2*yoffset));
  gainBand3->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+2*xoffset).withY(r.getY()+2*yoffset));
  gainBand4->setBounds(r.withWidth(xoffset).withHeight(yoffset).withX(r.getX()+3*xoffset).withY(r.getY()+2*yoffset));
  
  
  //BUTTONS
  buttonLowCut1->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withY(r.getY()+3*yoffset+0));
  buttonLowCut2->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withY(r.getY()+3*yoffset+1*yoffset12));
  buttonLowShelf->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withY(r.getY()+3*yoffset+2*yoffset12));
  
  buttonBandpass2->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withX(r.getX()+xoffset).withY(r.getY()+3*yoffset+0*yoffset12));
  buttonNotch2->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withX(r.getX()+xoffset).withY(r.getY()+3*yoffset+1*yoffset12));
  
  buttonBandpass3->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withX(r.getX()+2*xoffset).withY(r.getY()+3*yoffset+0*yoffset12));
  buttonNotch3->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withX(r.getX()+2*xoffset).withY(r.getY()+3*yoffset+1*yoffset12));
  
  buttonHighCut1->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withX(r.getX()+3*xoffset).withY(r.getY()+3*yoffset+0*yoffset12));
  buttonHighCut2->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withX(r.getX()+3*xoffset).withY(r.getY()+3*yoffset+1*yoffset12));
  buttonHighShelf->setBounds(r.withWidth(0.8*xoffset).withHeight(yoffset12).withX(r.getX()+3*xoffset).withY(r.getY()+3*yoffset+2*yoffset12));
  
  
}

void EQFourProcessorEditor::initialiseSlider(juce::Slider *slider, juce::AudioParameterFloat *param, double min, double max, double stepSize, double initialValue)
{
  slider->setRange(min, max, stepSize);
  slider->setSliderStyle(Slider::RotaryVerticalDrag);
  slider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 20);
  slider->setValue(dynamic_cast<const AudioProcessorParameter*>(param)->getValue());
  slider->addListener(this);
  slider->setValue(initialValue);
  slider->setTextBoxIsEditable(true);
  slider->setDoubleClickReturnValue(true, initialValue);
  parent.registerImmobileObject(*slider);
  addAndMakeVisible(slider);
}

void EQFourProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}

void EQFourProcessorEditor::sliderValueChanged(Slider* slider)
{
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();

  if (slider == cutoffFreqBand1) {
    AudioProcessorParameter* param = params[0];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else{
      param->setValue ((float) slider->getValue());
    }
  }
  else if (slider == cutoffFreqBand2) {
    AudioProcessorParameter* param = params[1];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == cutoffFreqBand3) {
    AudioProcessorParameter* param = params[2];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == cutoffFreqBand4) {
    AudioProcessorParameter* param = params[3];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == qParamBand1) {
    AudioProcessorParameter* param = params[4];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == qParamBand2) {
    AudioProcessorParameter* param = params[5];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == qParamBand3) {
    AudioProcessorParameter* param = params[6];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == qParamBand4) {
    AudioProcessorParameter* param = params[7];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == gainBand1) {
    AudioProcessorParameter* param = params[8];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == gainBand2) {
    AudioProcessorParameter* param = params[9];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == gainBand3) {
    AudioProcessorParameter* param = params[10];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == gainBand4) {
    AudioProcessorParameter* param = params[11];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  
}

void EQFourProcessorEditor::buttonClicked(Button* button)
{
  if(button == buttonLowCut1){
    if(buttonLowCut1->getState())
      processor.changeFilterTypeBand1(0);
  }
  else if (button == buttonLowCut2) {
    if (buttonLowCut2->getState()) {
      processor.changeFilterTypeBand1(1);
    }
  }
  else if (button == buttonLowShelf) {
    if (buttonLowShelf->getState()) {
      processor.changeFilterTypeBand1(2);
    }
  }
  else if (button == buttonBandpass2) {
    if (buttonBandpass2->getState()) {
      processor.changeFilterTypeBand2(0);
    }
  }
  else if (button == buttonNotch2) {
    if (buttonNotch2->getState()) {
      processor.changeFilterTypeBand2(1);
    }
  }
  else if (button == buttonBandpass3) {
    if (buttonBandpass3->getState()) {
      processor.changeFilterTypeBand3(0);
    }
  }
  else if (button == buttonNotch3) {
    if (buttonNotch3->getState()) {
      processor.changeFilterTypeBand3(1);
    }
  }
  else if (button == buttonHighCut1) {
    if (buttonHighCut1->getState()) {
      processor.changeFilterTypeBand4(0);
    }
  }
  else if (button == buttonHighCut2) {
    if (buttonHighCut2->getState()) {
      processor.changeFilterTypeBand4(1);
    }
  }
  else if (button == buttonHighShelf) {
    if (buttonHighShelf->getState()) {
      processor.changeFilterTypeBand4(2);
    }
  }
  
}

void EQFourProcessorEditor::buttonStateChanged(Button *button)
{
  
}