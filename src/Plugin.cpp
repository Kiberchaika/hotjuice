#include "Plugin.h"

hotjuice::Plugin::Plugin(PluginBase * pluginBase, std::vector<Plugin*>* plugins, std::string namePlugin) {
	this->pluginBase = pluginBase;
	this->plugins = plugins;
	this->namePlugin = namePlugin;
}

hotjuice::Plugin::~Plugin() {
	if (plugins) {
		for (size_t i = 0; i < plugins->size(); i++) {
			if (plugins->operator[](i) == this) {
				plugins->erase(plugins->begin() + i);
				break;
			}
		}
		plugins = nullptr;
	}

	if (pluginBase) {
		delete pluginBase;
		pluginBase = nullptr;
	}
}

void hotjuice::Plugin::addParameter(hotjuice::PluginParameter* parameter) {
    getPtrPlugin()->pluginParameters.push_back(parameter);
}

hotjuice::PluginBase* hotjuice::Plugin::getPtrPlugin() {
	return pluginBase;
}

const char* hotjuice::Plugin::getNamePlugin() {
	return namePlugin.c_str();
}

void hotjuice::Plugin::setReloaded(){
	if (pluginBase) pluginBase->setReloaded();
}

bool hotjuice::Plugin::isReloaded() {
	if (pluginBase) return pluginBase->isReloaded();
	return false;
}

void hotjuice::Plugin::setup(void* in, void* out) {
	if (pluginBase) pluginBase->setup(in, out);
}

void hotjuice::Plugin::setupRenderer(void* in, void* out) {
	if (pluginBase) pluginBase->setupRenderer(in, out);
}

void hotjuice::Plugin::update(void* in, void* out) {
	if (pluginBase) pluginBase->update(in, out);
}

void hotjuice::Plugin::process(void* in, void* out) {
	if (pluginBase) pluginBase->process(in, out);
}

void hotjuice::Plugin::draw(void* in, void* out) {
	if (pluginBase) pluginBase->draw(in, out);
}

void hotjuice::Plugin::cleanup(void* in, void* out) {
	if (pluginBase) pluginBase->cleanup(in, out);
}

void hotjuice::Plugin::setWindowSize(int w, int h) {
	if (pluginBase) pluginBase->setWindowSize(w, h);
}

void hotjuice::Plugin::setDesktopScale(float desktopScale) {
	if (pluginBase) pluginBase->setDesktopScale(desktopScale);
}

void hotjuice::Plugin::mouseDragged(int x, int y, int button) {
	if (pluginBase) pluginBase->mouseDragged(x, y, button);
}

void hotjuice::Plugin::mouseMoved(int x, int y) {
	if (pluginBase) pluginBase->mouseMoved(x, y);
}

void hotjuice::Plugin::mousePressed(int x, int y, int button) {
	if (pluginBase) pluginBase->mousePressed(x, y, button);
}

void hotjuice::Plugin::mouseReleased(int x, int y, int button) {
	if (pluginBase) pluginBase->mouseReleased(x, y, button);
}

void hotjuice::Plugin::keyPressed(int key) {
	if (pluginBase) pluginBase->keyPressed(key);
}

void hotjuice::Plugin::keyReleased(int key) {
	if (pluginBase) pluginBase->keyReleased(key);
}

void hotjuice::Plugin::custom(char* name, void* in, void* out) {
	if (pluginBase) pluginBase->custom(name, in, out);
}

void hotjuice::Plugin::addCallback(char* name, std::function<void(void*, void*)> callback) {
	if (pluginBase) pluginBase->addCallback(name, callback);
}

void hotjuice::Plugin::clone(PluginBase* obj) {
	if (pluginBase) pluginBase->clone(obj);
}

