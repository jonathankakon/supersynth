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
  }

EnvelopeProcessorEditor::~EnvelopeProcessorEditor()
{
    
  }


void EnvelopeProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::lightgoldenrodyellow);
    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Envelope!", Rectangle<int>(200,15), Justification::centred, 1);
  }

void EnvelopeProcessorEditor::resized()
{
    
  }

void EnvelopeProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    
  }