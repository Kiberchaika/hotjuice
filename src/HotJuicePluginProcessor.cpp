#if defined(JUCE_APP_VERSION)

#include "HotJuicePluginProcessor.h"

#include <chrono>
#include <thread>

HotJuicePluginProcessor::HotJuicePluginProcessor()
{
}
    
void HotJuicePluginProcessor::setup(std::string pluginObjectName,  std::string pluginFilename, std::string pluginEnclosingFolder, std::vector<std::string> additionalFilesToCopy)
{
	// Initialise GL objects for rendering here.
    pluginManager = new hotjuice::PluginManager();
	pluginManager->setAdditionalFilesToCopy(additionalFilesToCopy);
	pluginManager->setupWithHotReloading(pluginEnclosingFolder, pluginFilename, pluginEnclosingFolder + "/tempPlugins");
    
	isReloading = false;

    //plugin = nullptr;

	pluginManager->addCallbackBeforeLoad(
		[&]() -> void {
		std::cout << "callback" << std::endl;

		isReloading = true;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	);

	pluginManager->addCallbackAfterLoad(
		[&]() -> void {
		std::cout << "callback" << std::endl;

		plugin->setup();

		isReloading = false;

		needToReinitRender = true;
	}
	);

	plugin = pluginManager->createPluginObject(pluginObjectName.c_str());
	if (plugin) {
		plugin->setup();
	}
}

HotJuicePluginProcessor::~HotJuicePluginProcessor()
{
    if (plugin) {
        delete plugin;
        plugin = nullptr;
    }
    
    if (pluginManager) {
        delete pluginManager;
        pluginManager = nullptr;
    }
}

void HotJuicePluginProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages, int totalNumInputChannels, int totalNumOutputChannels, int sampleRate)
{
	std::tuple<std::vector<float*>, int, double> data;
	for (int channel = 0; channel < totalNumInputChannels; ++channel) {
		std::get<0>(data).push_back(buffer.getWritePointer(channel));
		std::get<1>(data) = buffer.getNumSamples();
		std::get<2>(data) = sampleRate;
	}

	if (!isReloading && plugin) {
		plugin->process(&data, nullptr);
	}
}

#endif