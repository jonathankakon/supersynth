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
	collapseButton(new CollapseButton()), manager(new UndoManager())
{
  setResizable(true, true);
  if(processor.stateInformation.getNumChildren() == 0)
  {
    isLoading = true;
    ValueTree root = processor.stateInformation.getOrCreateChildWithName("Supersynth", manager);
    viewport = new Viewport();
    worksheet = new Worksheet(1200,1200);
    viewport->setScrollOnDragEnabled(true);
    viewport->setScrollBarsShown(true, true);
    viewport->setViewedComponent(worksheet);
   
    root.setProperty("width", this->getWidth(), nullptr);
    root.setProperty("height", this->getHeight(), nullptr);

    ValueTree graphElement = root.getOrCreateChildWithName("Graph", manager);

    graphElement.setProperty("width", worksheet->getWidth(), manager);
    graphElement.setProperty("height", worksheet->getHeight(), manager);
    graphElement.setProperty("zoomFactor", worksheet->getZoomFactor(), manager);
    graphElement.setProperty("scrollX", viewport->getViewPositionX(), manager);
    graphElement.setProperty("scrollY", viewport->getViewPositionY(), manager);

    addAndMakeVisible(toolbox);
    addAndMakeVisible(collapseButton);
    addAndMakeVisible(viewport);

    addIOComponents();
    graphElement.setProperty("midiNodeId", midiInID, manager);
    SerializeGraph(graphElement);
    isLoading = false;
  }
  else
  {
    isLoading = true;
    ValueTree root = processor.stateInformation.getChildWithName("Supersynth");
    ValueTree graphElement = root.getChildWithName("Graph");
    int width = graphElement.getProperty("width");
    int height = graphElement.getProperty("height");
    int scrollx = graphElement.getProperty("scrollX");
    int scrolly = graphElement.getProperty("scrollY");
    float zoomFactor = graphElement.getProperty("zoomFactor", 1);
    midiInID = graphElement.getProperty("midiNodeId");

    viewport = new Viewport();
    worksheet = new Worksheet(width, height);
    worksheet->setZoomFactor(zoomFactor);
    viewport->setScrollOnDragEnabled(true);
    viewport->setScrollBarsShown(true, true);
    viewport->setViewedComponent(worksheet);
    viewport->setViewPosition(scrollx, scrolly);

    setSize(root.getProperty("width", 800), root.getProperty("height", 500));

    addAndMakeVisible(toolbox);
    addAndMakeVisible(collapseButton);
    addAndMakeVisible(viewport);

    DeserializeGraph(graphElement);
    isLoading = false;
  }

  collapseButton->addListener(this);
  worksheet->addComponentListener(this);

  Rectangle<int> r(getLocalBounds());
  viewport->setBounds(r.withTrimmedLeft(200));
  toolbox->setBounds(r.withWidth(180));
  collapseButton->setBounds(r.withWidth(200).withTrimmedLeft(180));
  setResizeLimits(800, 500, 10000, 10000);
}

SupersynthAudioProcessorEditor::~SupersynthAudioProcessorEditor()
{
	collapseButton->removeListener(this);
}

void SupersynthAudioProcessorEditor::addIOComponents()
{
  InternalIOProcessor* audioInput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioInputNode);
  InternalIOProcessor* audioOutput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioOutputNode);
  InternalIOProcessor* midiInput = new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::midiInputNode);

  midiInID = addInternalProcessor(midiInput, 100, 80, false);
  addInternalProcessor(audioInput, 100, 30, true);
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
  if (!isLoading)
  {
    SerializeGraph(processor.stateInformation.getChildWithName("Supersynth").getOrCreateChildWithName("Graph", manager));
  }
}

void SupersynthAudioProcessorEditor::removeConnection(Connection& connection) const
{
  processor.graph->removeConnection(connection.outputNodeId, connection.outputNodeChannel, connection.inputNodeId, connection.inputNodeChannel);
  if (!isLoading)
  {
    SerializeGraph(processor.stateInformation.getChildWithName("Supersynth").getOrCreateChildWithName("Graph", manager));
  }
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
  ValueTree root = processor.stateInformation.getOrCreateChildWithName("Supersynth", manager);
  root.setProperty("width", this->getWidth(), manager);
  root.setProperty("height", this->getHeight(), manager);

  ValueTree graphElement = root.getOrCreateChildWithName("Graph", manager);
  graphElement.setProperty("width", worksheet->getWidth(), manager);
  graphElement.setProperty("height", worksheet->getHeight(), manager);
  graphElement.setProperty("zoomFactor", worksheet->getZoomFactor(), manager);
  graphElement.setProperty("scrollX", viewport->getViewPositionX(), manager);
  graphElement.setProperty("scrollY", viewport->getViewPositionY(), manager);
}

void SupersynthAudioProcessorEditor::audioProcessorParameterChanged(AudioProcessor* /*p*/, int /*pi*/, float /*n*/)
{
  if (!isLoading)
  {
    SerializeGraph(processor.stateInformation.getChildWithName("Supersynth").getOrCreateChildWithName("Graph", manager));
  }
}

void SupersynthAudioProcessorEditor::audioProcessorChanged(AudioProcessor*)
{
  if (!isLoading)
  {
    SerializeGraph(processor.stateInformation.getChildWithName("Supersynth").getOrCreateChildWithName("Graph", manager));
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

void SupersynthAudioProcessorEditor::SerializeProcessors(ValueTree processors) const
{
  for(int i = 0; i < processor.graph->getNumNodes(); ++i)
  {
    AudioProcessorGraph::Node* nodeProcessor = processor.graph->getNode(i);
    int nodeId = nodeProcessor->nodeId;
    String name = nodeProcessor->getProcessor()->getName();
    if (name != "InputConnectorProcessor" || nodeId == midiInID)
    {
      ValueTree node("Processor");
      processors.addChild(node, -1, manager);

      node.setProperty("type", name, manager);
      node.setProperty("id", nodeId, manager);
      node.setProperty("xpos", worksheet->getEditorX(nodeId), manager);
      node.setProperty("ypos", worksheet->getEditorY(nodeId), manager);

      if(name == "InternalIOProcessor")
      {
        InternalIOProcessor* io = reinterpret_cast<InternalIOProcessor*>(nodeProcessor->getProcessor());
        node.setProperty("isInput", io->isInput(), manager);
      }

      StringPairArray inputIDs = worksheet->getInputAndChannelsOfEditor(nodeId);
      for(int k = 0; k < inputIDs.size(); ++k)
      {
        ValueTree input("Input");
        node.addChild(input, -1, manager);
        input.setProperty("id", inputIDs.getAllKeys()[k], manager);
        input.setProperty("connectedTo", inputIDs.getAllValues()[k], manager);
      }

      for (int k = 0; k < nodeProcessor->getProcessor()->getNumParameters(); ++k)
      {
        ValueTree param("Parameter");
        node.addChild(param, -1, manager);
        param.setProperty("paramId", k, manager);
        param.setProperty("value", nodeProcessor->getProcessor()->getParameter(k), manager);
      }
    }
  }
}

void SupersynthAudioProcessorEditor::SerializeConnections(ValueTree connections) const
{
  for(int i = 0; i < processor.graph->getNumConnections(); ++i)
  {
    if (!isMixerOrMidiConnection(processor.graph->getConnection(i)))
    {
      ValueTree connection("Connection");
      connections.addChild(connection, -1, manager);
      const AudioProcessorGraph::Connection* c = processor.graph->getConnection(i);

      connection.setProperty("source", (int)c->sourceNodeId, manager);
      connection.setProperty("dest", (int)c->destNodeId, manager);
      connection.setProperty("sourceChannel", (int)c->sourceChannelIndex, manager);
      connection.setProperty("destChannel", (int)c->destChannelIndex, manager);

      Rectangle<int> connectionRect = worksheet->getConnectionRectangle(c->sourceNodeId, c->destNodeId, c->sourceChannelIndex, c->destChannelIndex);
      connection.setProperty("x0", connectionRect.getX(), manager);
      connection.setProperty("x1", connectionRect.getWidth(), manager);
      connection.setProperty("y0", connectionRect.getY(), manager);
      connection.setProperty("y1", connectionRect.getHeight(), manager);
    }
  }
}

void SupersynthAudioProcessorEditor::SerializeGraph(ValueTree element) const
{
  element.removeAllChildren(manager);
  ValueTree processors = element.getOrCreateChildWithName("Processors", manager);
  ValueTree connections = element.getOrCreateChildWithName("Connections", manager);
  SerializeProcessors(processors);
  SerializeConnections(connections);
}

void SupersynthAudioProcessorEditor::DeserializeProcessors(ValueTree processors)
{
  if (processor.graph->getNodeForId(midiInID) == nullptr)
  {
    midiInID = addInternalProcessor(new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::midiInputNode), 0, 0, false);
  }

  for (int i = 0; i < processors.getNumChildren(); ++i)
  {
    ValueTree processorTree = processors.getChild(i);
    

    AudioProcessorGraph::Node* node;
    AudioProcessor* p;

    String name = processorTree.getProperty("type");
    int id = processorTree.getProperty("id");
    int xpos = processorTree.getProperty("xpos");
    int ypos = processorTree.getProperty("ypos");

    node = processor.graph->getNodeForId(id);
    if (node == nullptr)
    {
      if (name == "InternalIOProcessor")
      {
        bool isInputNode = processorTree.getProperty("isInput");
        p = isInputNode ? new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioInputNode)
          : new InternalIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::IODeviceType::audioOutputNode);
        p->enableAllBuses();
      }
      else
      {
        p = getProcessorFromClassName(name);
      }
      node = processor.graph->addNode(p, id);

      for (int k = 0; k < processorTree.getNumChildren(); ++k)
      {
        if (processorTree.getChild(k).hasType("Parameter"))
        {
          ValueTree parameter = processorTree.getChild(k);
          p->setParameter(parameter.getProperty("paramId"), parameter.getProperty("value"));
        }
      }
    }
    else
    {
      p = node->getProcessor();
    }

    if (p->acceptsMidi())
    {
      processor.graph->addConnection(midiInID, 4096, node->nodeId, 4096);
    }

    if (p->hasEditor() && node->nodeId != midiInID)
    {
      ProcessorEditorBase* editor = static_cast<ProcessorEditorBase*>(p->createEditor());
      worksheet->addEditor(editor);
      editor->setNodeId(node->nodeId);


      for (int k = 0; k < processorTree.getNumChildren(); ++k)
      {
        if (processorTree.getChild(k).hasType("Input"))
        {
          ValueTree input = processorTree.getChild(k);
          int mixerId = input.getProperty("id");
          AudioProcessor* mixerProcessor = processor.graph->getNodeForId(mixerId) != nullptr 
              ? processor.graph->getNodeForId(mixerId)->getProcessor() 
              : nullptr;
          editor->setConnector(mixerProcessor, mixerId, input.getProperty("connectedTo"));
        }
      }
      editor->setOutputConnectors();
      editor->setBounds(xpos, ypos, editor->getWidth(), editor->getHeight());
    }
  }
}

void SupersynthAudioProcessorEditor::DeserializeConnections(ValueTree connections)
{
  for(int i = 0; i <  connections.getNumChildren(); ++i)
  {
    ValueTree connection = connections.getChild(i);

    int sourceNodeId = connection.getProperty("source");
    int destNodeId = connection.getProperty("dest");
    int sourceChannelId = connection.getProperty("sourceChannel");
    int destChannelId = connection.getProperty("destChannel");

    int x0 = connection.getProperty("x0");
    int x1 = connection.getProperty("x1");
    int y0 = connection.getProperty("y0");
    int y1 = connection.getProperty("y1");

    enableAllInternalBuses(destNodeId, sourceNodeId);
    processor.graph->addConnection(sourceNodeId, sourceChannelId, destNodeId, destChannelId);

    worksheet->addConnection(x0, y0, x1, y1, sourceNodeId, sourceChannelId, destNodeId, destChannelId);
  }
}

void SupersynthAudioProcessorEditor::DeserializeGraph(ValueTree element)
{
  ValueTree processors = element.getOrCreateChildWithName("Processors", manager);
  ValueTree connections = element.getOrCreateChildWithName("Connections", manager);
  DeserializeProcessors(processors);
  DeserializeConnections(connections);
}

AudioProcessor* SupersynthAudioProcessorEditor::getProcessorFromClassName(String className) const
{
  for(ToolboxComponent::ModulesListElement* element : toolbox->modules)
  {
    if(element->className == className)
    {
      return element->getInstance();
    }
  }
  return nullptr;
}

bool SupersynthAudioProcessorEditor::isMixerOrMidiConnection(const AudioProcessorGraph::Connection* connection) const
{
  AudioProcessorGraph::Node* node = processor.graph->getNodeForId(connection->sourceNodeId);
  if (node->getProcessor()->getName() == "InputConnectorProcessor" || connection->sourceChannelIndex == 4096)
  {
    return true;
  }
  return false;
}

//==============================================================================
void SupersynthAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll (Colour(0xFFC8C8C8));
    g.setColour (Colour(0xFFD955A9));
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

void SupersynthAudioProcessorEditor::componentMovedOrResized(Component&	component, bool /*wasMoved*/, bool /*wasResized*/)
{
	if (&component == worksheet)
	{
		if (viewport->getWidth() + viewport->getViewPositionX() >= (worksheet->getWidth() - 50) * worksheet->getZoomFactor())
			worksheet->setSize(worksheet->getWidth() + 30, worksheet->getHeight());
		
		if (viewport->getHeight() + viewport->getViewPositionY() >= (worksheet->getHeight() - 50) * worksheet->getZoomFactor())
			worksheet->setSize(worksheet->getWidth(), worksheet->getHeight() + 30);

		worksheet->repaint();
	}

  setUIStateInformation();

  if(!isLoading)
  {
    SerializeGraph(processor.stateInformation.getChildWithName("Supersynth").getOrCreateChildWithName("Graph", manager));
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

  if (!isLoading)
  {
    SerializeGraph(processor.stateInformation.getChildWithName("Supersynth").getOrCreateChildWithName("Graph", manager));
  }
}

int SupersynthAudioProcessorEditor::addAudioProcessor(AudioProcessor* p, int nodeIdToConnect, int channelToConnect) const
{
  AudioProcessorGraph::Node* node = processor.graph->addNode(p);
  p->enableAllBuses();
  processor.graph->addConnection(node->nodeId, 0, nodeIdToConnect, channelToConnect);
  return node->nodeId;
}

void SupersynthAudioProcessorEditor::addAudioProcessor(AudioProcessor* p, int nodeIdToConnect, int mixerId, int channelToConnect) const
{
  AudioProcessorGraph::Node* node = processor.graph->addNode(p, mixerId);
  p->enableAllBuses();
  processor.graph->addConnection(node->nodeId, 0, nodeIdToConnect, channelToConnect);
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
  worksheet->removeConnections(nodeId, -1);

  setViewPortDragScrolling(true);
}
