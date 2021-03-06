/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "HotJuiceComponent.h"

//==============================================================================
/**
*/
class Juceglvst_audioProcessorEditor  : public AudioProcessorEditor
{
public:
    Juceglvst_audioProcessorEditor (Juceglvst_audioProcessor&);
    ~Juceglvst_audioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Juceglvst_audioProcessor& processor;
    
    HotJuiceComponent* hotJuiceComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juceglvst_audioProcessorEditor)
};
