/*
  ==============================================================================

    Worksheet.h
    Created: 10 Oct 2016 6:15:42pm
    Author:  Jonathan

  ==============================================================================
*/

#ifndef WORKSHEET_H_INCLUDED
#define WORKSHEET_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Connection.h"
#include "PopupDisplay.h"

//==============================================================================
/*
*/
class Worksheet : public Component, public DragAndDropTarget, public ComponentListener, Timer
{
public:
  Worksheet(int, int);
  ~Worksheet();



  void paint(Graphics&) override;
  void resized() override;

  bool isInterestedInDragSource(const SourceDetails& sourceDetails) override;
  void itemDragEnter(const SourceDetails& sourceDetails) override;
  void itemDragMove(const SourceDetails& sourceDetails) override;
  void itemDragExit(const SourceDetails& sourceDetails) override;
  void itemDropped(const SourceDetails& sourceDetails) override;
  void componentMovedOrResized(Component&, bool, bool) override; 
  void timerCallback() override;

  void 	mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;

  void addEditor(Component* editor);
  void addEditor(Component* editor, double x, double y);

  void beginConnectorDrag(int outputNodeId, int outputChannel, int inputNodeId, int inputChannel, const MouseEvent& e);
  String getTooltipTextAt(const Point<int> point);
  void dragConnector(const MouseEvent& e);
  void endDraggingConnector(const MouseEvent& e);

  void registerComponentListener(Connection* connection, int inputNodeId, int outputNodeId);
  void clearEditorListeners(Connection* connection);

  bool findConnectorAt(const bool isInput, const int x, const int y, Point<int>& outPosition, int& nodeId) const;
  void removeEditor(int nodeId);
  void removeConnections(int nodeId, int mixerNodeId);
  void setZoomFactor(float zoomFactor);
  float getZoomFactor() const;

  int getEditorX(int nodeId);
  int getEditorY(int nodeId);
  StringPairArray getInputAndChannelsOfEditor(int nodeId);
  Rectangle<int> getConnectionRectangle(const int sourceId, const int destId, const int sourceChannel, const int destChannel);
  void addConnection(int x0, int y0, int x1, int y1, int sourceNodeId, int sourceChannelId, int destNodeId, int destChannelId);
private:
  bool somethingIsBeingDraggedOver;
  float zoomFactor = 1;
  Point<int> dropPosition;
  String message;

  OwnedArray<Component> editors;
  OwnedArray<Connection> connections;

  SafePointer<Connection> draggingConnection;
  ScopedPointer<TooltipWindow> tooltipWindow;
  ScopedPointer<PopupDisplay> popup;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Worksheet)
};


#endif  // WORKSHEET_H_INCLUDED
