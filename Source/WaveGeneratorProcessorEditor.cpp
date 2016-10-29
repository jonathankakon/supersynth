/*
  ==============================================================================

    WaveGeneratorProcessorEditor.cpp
    Created: 7 Oct 2016 3:43:18pm
    Author:  Paul Lehmann

  ==============================================================================
*/

#include "WaveGeneratorProcessorEditor.h"
#include "WaveGeneratorProcessor.h"


//==============================================================================
WaveGeneratorProcessorEditor::WaveGeneratorProcessorEditor (WaveGeneratorProcessor& p)
: ProcessorEditorBase (&p, false, true, false), processor (p)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (300, 100);
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  const AudioParameterFloat* volumeParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  volumeSlider = new Slider(volumeParam->name);
  volumeSlider->setRange(0, 1);
  volumeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  volumeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  volumeSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(volumeParam)->getValue());
  volumeSlider->addListener(this);
  registerImmobileObject(*volumeSlider);
  addAndMakeVisible(volumeSlider);
  
  const AudioParameterFloat* frequencyParam = dynamic_cast<AudioParameterFloat*>(params[1]);
  frequencySlider = new Slider(frequencyParam->name);
  frequencySlider->setRange(0, 1, 0.0001);
  frequencySlider->setSliderStyle(Slider::RotaryVerticalDrag);
  frequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  frequencySlider->setValue(dynamic_cast<const AudioProcessorParameter*>(frequencyParam)->getValue());
  frequencySlider->addListener(this);
  registerImmobileObject(*frequencySlider);
  addAndMakeVisible(frequencySlider);

  const AudioParameterInt* octaveParam = dynamic_cast<AudioParameterInt*>(params[2]);
  octavesSlider = new Slider(octaveParam->name);
  octavesSlider->setRange(0, 1, 1.0f/6.0f);
  octavesSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  octavesSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  octavesSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(octaveParam)->getValue());
  octavesSlider->addListener(this);
  registerImmobileObject(*octavesSlider);
  addAndMakeVisible(octavesSlider);
  
  const AudioParameterInt* semitoneParam = dynamic_cast<AudioParameterInt*>(params[3]);
  semitonesSlider = new Slider(semitoneParam->name);
  semitonesSlider->setRange(0, 24, 1);
  semitonesSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  semitonesSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  semitonesSlider->setValue(semitoneParam->get());
  semitonesSlider->addListener(this);
  registerImmobileObject(*semitonesSlider);
  addAndMakeVisible(semitonesSlider);
  
  const AudioParameterInt* centParam = dynamic_cast<AudioParameterInt*>(params[4]);
  centsSlider = new Slider(centParam->name);
  centsSlider->setRange(0, 200, 1);
  centsSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  centsSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  centsSlider->setValue(centParam->get());
  centsSlider->addListener(this);
  registerImmobileObject(*centsSlider);
  addAndMakeVisible(centsSlider);
  
  
  setConnectors();
  volumeSlider->addListener(this);
   
  //label
  
  
  
  
}

WaveGeneratorProcessorEditor::~WaveGeneratorProcessorEditor()
{

}

//==============================================================================
void WaveGeneratorProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::lightgreen);
  g.setColour (Colours::black);
  g.setFont (15.0f);
  g.drawFittedText ("Wave Generator!", Rectangle<int>(200,15), Justification::centred, 1);
  
  Rectangle<int> r(30, 40 , 30, 30);
  volumeSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  frequencySlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  octavesSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  semitonesSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  centsSlider->setBounds(r);
  
}

void WaveGeneratorProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}


// copied from juce demo project GenericEditor.h
void WaveGeneratorProcessorEditor::sliderValueChanged (Slider* slider)
{
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  
  if (slider == volumeSlider)
  {
    DBG("volume 222: " << volumeSlider->getValue());
    AudioProcessorParameter* param = params[0];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) volumeSlider->getValue());
    else
      param->setValue ((float) volumeSlider->getValue());
  }
  else if (slider == frequencySlider)
  {
    DBG("freq 222: " << frequencySlider->getValue());
    AudioProcessorParameter* param = params[1];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost (frequencySlider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == octavesSlider)
  {
    AudioProcessorParameter* param = params[2];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) octavesSlider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  
  
}


