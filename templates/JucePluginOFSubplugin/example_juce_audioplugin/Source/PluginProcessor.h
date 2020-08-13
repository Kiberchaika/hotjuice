/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
 
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginManager.h"
#include "Plugin.h"

//==============================================================================
/**
*/
class Juceglvst_audioProcessor  : public AudioProcessor, public Timer 
{
public:
	hotjuice::PluginManager* hotreloader;
	hotjuice::Plugin* plugin;

	bool needToReinitRender;
	bool isReloading;

public:
    //==============================================================================
    Juceglvst_audioProcessor();
    ~Juceglvst_audioProcessor();

	void timerCallback() override;

	//==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juceglvst_audioProcessor)
};
