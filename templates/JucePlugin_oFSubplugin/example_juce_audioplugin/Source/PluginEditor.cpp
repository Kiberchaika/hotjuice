/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Juceglvst_audioProcessorEditor::Juceglvst_audioProcessorEditor (Juceglvst_audioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
    
    hotJuiceComponent = new HotJuiceComponent();
    hotJuiceComponent->setSize(getWidth(), getHeight());
    hotJuiceComponent->processor = &p;

    addAndMakeVisible(hotJuiceComponent);
}

Juceglvst_audioProcessorEditor::~Juceglvst_audioProcessorEditor()
{
    deleteAllChildren();
}


//==============================================================================
void Juceglvst_audioProcessorEditor::paint (Graphics& g)
{
}

void Juceglvst_audioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
