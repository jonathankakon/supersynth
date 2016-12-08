/*
  ==============================================================================

    FIRFilterProcessorEditor.cpp
    Created: 8 Dec 2016 1:55:24pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FIRFilterProcessorEditor.h"

FIRFilterProcessorEditor::FIRFilterProcessorEditor(FIRFilterProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor(p) , processor (*p), parent(*b)
{
  
  setSize(150, 60);
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();

  
  //COMBOBOX
  tapsSelectorComboBox = new ComboBox("tapsSelectorBox");
  tapsSelectorComboBox->addItemList({"Strong Lowpass", "Bandpass"}, 1);
  tapsSelectorComboBox->addListener(this);
  //auto value = dynamic_cast<const AudioParameterChoice*>(params[0]);
  //tapsSelectorComboBox->setSelectedId(value->getIndex() +1);
  parent.registerImmobileObject(*tapsSelectorComboBox);
  addAndMakeVisible(tapsSelectorComboBox);
  
  //BUTTON
  bypassButton = new ToggleButton("BypassButton");
  bypassButton->addListener(this);
  parent.registerImmobileObject(*bypassButton);
  addAndMakeVisible(bypassButton);
  //bypassButton->setToggleState(true, juce::dontSendNotification);
  
  
}

FIRFilterProcessorEditor::~FIRFilterProcessorEditor()
{
  
}

void FIRFilterProcessorEditor::paint(juce::Graphics & g)
{
  g.fillAll(Colours::mediumorchid);
  g.setColour(Colours::black);
  g.setFont(15.0f);
  
  Rectangle<int> r(getLocalBounds());
  r.reduce(20, 20);
  
  bypassButton->setBounds(r.withWidth(30));
  tapsSelectorComboBox->setBounds(r.withX(r.getX()+ 50).withWidth(r.getWidth() - 70));
  
  
  
}

void FIRFilterProcessorEditor::resized()
{
}

void FIRFilterProcessorEditor::buttonClicked(juce::Button *button)
{
  if(bypassButton->getState())
    processor.bypass(true);
  else
    processor.bypass(false);
}

void FIRFilterProcessorEditor::buttonStateChanged(juce::Button *button)
{
  
}

void FIRFilterProcessorEditor::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
  switch (comboBoxThatHasChanged->getSelectedId()) {
    case 1:
      processor.changeFilterTaps(lowpass_200hz_strong);
      break;
      
    case 2:
      processor.changeFilterTaps(bandpass300to600);
      break;
      
    default:
      break;
  }
}


