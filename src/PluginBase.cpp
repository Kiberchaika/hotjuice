#include "PluginBase.h"

std::map<std::string, std::function<void*()>>& getMapOfConstrutors()
{
	static std::map<std::string, std::function<void*()>> mapOfConstrutors;
	return mapOfConstrutors;
}

std::map<std::pair<std::type_index, std::string>, hotjuice::PluginBase::CustomFunc>& getMapOfCustomFunctions() {
	static std::map<std::pair<std::type_index, std::string>, hotjuice::PluginBase::CustomFunc> mapOfCustomFunctions;
	return mapOfCustomFunctions;
}

void hotjuice::PluginUtils::addConstrutor(std::string name, std::function<void*()> function)
{
	getMapOfConstrutors()[name] = function;
}

void hotjuice::PluginUtils::addCustomFunction(const std::type_info& type, char* name, hotjuice::PluginBase::CustomFunc func) {
	getMapOfCustomFunctions()[std::make_pair<std::type_index, std::string>(type, name)] = func;
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

void hotjuice::PluginBase::update(void * in, void * out) {
}

void hotjuice::PluginBase::process(void * in, void * out)
{
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

void hotjuice::PluginBase::custom(char * name, void * in, void * out) {
	if (name != nullptr && getMapOfCustomFunctions().find(std::make_pair<std::type_index, std::string>(typeid(*this), name)) != getMapOfCustomFunctions().end()) {
		(this->*getMapOfCustomFunctions()[std::make_pair<std::type_index, std::string>(typeid(*this), name)])(in, out);
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
#if defined(_WIN32)
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
#elif defined(__APPLE__)
__attribute__((constructor)) void DllMain()
{
}
#endif

