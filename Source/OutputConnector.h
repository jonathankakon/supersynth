/*
  ==============================================================================

    OutputConnector.h
    Created: 21 Oct 2016 4:02:11pm
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef OUTPUTCONNECTOR_H_INCLUDED
#define OUTPUTCONNECTOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Worksheet.h"

//==============================================================================
/*
*/
class OutputConnector : public Component, public MouseListener
{
public:
  OutputConnector(int processorNodeId);
  ~OutputConnector();

  void paint(Graphics&) override;
  void resized() override;

  void mouseDown(const MouseEvent& e) override { getWorksheet()->beginConnectorDrag(nodeId, 0, 0, 0, e); }
  int getNodeId() const { return nodeId; };
  static Point<int> getClosestConnector(const int x, const int y);
  void mouseDrag(const MouseEvent& e) override { getWorksheet()->dragConnector(e); };
  void mouseUp(const MouseEvent& e) override { getWorksheet()->endDraggingConnector(e); };

private:
  Worksheet* getWorksheet() const noexcept
  {
    return findParentComponentOfClass<Worksheet>(); 
  }

  int nodeId;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputConnector)
};


#endif  // OUTPUTCONNECTOR_H_INCLUDED
