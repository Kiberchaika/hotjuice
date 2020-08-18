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
	std::string pluginEnclosingFolder = hotjuice::Utils::getAppDataDirectory() + pluginPath;
	pluginManager->setAdditionalFilesToCopy({ "FreeImage.dll", "fmodex64.dll","fmodexL64.dll" });
	pluginManager->setupWithHotReloading(pluginEnclosingFolder, pluginFilename, pluginEnclosingFolder + "/tempPlugins");
#elif defined (__APPLE__)
	std::string pluginPath = "MyCompany/com.company.application";
	std::string pluginEnclosingFolder = hotjuice::Utils::getAppDataDirectory() + pluginPath;
	pluginManager->setAdditionalFilesToCopy({ "libfmodex.dylib" });
	pluginManager->setupWithHotReloading(pluginEnclosingFolder, pluginFilename, pluginEnclosingFolder + "/tempPlugins");
#endif

	isReloading = false;
	needToReinitRender = true;

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

	plugin = pluginManager->createPluginObject("MyPlugin");


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
	if (!isReloading && plugin) {
		if (needToReinitRender) {
			plugin->setupRenderer();
			needToReinitRender = false;
		}

		float desktopScale = 1.0;
		plugin->setDesktopScale(desktopScale);
		plugin->setWindowSize(ofGetWidth(), ofGetHeight());

		plugin->draw();
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
