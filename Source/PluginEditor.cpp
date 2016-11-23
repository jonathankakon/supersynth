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
#include "WaveGeneratorProcessor.h"


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

void SupersynthAudioProcessorEditor::addIOComponents()
{
  InternalIOProcessor* audioInput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioInputNode);
  InternalIOProcessor* audioOutput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioOutputNode);
  InternalIOProcessor* midiInput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::midiInputNode);
    
  addInternalProcessor(audioInput, 100, 30, true);
  midiInID = addInternalProcessor(midiInput, 100, 80, false);
  addInternalProcessor(audioOutput, 500, 440, true);
}

void SupersynthAudioProcessorEditor::enableAllInternalBuses(int outNode, int inNode) const
{
  processor.getNodeForId(outNode)->getProcessor()->enableAllBuses();
  processor.getNodeForId(inNode)->getProcessor()->enableAllBuses();
}

void SupersynthAudioProcessorEditor::addConnection(Connection* connection) const
{
  enableAllInternalBuses(connection->outputNodeId, connection->inputNodeId);
  int a = processor.addConnection(connection->outputNodeId, connection->outputNodeChannel, connection->inputNodeId, connection->inputNodeChannel);
  
  /*DBG("creating connection from: " << connection->outputNodeId);
  DBG(" to " << connection->inputNodeId);
  DBG("success: " << a);*/
}

void SupersynthAudioProcessorEditor::removeConnection(Connection& connection) const
{
  int a = processor.removeConnection(connection.outputNodeId, connection.outputNodeChannel, connection.inputNodeId, connection.inputNodeChannel);
  /*DBG("deleting connection from: " << connection.outputNodeId);
  DBG(" to " << connection.inputNodeId);
  DBG("success: " << a);*/
}

bool SupersynthAudioProcessorEditor::testConnection(Connection& connection, int dest_id) const
{
  int outputNodeId = connection.draggingToInput ? connection.outputNodeId : dest_id;
  int inputNodeId = connection.draggingToInput ? dest_id : connection.inputNodeId;

  enableAllInternalBuses(outputNodeId, inputNodeId);
  return processor.canConnect(outputNodeId, connection.outputNodeChannel, inputNodeId, connection.inputNodeChannel);
}

int SupersynthAudioProcessorEditor::addInternalProcessor(InternalIOProcessor* p, int x, int y, bool addToWorksheet) const
{
  AudioProcessorGraph::Node* node = processor.addNode(p);
  if (addToWorksheet)
  {
    ProcessorEditorBase* editor = static_cast<ProcessorEditorBase*>(p->createEditor());
    worksheet->addEditor(editor, x, y);
    editor->setNodeId(node->nodeId);
    editor->setConnectors();
    p->enableAllBuses();
    processor.addConnection(editor->getMixerNodeId(), 0, node->nodeId, 0);
  }
  return node->nodeId;
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

  if (proc->acceptsMidi())
  {
    processor.addConnection(midiInID, 4096, node->nodeId, 4096);
  }

  if (proc->hasEditor())
  {
    ProcessorEditorBase* editor = static_cast<ProcessorEditorBase*>(proc->createEditor());
    worksheet->addEditor(editor);
    editor->setNodeId(node->nodeId);
    editor->setConnectors();
  }
}

int SupersynthAudioProcessorEditor::addAudioProcessor(AudioProcessor* p, int nodeIdToConnect, int channelToConnect) const
{
  AudioProcessorGraph::Node* node = processor.addNode(p);
  processor.addConnection(node->nodeId, 0, nodeIdToConnect, channelToConnect);
  return node->nodeId;
}
