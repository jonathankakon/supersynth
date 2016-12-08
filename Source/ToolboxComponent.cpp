/*
  ==============================================================================

    ToolboxComponent.cpp
    Created: 9 Oct 2016 11:03:18am
    Author:  Jonathan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ToolboxComponent.h"

#include "WaveGeneratorProcessor.h"
#include "FilterProcessor.h"
#include "EQFourProcessorEditor.h"
#include "EnvelopeProcessor.h"
#include "NoiseGeneratorProcessor.h"
#include "DistortionProcessor.h"
#include "DelayProcessor.h"
#include "FIRFilterProcessor.h"
#include "PluginEditor.h"

int ToolboxComponent::getNumRows()
{
	return modules.size();
}

void ToolboxComponent::paintListBoxItem(int rowNumber, Graphics& g,
  int width, int height, bool rowIsSelected)
{
  g.fillAll(Colour(0xFFC8C8C8));
  g.setColour(Colour(0xFF1F1F1F));
  if (rowIsSelected)
  {
    g.fillAll(Colour(0xFF007ACC));
    g.setColour(Colour(0xFFFFFFFF));
  }

  g.setFont(height * 0.7f);

  g.drawText((modules[rowNumber])->name,
    5, 0, width, height,
    Justification::centredLeft, true);
}

var ToolboxComponent::getDragSourceDescription(const SparseSet<int>& selectedRows)
{
  // for our drag description, we'll just make a comma-separated list of the selected row
  // numbers - this will be picked up by the drag target and displayed in its box.
  if (selectedRows.size() > 0)
  {
    return new ToolboxComponent::ModulesListElement(*modules[selectedRows[0]]);
  }
  return 0;
}

//==============================================================================
ToolboxComponent::ToolboxComponent() :
  moduleList(new ListBox("ModuleList", nullptr)), saveButton(new TextButton("Save Preset")), loadButton(new TextButton("Load Preset")), enableAnimation(new TextButton("Animate"))
{
  modules.add(new ModulesListElement("Wave Generator", "WaveGeneratorProcessor", &createInstance<WaveGeneratorProcessor>));
  modules.add(new ModulesListElement("IIR Filter", "FilterProcessor", &createInstance<FilterProcessor>));
  modules.add(new ModulesListElement("Equalizer", "EQFourProcessor", &createInstance<EQFourProcessor>));
  modules.add(new ModulesListElement("Envelope", "EnvelopeProcessor", &createInstance<EnvelopeProcessor>));
  modules.add(new ModulesListElement("NoiseGenerator", "NoiseGeneratorProcessor", &createInstance<NoiseGeneratorProcessor>));
  modules.add(new ModulesListElement("Distortion", "DistortionProcessor", &createInstance<DistortionProcessor>));
  modules.add(new ModulesListElement("Delay", "DelayProcessor", &createInstance<DelayProcessor>));
  modules.add(new ModulesListElement("FIR Filter", "FIRFilterProcessor", &createInstance<FIRFilterProcessor>));


  Rectangle<int> r(getLocalBounds());

  addAndMakeVisible(moduleList);
  moduleList->setColour(ListBox::ColourIds::backgroundColourId, Colour(0xFFC8C8C8));
  moduleList->setColour(ListBox::ColourIds::outlineColourId, Colour(0xFFC8C8C8));
  moduleList->setBounds(r.withTrimmedBottom(65));
  moduleList->setModel(this);
  moduleList->setMultipleSelectionEnabled(false);


  saveButton->addListener(this);
  loadButton->addListener(this);
  enableAnimation->addListener(this);

  saveButton->setBounds(r.removeFromBottom(60).withHeight(20));
  loadButton->setBounds(r.removeFromBottom(40).withHeight(20));
  enableAnimation->setBounds(r.removeFromBottom(20).withHeight(20));

  addAndMakeVisible(saveButton);
  addAndMakeVisible(loadButton);
  addAndMakeVisible(enableAnimation);
}

ToolboxComponent::~ToolboxComponent()
{
	moduleList = nullptr;
	resizeBorder = nullptr;
}

void ToolboxComponent::paint(Graphics& g)
{
  g.fillAll(Colour(0xFFC8C8C8));
}

void ToolboxComponent::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..

  Rectangle<int> r(getLocalBounds());

  moduleList->setBounds(r.withTrimmedBottom(80));
  //resizeBorder->setBounds(r);

  Rectangle<int> buttons = r.removeFromBottom(75).withTrimmedLeft(10);
  saveButton->setBounds(buttons.withHeight(20));
  loadButton->setBounds(buttons.withHeight(20).withY(buttons.getY() + 23));
  enableAnimation->setBounds(buttons.withHeight(20).withY(buttons.getY() + 51));

}

void ToolboxComponent::buttonClicked(Button* button)
{
  if(button == saveButton)
  {
    findParentComponentOfClass<SupersynthAudioProcessorEditor>()->save(true, true);
  }
  else if (button == loadButton)
  {
    findParentComponentOfClass<SupersynthAudioProcessorEditor>()->loadFromUserSpecifiedFile(true);
  }
  else if (button == enableAnimation)
  {
    enableAnimation->setToggleState(!enableAnimation->getToggleState(), dontSendNotification);
    findParentComponentOfClass<SupersynthAudioProcessorEditor>()->setAnimating(enableAnimation->getToggleState());
  }
}
