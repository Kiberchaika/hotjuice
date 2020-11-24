#include "BasePlugin.h"

std::map<std::string, std::function<void*()>>& getMapOfConstrutors()
{
	static std::map<std::string, std::function<void*()>> mapOfConstrutors;
	return mapOfConstrutors;
}

void hotjuice::PluginUtils::addConstrutor(std::string name, std::function<void*()> function)
{
	getMapOfConstrutors()[name] = function;
}

hotjuice::BasePlugin::BasePlugin() {
	reloaded = false;
}

hotjuice::BasePlugin::~BasePlugin() {
}

hotjuice::BasePlugin* hotjuice::BasePlugin::getPtrPlugin() {
	return this;
}

void hotjuice::BasePlugin::setReloaded() {
	reloaded = true;
}

bool hotjuice::BasePlugin::isReloaded() {
	return reloaded;
}

void hotjuice::BasePlugin::setup(void * in, void * out) {
}

void hotjuice::BasePlugin::setupRenderer(void * in, void * out) {
}

void hotjuice::BasePlugin::prepareToStartRendering(bool reloadData) {
}

void hotjuice::BasePlugin::prepareToStopRendering() {
}

void hotjuice::BasePlugin::closeRenderer(void * in, void * out) {
}

void hotjuice::BasePlugin::update(void * in, void * out) {
}

void hotjuice::BasePlugin::process(void * in, void * out) {
}

void hotjuice::BasePlugin::draw(void * in, void * out) {
}

void hotjuice::BasePlugin::cleanup(void * in, void * out) {
}

void hotjuice::BasePlugin::setWindowSize(int w, int h) {
}

void hotjuice::BasePlugin::setDesktopScale(float desktopScale) {
}

void hotjuice::BasePlugin::setClipboardCallbacks(std::function<std::string(void)> getCallback, std::function<void(std::string)> setCallback) {
}

void hotjuice::BasePlugin::mouseDragged(int x, int y, int button) {
}

void hotjuice::BasePlugin::mouseMoved(int x, int y) {
}

void hotjuice::BasePlugin::mousePressed(int x, int y, int button) {
}

void hotjuice::BasePlugin::mouseReleased(int x, int y, int button) {
}

void hotjuice::BasePlugin::keyPressed(int key) {
}

void hotjuice::BasePlugin::keyReleased(int key) {
}

void hotjuice::BasePlugin::addCustomFunction(std::string name, hotjuice::BasePlugin::CustomFunc func) {
	mapOfCustomFunctions[name] = func;
}

void hotjuice::BasePlugin::addCallback(char* name, std::function<void(void*, void*)> callback) {
	mapOfCallbacks[name] = callback;
}

void hotjuice::BasePlugin::custom(char * name, void * in, void * out) {
	if (name != nullptr && mapOfCustomFunctions.find(name) != mapOfCustomFunctions.end()) {
		(this->*mapOfCustomFunctions[name])(in, out);
	}
}

void hotjuice::BasePlugin::callback(char * name, void * in, void * out) {
	if (name != nullptr && mapOfCallbacks.find(name) != mapOfCallbacks.end()) {
		mapOfCallbacks[name](in, out);
	}
}

void hotjuice::BasePlugin::clone(BasePlugin * obj) {
}

void hotjuice::BasePlugin::setStateString(char* str) {
}

char* hotjuice::BasePlugin::getStateString() {
	return nullptr;
}

void* createPlugin(const char* name)
{ 
	hotjuice::BasePlugin* ptr = nullptr;

	if (getMapOfConstrutors().find(name) != getMapOfConstrutors().end()) {
		ptr = (hotjuice::BasePlugin*)getMapOfConstrutors()[name]();
	}

	return ptr;
}

// lib entry point
#if defined(_WIN32) && !defined(JUCE_APP_VERSION)
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#elif defined(__APPLE__) && !defined(JUCE_APP_VERSION)
__attribute__((constructor)) void DllMain()
{
}
#endif

