#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	soundStream.printDeviceList();
	ofSoundStreamSettings settings;

#ifdef TARGET_WIN32
	auto deviceList = soundStream.getDeviceList(ofSoundDevice::Api::MS_DS);
	settings.setOutDevice(deviceList[1]);

#else
	auto deviceList = soundStream.getMatchingDevices("default");
	for (auto &dev : deviceList) {
		if (dev.outputChannels == 2) {
			settings.setOutDevice(dev);
		}
	}
#endif

	settings.setOutListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = 512;
	soundStream.setup(settings);



	// Initialise GL objects for rendering here.
	pluginManager = new hotjuice::PluginManager();

	// The library file that we load here should be called like this:
	std::string pluginFilename = "example_oF_subplugin"; // omit the .dylib or .dll and "lib" in the beginning

#if defined (_WIN32)
	std::string pluginPath = "MyCompany/Hotjuice oF and JUCE example";
	pluginManager->setAdditionalFilesToCopy({ "FreeImage.dll", "fmodex64.dll","fmodexL64.dll" });
#elif defined (__APPLE__)
	std::string pluginPath = "MyCompany/com.company.application";
	pluginManager->setAdditionalFilesToCopy({ "libfmodex.dylib" });
#endif
	std::string pluginEnclosingFolder = hotjuice::Utils::getAppDataDirectory() + pluginPath;
	pluginManager->setupWithHotReloading(pluginEnclosingFolder, pluginFilename, pluginEnclosingFolder + "/tempPlugins");

	isReloading = false;

	//plugin = nullptr;

	pluginManager->addCallbackBeforeLoad(
		[&]() -> void {
		std::cout << "callback" << std::endl;

		isReloading = true;

		plugin->setState(hotjuice::PluginStateCloseRender);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	);

	pluginManager->addCallbackAfterLoad(
		[&]() -> void {
		std::cout << "callback" << std::endl;

		plugin->setState(hotjuice::PluginStateSetupRender);
		plugin->setup();

		isReloading = false;
	}
	);

	plugin = pluginManager->createPluginObject("MyPlugin");
	if (plugin) {
		plugin->setState(hotjuice::PluginStateSetupRender);
		plugin->setup();
	}


}

//--------------------------------------------------------------
void ofApp::update(){
	pluginManager->tryToLoadIfUpdated();

	if (!isReloading && plugin) {
		float in[2] = { 0, 0 };
		float out = 0;

		plugin->update(&in, &out);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (plugin) {
		if (!isReloading) {
			if (plugin->getState() == hotjuice::PluginStateSetupRender) {
				plugin->setupRenderer();
				plugin->prepareToStartRendering(plugin->doNeedToReloadData());

				plugin->setNeedToReloadData();
				plugin->setState(hotjuice::PluginStateNone);
			}

			float desktopScale = 1.0;
			plugin->setDesktopScale(desktopScale);
			plugin->setWindowSize(ofGetWidth(), ofGetHeight());

			plugin->draw();
		}
		else if (plugin->getState() == hotjuice::PluginStateSetupRender) {
			plugin->prepareToStopRendering();
			plugin->closeRenderer();
			plugin->setState(hotjuice::PluginStateNone);
		}
	}
}

void ofApp::exit() {
	if (plugin) {
		delete plugin;
		plugin = nullptr;
	}

	if (pluginManager) {
		delete pluginManager;
		pluginManager = nullptr;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (plugin) {
		plugin->keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (plugin) {
		plugin->keyReleased(key);
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (plugin) {
		float desktopScale = 1.0;
		plugin->mouseMoved(x * desktopScale, y * desktopScale);
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (plugin) {
		float desktopScale = 1.0;
		plugin->mouseDragged(x * desktopScale, y * desktopScale, 0);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (plugin) {
		float desktopScale = 1.0;
		plugin->mousePressed(x * desktopScale, y * desktopScale, 0);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (plugin) {
		float desktopScale = 1.0;
		plugin->mouseReleased(x * desktopScale, y * desktopScale, 0);
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

void ofApp::audioOut(ofSoundBuffer & buffer){

	std::vector<float*>& audioBuffers = std::get<0>(audioData);

	if (audioBuffers.size() != buffer.getNumChannels()) {
		for (int i = 0; i < audioBuffers.size();i++) {
			delete audioBuffers[i];
		}
		audioBuffers.clear();

		for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
			audioBuffers.push_back(new float[buffer.getNumFrames()]);
		}
		for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
			for (int i = 0; i < buffer.getNumFrames(); i++) {
				audioBuffers[channel][i] = 0;
			}
		}
	}

	std::get<1>(audioData) = buffer.getNumChannels();
	std::get<2>(audioData) = buffer.getSampleRate();

	/*
	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		for (int i = 0; i < buffer.getNumFrames(); i++) {
			audioBuffers[channel][i] = buffer[i*buffer.getNumChannels() + channel];
		}
	}
	*/

	if (!isReloading && plugin) {
		plugin->process(&audioData, nullptr);
	}

	for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
		for (int i = 0; i < buffer.getNumFrames(); i++) {
			buffer[i*buffer.getNumChannels() + channel] = audioBuffers[channel][i];
		}
	}

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
