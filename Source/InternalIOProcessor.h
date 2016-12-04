/*
  ==============================================================================

    InternalIOProcessor.h
    Created: 1 Nov 2016 12:32:38pm
    Author:  Jonathan

  ==============================================================================
*/

#ifndef INTERNALIOPROCESSOR_H_INCLUDED
#define INTERNALIOPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class InternalIOProcessor : public AudioProcessorGraph::AudioGraphIOProcessor
{
public:

  InternalIOProcessor(IODeviceType type);
  ~InternalIOProcessor();

  IODeviceType IOType;


  AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;
  const String getName() const override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InternalIOProcessor)
};



#endif  // INTERNALIOPROCESSOR_H_INCLUDED
