#pragma once

#if defined(JUCE_APP_VERSION)

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginManager.h"
#include "Plugin.h"

class HotJuicePluginProcessor 
{
public:
	hotjuice::PluginManager* pluginManager;
	std::vector<hotjuice::Plugin*> plugins;
	
	std::function<void(void)> callbackBeforeLoad;
	std::function<void(void)> callbackAfterLoad;

	bool isReloading;

	int position;
	double sampleRate;
	int bufferSize;
	std::vector<std::vector<float>> audioData;

    HotJuicePluginProcessor();
    virtual ~HotJuicePluginProcessor();

	void setup(std::vector<std::string> pluginObjectNames, std::string pluginFilename, std::string pluginEnclosingFolder, std::vector<std::string> additionalFilesToCopy);
	void prepareToPlay(double sampleRate, int samplesPerBlock);
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages, int totalNumInputChannels, int totalNumOutputChannels, double sampleRate);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HotJuicePluginProcessor)
};

#endif
