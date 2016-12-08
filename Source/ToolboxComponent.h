/*
  ==============================================================================

    ToolboxComponent.h
    Created: 9 Oct 2016 11:03:18am
    Author:  Jonathan

  ==============================================================================
*/

#ifndef TOOLBOXCOMPONENT_H_INCLUDED
#define TOOLBOXCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ToolboxComponent : public Component, public ListBoxModel
{
public:

  class ModulesListElement : public ReferenceCountedObject
  {
  public:
    ModulesListElement(String displayName, String className, AudioProcessor* (*createFunction)()) : name(displayName), className(className), getInstance(createFunction){}
    ModulesListElement(ModulesListElement& other) : name(other.name), className(other.className), getInstance(other.getInstance){}
    ~ModulesListElement(){}

    String name;
    String className;
    AudioProcessor* (*getInstance)();
  };

  ToolboxComponent();
  ~ToolboxComponent();

  void paint(Graphics&) override;
  void resized() override;

  int getNumRows() override;
  void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool isRowSeleected) override;
  var getDragSourceDescription(const SparseSet<int>& selectedrow) override;

  OwnedArray<ModulesListElement> modules;
private:
  ScopedPointer<ListBox> moduleList;
  ScopedPointer<ResizableBorderComponent> resizeBorder;

  template<typename T>
  static AudioProcessor * createInstance() { return new T; }


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToolboxComponent)
};


#endif  // TOOLBOXCOMPONENT_H_INCLUDED
