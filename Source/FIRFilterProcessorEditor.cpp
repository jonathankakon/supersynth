/*
  ==============================================================================

    FIRFilterProcessorEditor.cpp
    Created: 8 Dec 2016 1:55:24pm
    Author:  Kian Hunziker

  ==============================================================================
*/

#include "FIRFilterProcessorEditor.h"
#include "FIRTaps.h"

FIRFilterProcessorEditor::FIRFilterProcessorEditor(FIRFilterProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor(p) , processor (*p), parent(*b)
{
  
  setSize(250, 60);
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();

  
  //COMBOBOX
  tapsSelectorComboBox = new ComboBox("tapsSelectorBox");
  tapsSelectorComboBox->addItemList({"Strong Lowpass 200hz","Medium Lowpass 500hz", "Soft Lowpass 500hz","Cut 15k", "Cut 18k", "Bandpass 450hz", "Notch 250hz", "Cut below 40hz", "Cut below 50hz", "Highpass 600hz", "Lowpass 2600hz"}, 1);
  tapsSelectorComboBox->addListener(this);
  //auto value = dynamic_cast<const AudioParameterChoice*>(params[0]);
  //tapsSelectorComboBox->setSelectedId(value->getIndex() +1);
  parent.registerImmobileObject(*tapsSelectorComboBox);
  addAndMakeVisible(tapsSelectorComboBox);
  
  //BUTTON
  bypassButton = new ToggleButton("Bypass");
  bypassButton->addListener(this);
  parent.registerImmobileObject(*bypassButton);
  addAndMakeVisible(bypassButton);
  bypassButton->setToggleState(true, juce::dontSendNotification);
  
  
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
  
  bypassButton->setBounds(r.withWidth(80));
  tapsSelectorComboBox->setBounds(r.withX(r.getX()+ 100).withWidth(r.getWidth() - 120));
  
  
  
}

void FIRFilterProcessorEditor::resized()
{
}

void FIRFilterProcessorEditor::buttonClicked(juce::Button *button)
{
  if(bypassButton->getToggleState())
    processor.bypass(true);
  else
    processor.bypass(false);
}

void FIRFilterProcessorEditor::buttonStateChanged(juce::Button *button)
{
  
}

void FIRFilterProcessorEditor::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
  int size = 0;
  switch (comboBoxThatHasChanged->getSelectedId()) {
    case 1:
      size =  sizeof(lowpass_200hz_strong)/sizeof(*lowpass_200hz_strong);
      processor.changeFilterTaps(lowpass_200hz_strong, size);
      break;
      
    case 2:
      size =  sizeof(lowpass_500hz)/sizeof(*lowpass_500hz);
      processor.changeFilterTaps(lowpass_500hz, size);
      break;
    case 3:
      size =  sizeof(lowpass_500to1000)/sizeof(*lowpass_500to1000);
      processor.changeFilterTaps(lowpass_500to1000, size);
      break;
    case 4:
      size =  sizeof(highCut_15000hz)/sizeof(*highCut_15000hz);
      processor.changeFilterTaps(highCut_15000hz, size);
      break;
    case 5:
      size =  sizeof(highCut_18000hz)/sizeof(*highCut_18000hz);
      processor.changeFilterTaps(highCut_18000hz, size);
      break;
    case 6:
      size =  sizeof(bandpass300to600)/sizeof(*bandpass300to600);
      processor.changeFilterTaps(bandpass300to600, size);
      break;
    case 7:
      size =  sizeof(notch_250hz)/sizeof(*notch_250hz);
      processor.changeFilterTaps(notch_250hz, size);
      break;
    case 8:
      size =  sizeof(highPass_40hz)/sizeof(*highPass_40hz);
      processor.changeFilterTaps(highPass_40hz, size);
      break;
    case 9:
      size =  sizeof(highPass_50hz)/sizeof(*highPass_50hz);
      processor.changeFilterTaps(highPass_50hz, size);
      break;
    case 10:
      size =  sizeof(highPass_600hz)/sizeof(*highPass_600hz);
      processor.changeFilterTaps(highPass_600hz, size);
      break;
    case 11:
      size =  sizeof(lowpass_2600hz)/sizeof(*lowpass_2600hz);
      processor.changeFilterTaps(lowpass_2600hz, size);
      break;

      
    default:
      break;
  }
}


