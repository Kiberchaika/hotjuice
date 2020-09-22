#pragma once

#if defined(JUCE_APP_VERSION)

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginManager.h"
#include "Plugin.h"

class HotJuicePluginProcessor 
{
public:
	hotjuice::PluginManager* pluginManager;
	hotjuice::Plugin* plugin;

	bool needToReinitRender;
	bool isReloading;

    HotJuicePluginProcessor();
    virtual ~HotJuicePluginProcessor();

	void setup(std::string pluginObjectName, std::string pluginFilename, std::string pluginEnclosingFolder, std::vector<std::string> additionalFilesToCopy);
    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages, int totalNumInputChannels, int totalNumOutputChannels, int sampleRate);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HotJuicePluginProcessor)
};

#endif
