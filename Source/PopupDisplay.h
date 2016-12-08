/*
  ==============================================================================

    PopupDisplay.h
    Created: 8 Dec 2016 11:50:35am
    Author:  jonathan kakon

  ==============================================================================
*/

#ifndef POPUPDISPLAY_H_INCLUDED
#define POPUPDISPLAY_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"

class PopupDisplay : public BubbleComponent, public Timer
{
public:
  PopupDisplay(Component& s)
    : owner(s)
  {
    setAlwaysOnTop(true);
    setAllowedPlacement(8);
    setLookAndFeel(&s.getLookAndFeel());
  }

  void paintContent(Graphics& g, int w, int h) override
  {
    g.setFont(font);
    g.setColour(owner.findColour(TooltipWindow::textColourId, true));
    g.drawFittedText(text, Rectangle<int>(w, h), Justification::centred, 1);
  }

  void getContentSize(int& w, int& h) override
  {
    w = font.getStringWidth(text) + 18;
    h = (int)(font.getHeight() * 1.6f);
  }

  void updatePosition(const String& newText, Point<int> position)
  {
    text = newText;
    BubbleComponent::setPosition(position);
    repaint();
  }

  void timerCallback() override
  {
    return;
  }

private:
  Component& owner;
  Font font;
  String text;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupDisplay)
};


#endif  // POPUPDISPLAY_H_INCLUDED
