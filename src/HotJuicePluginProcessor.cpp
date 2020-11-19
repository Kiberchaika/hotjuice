#if defined(JUCE_APP_VERSION)

#include "HotJuicePluginProcessor.h"

#include <chrono>
#include <thread>

HotJuicePluginProcessor::HotJuicePluginProcessor()
{
    pluginManager = nullptr;

	isReloading = false;

	callbackBeforeLoad = nullptr;
	callbackAfterLoad = nullptr;
}
    
void HotJuicePluginProcessor::setup(std::vector<std::string> pluginObjectNames,  std::string pluginFilename, std::string pluginEnclosingFolder, std::vector<std::string> additionalFilesToCopy)
{
	// Initialise GL objects for rendering here.
    pluginManager = new hotjuice::PluginManager();
	pluginManager->setAdditionalFilesToCopy(additionalFilesToCopy);
	pluginManager->setupWithHotReloading(pluginEnclosingFolder, pluginFilename, pluginEnclosingFolder + "/tempPlugins");
    
	isReloading = false;

	plugins.resize(pluginObjectNames.size());

	pluginManager->addCallbackBeforeLoad(
		[&]() -> void {
		std::cout << "callback" << std::endl;

		if (callbackBeforeLoad) callbackBeforeLoad();

		isReloading = true;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	);

	pluginManager->addCallbackAfterLoad(
		[&]() -> void {
		std::cout << "callback" << std::endl;

		for (int i = 0; i < plugins.size(); i++) {
			plugins[i]->setNeededToSetupRender();
			plugins[i]->setup();
		}

		if (callbackAfterLoad) callbackAfterLoad();

        isReloading = false;
	}
	);

	for (int i = 0; i < plugins.size(); i++) {
		plugins[i] = pluginManager->createPluginObject(pluginObjectNames[i].c_str());
		if (plugins[i]) {
			plugins[i]->setNeededToSetupRender();
			plugins[i]->setup();
		}
	}
	
}

HotJuicePluginProcessor::~HotJuicePluginProcessor()
{
	for (int i = 0; i < plugins.size(); i++) {
		if (plugins[i] != nullptr) {
			delete plugins[i];
			plugins[i] = nullptr;
		}
	}

    if (pluginManager != nullptr) {
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

	if (!isReloading) {
		for (int i = 0; i < plugins.size(); i++) {
			if (plugins[i]) {
				plugins[i]->process(&data, nullptr);
			}
		}
	}
}

#endif
