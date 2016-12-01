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

//==============================================================================
/*
*/
class Worksheet : public Component, public DragAndDropTarget
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

  void 	mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;

  void addEditor(Component* editor);
  void addEditor(Component* editor, double x, double y);

  void beginConnectorDrag(int outputNodeId, int outputChannel, int inputNodeId, int inputChannel, const MouseEvent& e);
  void dragConnector(const MouseEvent& e);
  void endDraggingConnector(const MouseEvent& e);

  void registerComponentListener(Connection* connection, int inputNodeId, int outputNodeId);
  void clearEditorListeners(Connection* connection);

  bool findConnectorAt(const bool isInput, const int x, const int y, Point<int>& outPosition, int& nodeId) const;
  void removeEditor(int nodeId);
  void removeConnections(int nodeId, int mixerNodeId);
  void setZoomFactor(float zoomFactor);
  float getZoomFactor() const;
private:
  bool somethingIsBeingDraggedOver;
  float zoomFactor = 1;
  Point<int> dropPosition;
  String message;

  OwnedArray<Component> editors;
  OwnedArray<Connection> connections;

  ScopedPointer<Connection> draggingConnection;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Worksheet)
};


#endif  // WORKSHEET_H_INCLUDED
