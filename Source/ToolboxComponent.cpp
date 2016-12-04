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
  moduleList(new ListBox("ModuleList", nullptr))
{
  modules.add(new ModulesListElement("Wave Generator", "WaveGeneratorProcessor", &createInstance<WaveGeneratorProcessor>));
  modules.add(new ModulesListElement("IIR Filter", "FilterProcessor", &createInstance<FilterProcessor>));
  modules.add(new ModulesListElement("Equalizer", "EQFourProcessor", &createInstance<EQFourProcessor>));
  modules.add(new ModulesListElement("Envelope", "EnvelopeProcessor", &createInstance<EnvelopeProcessor>));
  modules.add(new ModulesListElement("NoiseGenerator", "NoiseGeneratorProcessor", &createInstance<NoiseGeneratorProcessor>));


  Rectangle<int> r(getLocalBounds());

  addAndMakeVisible(moduleList);
  moduleList->setColour(ListBox::ColourIds::backgroundColourId, Colour(0xFFC8C8C8));
  moduleList->setColour(ListBox::ColourIds::outlineColourId, Colour(0xFFC8C8C8));
  moduleList->setBounds(r);
  moduleList->setModel(this);
  moduleList->setMultipleSelectionEnabled(false);

  /*
  ComponentBoundsConstrainer* bounds = new ComponentBoundsConstrainer();
  bounds->setMaximumWidth(400);
  bounds->setMinimumWidth(150);
  resizeBorder = new ResizableBorderComponent(this, bounds);
  addAndMakeVisible(resizeBorder);
  resizeBorder->setBorderThickness(BorderSize<int>(0,0,0,3));
  resizeBorder->setBounds(r);
  */
}

ToolboxComponent::~ToolboxComponent()
{
	moduleList = nullptr;
	resizeBorder = nullptr;
}

void ToolboxComponent::paint(Graphics& g)
{
  g.fillAll(Colour(0xFF3F3F46)); 
}

void ToolboxComponent::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..

  Rectangle<int> r(getLocalBounds());

  moduleList->setBounds(r);
  //resizeBorder->setBounds(r);
}

