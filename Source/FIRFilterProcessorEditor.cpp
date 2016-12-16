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
: AudioProcessorEditor(p) , processor (*p), parent(*b), thumbnailCache(5),                           // [4]
thumbnail(64, formatManager, thumbnailCache)
{
  
  setSize(320, 160);
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();

  
  //COMBOBOX
  tapsSelectorComboBox = new ComboBox("tapsSelectorBox");
  tapsSelectorComboBox->addItemList({"Strong Lowpass 200hz","Medium Lowpass 500hz", "Soft Lowpass 500hz (church)","Cut 15k", "Cut 18k", "Bandpass 450hz", "Notch 250hz", "Cut below 40hz", "Cut below 50hz", "Highpass 600hz", "Lowpass 2600hz"}, 1);
  tapsSelectorComboBox->addListener(this);
  //auto value = dynamic_cast<const AudioParameterChoice*>(params[0]);
  //tapsSelectorComboBox->setSelectedId(value->getIndex() +1);
  parent.registerImmobileObject(*tapsSelectorComboBox);
  addAndMakeVisible(tapsSelectorComboBox);
  
  //BUTTON
  bypassButton = new TextButton("Bypass");
  bypassButton->addListener(this);
  parent.registerImmobileObject(*bypassButton);
  addAndMakeVisible(bypassButton);
  bypassButton->setToggleState(true, juce::dontSendNotification);
  
  openButton = new TextButton("Open");
  openButton->addListener(this);
  parent.registerImmobileObject(*openButton);
  addAndMakeVisible(openButton);

  formatManager.registerBasicFormats();
  thumbnail.addChangeListener(this);
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
  r.reduce(10, 10);
  
  openButton->setBounds(r.withWidth(80).withHeight(18));
  bypassButton->setBounds(r.withWidth(80).withY(r.getY() + 21).withHeight(18));
  tapsSelectorComboBox->setBounds(r.withWidth(80).withHeight(18).withY(r.getY() + 42));

  Rectangle<int> r1(getLocalBounds());
  r1.reduce(10, 10);
  r1.setWidth(r1.getWidth() - 90);
  r1.setX(r1.getX() + 90);

  g.setColour(Colours::lightgrey);
  g.fillRect(r1);

  g.setColour(Colours::blue);
  if (hasFile)
  {
    thumbnail.drawChannel(g, r1, 0.0, thumbnail.getTotalLength(), 0, 1.0f);
  }
}

void FIRFilterProcessorEditor::resized()
{
}

void FIRFilterProcessorEditor::buttonClicked(juce::Button *button)
{
  if (button == bypassButton)
  {
    if (bypassButton->getToggleState())
    {
      bypassButton->setToggleState(false, dontSendNotification);
      processor.bypass(false);
    }
    else
    {
      bypassButton->setToggleState(true, dontSendNotification);
      processor.bypass(true);
    }
  }
  else
  {
    FileChooser fileChoose("load WAV Impulse Response",
      File::getSpecialLocation(File::userHomeDirectory),
      "*.wav");
    if (fileChoose.browseForFileToOpen())
    {
      File impulse(fileChoose.getResult());
      processor.loadImpulse(impulse);


      AudioFormatReader* reader = formatManager.createReaderFor(impulse);
      if (reader != nullptr)
      {
        ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(reader, true);
        thumbnail.setSource(new FileInputSource(impulse));
        readerSource = newSource.release();
        hasFile = true;
      }
    }
  }
}

void FIRFilterProcessorEditor::buttonStateChanged(juce::Button *button)
{
  
}

void FIRFilterProcessorEditor::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
  int size = 0;
  processor.bypass(true);
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
      size =  sizeof(church)/sizeof(*church);
      processor.changeFilterTaps(church, size);
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
  hasFile = false;
  processor.bypass(false);
  repaint();
}

void FIRFilterProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
  repaint();
}

