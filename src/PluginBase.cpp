#include "PluginBase.h"

std::map<std::string, std::function<void*()>>& getMapOfConstrutors()
{
	static std::map<std::string, std::function<void*()>> mapOfConstrutors;
	return mapOfConstrutors;
}

void hotjuice::PluginUtils::addConstrutor(std::string name, std::function<void*()> function)
{
	getMapOfConstrutors()[name] = function;
}

hotjuice::PluginBase::PluginBase() {
	reloaded = false;
}

hotjuice::PluginBase::~PluginBase() {
}

hotjuice::PluginBase* hotjuice::PluginBase::getPtrPlugin() {
	return this;
}

void hotjuice::PluginBase::setReloaded() {
	reloaded = true;
}

bool hotjuice::PluginBase::isReloaded() {
	return reloaded;
}

void hotjuice::PluginBase::setup(void * in, void * out) {
}

void hotjuice::PluginBase::setupRenderer(void * in, void * out) {
}

void hotjuice::PluginBase::update(void * in, void * out) {
}

void hotjuice::PluginBase::process(void * in, void * out) {
}

void hotjuice::PluginBase::draw(void * in, void * out) {
}

void hotjuice::PluginBase::cleanup(void * in, void * out) {
}

void hotjuice::PluginBase::setWindowSize(int w, int h) {
}

void hotjuice::PluginBase::setDesktopScale(float desktopScale) {
}

void hotjuice::PluginBase::mouseDragged(int x, int y, int button) {
}

void hotjuice::PluginBase::mouseMoved(int x, int y) {
}

void hotjuice::PluginBase::mousePressed(int x, int y, int button) {
}

void hotjuice::PluginBase::mouseReleased(int x, int y, int button) {
}

void hotjuice::PluginBase::keyPressed(int key) {
}

void hotjuice::PluginBase::keyReleased(int key) {
}

void hotjuice::PluginBase::addCustomFunction(std::string name, hotjuice::PluginBase::CustomFunc func) {
	mapOfCustomFunctions[name] = func;
}

void hotjuice::PluginBase::addCallback(char* name, std::function<void(void*, void*)> callback) {
	mapOfCallbacks[name] = callback;
}

void hotjuice::PluginBase::custom(char * name, void * in, void * out) {
	if (name != nullptr && mapOfCustomFunctions.find(name) != mapOfCustomFunctions.end()) {
		(this->*mapOfCustomFunctions[name])(in, out);
	}
}

void hotjuice::PluginBase::callback(char * name, void * in, void * out) {
	if (name != nullptr && mapOfCallbacks.find(name) != mapOfCallbacks.end()) {
		mapOfCallbacks[name](in, out);
	}
}

void hotjuice::PluginBase::clone(PluginBase * obj) {
}

void* createPlugin(const char* name)
{ 
	hotjuice::PluginBase* ptr = nullptr;

	if (getMapOfConstrutors().find(name) != getMapOfConstrutors().end()) {
		ptr = (hotjuice::PluginBase*)getMapOfConstrutors()[name]();
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

