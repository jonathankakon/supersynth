/*
  ==============================================================================

    Connection.h
    Created: 28 Oct 2016 4:18:36pm
    Author:  jonathan kakon

  ==============================================================================
*/
#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
#pragma warning( disable : 4521 ) 
class Worksheet;

class Connection : public Component, public ComponentListener
{
public:
  Connection(int inId, int inChannel, int outId, int outChannel);
  Connection(Connection& c);
  ~Connection();

  void paint(Graphics&) override;
  void resized() override;

  bool hitTest(int x, int y) override;
  void mouseDown(const MouseEvent&) override;

  void mouseDrag(const MouseEvent& e) override;
  void mouseUp(const MouseEvent& e) override;

  void componentMovedOrResized(Component& component, bool wasMoved, bool wasResized) override;

  void dragEnd(int x, int y);
  void dragStart(int x, int y);

  void update();

  void setFixedConnectorPosition(const Point<int>& point);
  void setDestNodeId(int nodeId);;

  float getX1() const { return lastInputX; }
  float getX2() const { return lastOutputX; };
  float getY1() const { return lastInputY; };
  float getY2() const { return lastOutputY; };
  bool getStopPathRedraw() const { return stopPathRedraw; };

  int inputNodeId;
  int inputNodeChannel;
  int outputNodeId;
  int outputNodeChannel;

  bool draggingToInput;
private:

  Worksheet* getWorksheet() const noexcept
  {
    return findParentComponentOfClass<Worksheet>();
  }

  Path path, hitPath;
  bool dragging;
  bool stopPathRedraw;

  float lastInputX, lastOutputX, lastInputY, lastOutputY;

  void getDistancesFromEnds(int x, int y, double& distanceFromStart, double& distanceFromEnd) const;
  void getPoints(float& x1, float& y1, float& x2, float& y2) const;
  void resizeToFit();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Connection)
};


#endif  // CONNECTION_H_INCLUDED
