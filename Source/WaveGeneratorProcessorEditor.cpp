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
WaveGeneratorProcessorEditor::WaveGeneratorProcessorEditor (WaveGeneratorProcessor* p, ProcessorEditorBase* b)
: AudioProcessorEditor (p), processor (*p), parent(*b)
{
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (150, 80);
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  const AudioParameterFloat* volumeParam = dynamic_cast<AudioParameterFloat*>(params[0]);
  volumeSlider = new Slider(volumeParam->name);
  volumeSlider->setRange(0, 1);
  volumeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  volumeSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  volumeSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(volumeParam)->getValue());
  volumeSlider->addListener(this);
  parent.registerImmobileObject(*volumeSlider);
  addAndMakeVisible(volumeSlider);
  
  const AudioParameterFloat* frequencyParam = dynamic_cast<AudioParameterFloat*>(params[1]);
  frequencySlider = new Slider(frequencyParam->name);
  frequencySlider->setRange(0, 1, 0.000001);
  frequencySlider->setSliderStyle(Slider::RotaryVerticalDrag);
  frequencySlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  frequencySlider->setValue(dynamic_cast<const AudioProcessorParameter*>(frequencyParam)->getValue());
  frequencySlider->addListener(this);
  parent.registerImmobileObject(*frequencySlider);
  addAndMakeVisible(frequencySlider);

  const AudioParameterInt* octaveParam = dynamic_cast<AudioParameterInt*>(params[2]);
  octavesSlider = new Slider(octaveParam->name);
  octavesSlider->setRange(0, 1, 1.0f/6.0f);
  octavesSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  octavesSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  octavesSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(octaveParam)->getValue());
  octavesSlider->addListener(this);
  parent.registerImmobileObject(*octavesSlider);
  addAndMakeVisible(octavesSlider);
  
  const AudioParameterInt* semitoneParam = dynamic_cast<AudioParameterInt*>(params[3]);
  semitonesSlider = new Slider(semitoneParam->name);
  semitonesSlider->setRange(0, 1, 1.0f/24.0f);
  semitonesSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  semitonesSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  semitonesSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(semitoneParam)->getValue());
  semitonesSlider->addListener(this);
  parent.registerImmobileObject(*semitonesSlider);
  addAndMakeVisible(semitonesSlider);
  
  const AudioParameterInt* centParam = dynamic_cast<AudioParameterInt*>(params[4]);
  centsSlider = new Slider(centParam->name);
  centsSlider->setRange(0, 1, 1.0f/200.0f);
  centsSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  centsSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  centsSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(centParam)->getValue());
  centsSlider->addListener(this);
  parent.registerImmobileObject(*centsSlider);
  addAndMakeVisible(centsSlider);
  
  const AudioParameterChoice* waveformParam = dynamic_cast<AudioParameterChoice*>(params[5]);
  waveformSlider = new Slider(waveformParam->name);
  waveformSlider->setRange(0, 1, 0.25);
  waveformSlider->setSliderStyle(Slider::RotaryVerticalDrag);
  waveformSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  waveformSlider->setValue(dynamic_cast<const AudioProcessorParameter*>(waveformParam)->getValue());
  waveformSlider->addListener(this);
  parent.registerImmobileObject(*waveformSlider);
  addAndMakeVisible(waveformSlider);
  
  
  volumeSlider->addListener(this);
   
  //labels
  
  
  blepSlider = new Slider();
  blepSlider->setRange(0, 1, 1);
  blepSlider->setSliderStyle(Slider::LinearVertical);
  blepSlider->setValue(1);
  blepSlider->addListener(this);
  parent.registerImmobileObject(*blepSlider);
  addAndMakeVisible(blepSlider);
  
}

WaveGeneratorProcessorEditor::~WaveGeneratorProcessorEditor()
{

}

//==============================================================================
void WaveGeneratorProcessorEditor::paint (Graphics& g)
{
  g.fillAll (Colours::lightgreen);
  
  Rectangle<int> r(10, 5 , 30, 30);
  volumeSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  waveformSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  frequencySlider->setBounds(r);
  
  r.setX(r.getX() - 80);
  r.setY(r.getY() + 40);
  octavesSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  semitonesSlider->setBounds(r);
  
  r.setX(r.getX() + 40);
  centsSlider->setBounds(r);
  
  r.setX(r.getX() + 30);
  r.setY(r.getY() - 20);
  blepSlider->setBounds(r);
  
}

void WaveGeneratorProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}


// copied from juce demo project GenericEditor.h
void WaveGeneratorProcessorEditor::sliderValueChanged(Slider* slider)
{
  
  const OwnedArray<AudioProcessorParameter>& params = processor.getParameters();
  
  
  if (slider == volumeSlider)
  {
    DBG("volume 222: " << volumeSlider->getValue());
    AudioProcessorParameter* param = params[0];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == frequencySlider)
  {
    DBG("freq 222: " << frequencySlider->getValue());
    AudioProcessorParameter* param = params[1];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost (slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == octavesSlider)
  {
    AudioProcessorParameter* param = params[2];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == semitonesSlider)
  {
    AudioProcessorParameter* param = params[3];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == centsSlider)
  {
    AudioProcessorParameter* param = params[4];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) slider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == waveformSlider)
  {
    AudioProcessorParameter* param = params[5];
    if (slider->isMouseButtonDown())
      param->setValueNotifyingHost ((float) waveformSlider->getValue());
    else
      param->setValue ((float) slider->getValue());
  }
  else if (slider == blepSlider)
  {
    processor.setBlepOn(slider->getValue() );
  }
  
}


