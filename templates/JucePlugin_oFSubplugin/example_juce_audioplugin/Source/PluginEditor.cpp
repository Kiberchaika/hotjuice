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
    
    mainComponent = new MainComponent();
    mainComponent->setSize(getWidth(), getHeight());
    mainComponent->processor = &p;

    addAndMakeVisible(mainComponent);
    
   
}

Juceglvst_audioProcessorEditor::~Juceglvst_audioProcessorEditor()
{
    deleteAllChildren();
}


//==============================================================================
void Juceglvst_audioProcessorEditor::paint (Graphics& g)
{
    
    
    
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
   // g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

   // g.setColour (Colours::white);
  //  g.setFont (15.0f);
   // g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void Juceglvst_audioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
