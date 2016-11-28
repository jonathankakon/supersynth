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
	collapseButton(new CollapseButton())
{
  setResizable(true, true);
  if(processor.stateInformation == nullptr)
  {
    viewport = new Viewport();
    worksheet = new Worksheet(1200,1200);
    viewport->setScrollOnDragEnabled(true);
    viewport->setScrollBarsShown(true, true);
    viewport->setViewedComponent(worksheet);

    processor.stateInformation = new XmlElement("SuperSynthAudioProcessor");
    XmlElement* graphElement = new XmlElement("Graph");
    processor.stateInformation->prependChildElement(graphElement);

    processor.stateInformation->setAttribute("width", this->getWidth());
    processor.stateInformation->setAttribute("height", this->getHeight());

    graphElement->setAttribute("width", worksheet->getWidth());
    graphElement->setAttribute("height", worksheet->getHeight());
    graphElement->setAttribute("zoomFactor", worksheet->getZoomFactor());
    graphElement->setAttribute("scrollX", viewport->getViewPositionX());
    graphElement->setAttribute("scrollY", viewport->getViewPositionY());
  }
  else
  {
    XmlElement* graphElement = processor.stateInformation->getChildByName("Graph");
    int width = graphElement->getIntAttribute("width");
    int height = graphElement->getIntAttribute("height");
    int scrollx = graphElement->getIntAttribute("scrollX");
    int scrolly = graphElement->getIntAttribute("scrollY");
    float zoomFactor = graphElement->getDoubleAttribute("zoomFactor", 1);

    viewport = new Viewport();
    worksheet = new Worksheet(width, height);
    worksheet->setZoomFactor(zoomFactor);
    viewport->setScrollOnDragEnabled(true);
    viewport->setScrollBarsShown(true, true);
    viewport->setViewedComponent(worksheet);
    viewport->setViewPosition(scrollx, scrolly);
    setSize(processor.stateInformation->getIntAttribute("width", 800), processor.stateInformation->getIntAttribute("height", 500));
  }

  addAndMakeVisible(toolbox);
  addAndMakeVisible(collapseButton);
  addAndMakeVisible(viewport);

  collapseButton->addListener(this);
  worksheet->addComponentListener(this);

  addIOComponents();

  Rectangle<int> r(getLocalBounds());
  viewport->setBounds(r.withTrimmedLeft(200));
  toolbox->setBounds(r.withWidth(180));
  collapseButton->setBounds(r.withWidth(200).withTrimmedLeft(180));
  setResizeLimits(800, 500, 10000, 10000);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	
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
  processor.graph->getNodeForId(outNode)->getProcessor()->enableAllBuses();
  processor.graph->getNodeForId(inNode)->getProcessor()->enableAllBuses();
}

void SupersynthAudioProcessorEditor::addConnection(Connection* connection) const
{
  enableAllInternalBuses(connection->outputNodeId, connection->inputNodeId);
  processor.graph->addConnection(connection->outputNodeId, connection->outputNodeChannel, connection->inputNodeId, connection->inputNodeChannel);
}

void SupersynthAudioProcessorEditor::removeConnection(Connection& connection) const
{
  processor.graph->removeConnection(connection.outputNodeId, connection.outputNodeChannel, connection.inputNodeId, connection.inputNodeChannel);
}

bool SupersynthAudioProcessorEditor::testConnection(Connection& connection, int dest_id) const
{
  int outputNodeId = connection.draggingToInput ? connection.outputNodeId : dest_id;
  int inputNodeId = connection.draggingToInput ? dest_id : connection.inputNodeId;

  enableAllInternalBuses(outputNodeId, inputNodeId);
  return processor.graph->canConnect(outputNodeId, connection.outputNodeChannel, inputNodeId, connection.inputNodeChannel);
}

void SupersynthAudioProcessorEditor::setUIStateInformation() const
{
  if (processor.stateInformation != nullptr)
  {
    processor.stateInformation->setAttribute("width", this->getWidth());
    processor.stateInformation->setAttribute("height", this->getHeight());

    XmlElement* graphElement = processor.stateInformation->getChildByName("Graph");
    graphElement->setAttribute("width", worksheet->getWidth());
    graphElement->setAttribute("height", worksheet->getHeight());
    graphElement->setAttribute("zoomFactor", worksheet->getZoomFactor());
    graphElement->setAttribute("scrollX", viewport->getViewPositionX());
    graphElement->setAttribute("scrollY", viewport->getViewPositionY());
  }
}

int SupersynthAudioProcessorEditor::addInternalProcessor(InternalIOProcessor* p, int x, int y, bool addToWorksheet) const
{
  AudioProcessorGraph::Node* node = processor.graph->addNode(p);
  if (addToWorksheet)
  {
    ProcessorEditorBase* editor = static_cast<ProcessorEditorBase*>(p->createEditor());
    worksheet->addEditor(editor, x, y);
    editor->setNodeId(node->nodeId);
    editor->setConnectors();
    p->enableAllBuses();
    processor.graph->addConnection(editor->getMixerNodeIds().getFirst(), 0, node->nodeId, 0);
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

	if(viewport->getViewWidth() / worksheet->getZoomFactor()>= worksheet->getWidth())
	{
		worksheet->setSize(viewport->getViewWidth() + 100, worksheet->getHeight());
	}

	if (viewport->getViewHeight() / worksheet->getZoomFactor() >= worksheet->getHeight())
	{
		worksheet->setSize(worksheet->getWidth(), viewport->getViewHeight() + 100);
	}


    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
  setUIStateInformation();
}

void SupersynthAudioProcessorEditor::componentMovedOrResized(Component&	component, bool wasMoved, bool wasResized)
{
	if (&component == worksheet)
	{
    DBG("viewpoert: " << viewport->getWidth() + viewport->getViewPositionX());
    DBG("worksheet: " << (worksheet->getWidth() - 50) * worksheet->getZoomFactor());
		if (viewport->getWidth() + viewport->getViewPositionX() >= (worksheet->getWidth() - 50) * worksheet->getZoomFactor())
			worksheet->setSize(worksheet->getWidth() + 30, worksheet->getHeight());
		
		if (viewport->getHeight() + viewport->getViewPositionY() >= (worksheet->getHeight() - 50) * worksheet->getZoomFactor())
			worksheet->setSize(worksheet->getWidth(), worksheet->getHeight() + 30);

		worksheet->repaint();
	}

  setUIStateInformation();
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
  AudioProcessorGraph::Node* node = processor.graph->addNode(proc);

  if (proc->acceptsMidi())
  {
    processor.graph->addConnection(midiInID, 4096, node->nodeId, 4096);
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
  AudioProcessorGraph::Node* node = processor.graph->addNode(p);
  processor.graph->addConnection(node->nodeId, 0, nodeIdToConnect, channelToConnect);
  return node->nodeId;
}

void SupersynthAudioProcessorEditor::removeAudioProcessor(int nodeId, Array<int> mixerNodeIds) const
{
  processor.graph->removeNode(nodeId);
  worksheet->removeEditor(nodeId);

  for (int mixerId : mixerNodeIds)
  {
    processor.graph->removeNode(mixerId);
    worksheet->removeConnections(nodeId, mixerId);
  }

  setViewPortDragScrolling(true);
}
