/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ToolboxComponent.h"
#include "InternalIOProcessor.h"
#include "ProcessorEditorBase.h"


//==============================================================================
SupersynthAudioProcessorEditor::SupersynthAudioProcessorEditor (SupersynthAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	toolbox(new ToolboxComponent()), 
	collapseButton(new CollapseButton()),
	viewport(new Viewport()),
	worksheet(new Worksheet())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setResizable(true, true);
	setResizeLimits(800, 500, 10000, 10000);

	addAndMakeVisible(toolbox);
	Rectangle<int> r(getLocalBounds());
	toolbox->setBounds(r.withWidth(180));

	addAndMakeVisible(collapseButton);
	collapseButton->setBounds(r.withWidth(200).withTrimmedLeft(180));
	collapseButton->addListener(this);

	addAndMakeVisible(viewport);
	viewport->setBounds(r.withTrimmedLeft(200));
	viewport->setScrollOnDragEnabled(true);
	viewport->setScrollBarsShown(true, true);
	viewport->setViewedComponent(worksheet);

	worksheet->addComponentListener(this);

  addIOComponents();
}

SupersynthAudioProcessorEditor::~SupersynthAudioProcessorEditor()
{
	collapseButton->removeListener(this);

	toolbox = nullptr;
	worksheet = nullptr;
	viewport = nullptr;
	collapseButton = nullptr;
}

void SupersynthAudioProcessorEditor::addIOComponents() const
{
  InternalIOProcessor* audioInput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioInputNode);
  InternalIOProcessor* audioOutput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioOutputNode);
  InternalIOProcessor* midiInput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::midiInputNode);

  AudioProcessorGraph::Node* aiNode = processor.addNode(audioInput);
  AudioProcessorGraph::Node* aoNode = processor.addNode(audioOutput);
  AudioProcessorGraph::Node* miNode = processor.addNode(midiInput);

  ProcessorEditorBase* editor = static_cast<ProcessorEditorBase*>(audioInput->createEditor());
  worksheet->addEditor(editor, 100, 30);
  editor->setConnectors();

  editor = static_cast<ProcessorEditorBase*>(audioOutput->createEditor());
  worksheet->addEditor(editor, 100, 80);
  editor->setConnectors();

  editor = static_cast<ProcessorEditorBase*>(midiInput->createEditor());
  worksheet->addEditor(editor, 500, 440);
  editor->setConnectors();
}

//==============================================================================
void SupersynthAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colour(0xFFC8C8C8));
    g.setColour (Colour(0xFFD955A9));
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void SupersynthAudioProcessorEditor::resized()
{
	Rectangle<int> r(getLocalBounds());
	toolbox->setBounds(r.withWidth(toolbox->getWidth()).withX(toolbox->getX()));
	Rectangle<int> toolsBounds(toolbox->getBounds());
	collapseButton->setBounds(r.withWidth(20).withX(toolsBounds.getRight()));
	viewport->setBounds(r.withTrimmedLeft(toolsBounds.getWidth() + toolsBounds.getX() + 20));

	if(viewport->getViewWidth() >= worksheet->getWidth())
	{
		worksheet->setSize(viewport->getViewWidth() + 100, worksheet->getHeight());
	}

	if (viewport->getViewHeight() >= worksheet->getHeight())
	{
		worksheet->setSize(worksheet->getWidth(), viewport->getViewHeight() + 100);
	}


    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void SupersynthAudioProcessorEditor::componentMovedOrResized(Component&	component, bool wasMoved, bool wasResized)
{
	if (&component == worksheet && wasMoved && !wasResized)
	{
		if (viewport->getWidth() + viewport->getViewPositionX() >= worksheet->getWidth() - 30)
			worksheet->setSize(worksheet->getWidth() + 50, worksheet->getHeight());
		
		if (viewport->getHeight() + viewport->getViewPositionY() >= worksheet->getHeight() - 30)
			worksheet->setSize(worksheet->getWidth(), worksheet->getHeight() + 50);

		worksheet->repaint();
	}
}


void SupersynthAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{

	if (buttonThatWasClicked == collapseButton)
	{
		open = collapseButton->getToggleState();

		if (open)
		{
			Desktop::getInstance().getAnimator().animateComponent(toolbox, toolbox->getBounds().withX(-(toolbox->getWidth())), 1, 100, true, 1, 0.01);
			Desktop::getInstance().getAnimator().animateComponent(collapseButton, collapseButton->getBounds().withX(0), 1, 100, true, 1, 0.01);
			Desktop::getInstance().getAnimator().animateComponent(viewport, viewport->getBounds().withX(20).withRight(this->getRight()), 1, 100, true, 1, 0.01);
		}
		else
		{
			Rectangle<int> newToolboxBounds(toolbox->getBounds().withX(0));
			Desktop::getInstance().getAnimator().animateComponent(toolbox, newToolboxBounds, 1, 100, true, 1, 0.01);
			Desktop::getInstance().getAnimator().animateComponent(collapseButton, collapseButton->getBounds().withX(newToolboxBounds.getRight()), 1, 100, true, 1, 0.01);
			Desktop::getInstance().getAnimator().animateComponent(viewport, viewport->getBounds().withX(newToolboxBounds.getRight() + 20).withRight(this->getRight()), 1, 100, true, 1, 0.01);
		}
	}
}

void SupersynthAudioProcessorEditor::setViewPortDragScrolling(bool allow) const
{
	viewport->setScrollOnDragEnabled(allow);
}

void SupersynthAudioProcessorEditor::addAudioProcessor(ToolboxComponent::ModulesListElement* element) const
{
  AudioProcessor* proc = element->getInstance();
  AudioProcessorGraph::Node* node = processor.addNode(proc);
  if (proc->hasEditor())
  {
    AudioProcessorEditor* editor =  proc->createEditor();
    worksheet->addEditor(editor);
    static_cast<ProcessorEditorBase*>(editor)->setConnectors();
  }

  /*
  if (processor.getNodeForId(1) == nullptr)
  {
    AudioProcessorGraph::AudioGraphIOProcessor* node = new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
    processor.addNode(node, 1);
  }

	if (processorType == 0)
	{
		WaveGeneratorProcessor* wave = new WaveGeneratorProcessor();
		AudioProcessorGraph::Node* generator = processor.addNode(wave);

    FilterProcessor* filter = new FilterProcessor();
    AudioProcessorGraph::Node* filterNode = processor.addNode(filter);
    
    EQFourProcessor* eqFour = new EQFourProcessor();
    AudioProcessorGraph::Node* eqNode = processor.addNode(eqFour);
    
    worksheet->addEditor(filter->createEditor());
    processor.addConnection(filterNode->nodeId, 0, eqNode->nodeId, 0);
    processor.getNodeForId(1)->getProcessor()->enableAllBuses();
    
    worksheet->addEditor(wave->createEditor());
    processor.addConnection(generator->nodeId, 0, filterNode->nodeId, 0);
    
    worksheet->addEditor(eqFour->createEditor());
    processor.addConnection(eqNode->nodeId, 0, 1, 0);
    processor.addConnection(eqNode->nodeId, 0, 1, 1);
    
	}
  else if (processorType == 1)
  {
    
    FilterProcessor* filter = new FilterProcessor();
    AudioProcessorGraph::Node* filterNode = processor.addNode(filter);
    //processor.addNode(filter);
    worksheet->addEditor(filter->createEditor());
    processor.addConnection(filterNode->nodeId, 0, 1, 0);
    processor.addConnection(filterNode->nodeId, 0, 1, 1);
  }*/
}

int SupersynthAudioProcessorEditor::addAudioProcessor(AudioProcessor* p) const
{
  AudioProcessorGraph::Node* node = processor.addNode(p);
  return node->nodeId;
}
