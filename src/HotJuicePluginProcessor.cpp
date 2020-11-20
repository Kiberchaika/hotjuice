#if defined(JUCE_APP_VERSION)

#include "HotJuicePluginProcessor.h"

#include <chrono>
#include <thread>

HotJuicePluginProcessor::HotJuicePluginProcessor()
{
    pluginManager = nullptr;

	isReloading = false;

	position = 0;
	sampleRate = 0;
	bufferSize = 0;

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

void HotJuicePluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	this->sampleRate = sampleRate;
	this->bufferSize = samplesPerBlock;
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

void HotJuicePluginProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages, int totalNumInputChannels, int totalNumOutputChannels, double sampleRate)
{
	if (audioData.size() != totalNumInputChannels) {
		for (int i = 0; i < totalNumInputChannels; i++) {
			audioData.push_back(std::vector<float>(bufferSize, 0));
		}
	}

	int inputPosition = 0;
	int inputBufferSize = buffer.getNumSamples();

	while (inputPosition < inputBufferSize) {
		int cnt = (std::min)(bufferSize - position, inputBufferSize - inputPosition);
		for (int channel = 0; channel < totalNumInputChannels; ++channel) {
			memcpy(audioData[channel].data() + sizeof(float) * position, buffer.getWritePointer(channel) + sizeof(float) * inputPosition, sizeof(float) * cnt);
		}
		inputPosition += cnt;
		position += cnt;

		if (position == bufferSize) {
			std::tuple<std::vector<float*>, int, double> audioDataPackage;
			for (int channel = 0; channel < totalNumInputChannels; ++channel) {
				std::get<0>(audioDataPackage).push_back(audioData[channel].data());
			}
			std::get<1>(audioDataPackage) = bufferSize;
			std::get<2>(audioDataPackage) = sampleRate;

			if (!isReloading) {
				for (int i = 0; i < plugins.size(); i++) {
					if (plugins[i]) {
						plugins[i]->process(&audioDataPackage, nullptr);
					}
				}
			}

			position = 0;
		}
	}

}

#endif
