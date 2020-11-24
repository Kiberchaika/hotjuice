#include "Plugin.h"

hotjuice::Plugin::Plugin(BasePlugin * basePlugin, std::vector<Plugin*>* plugins, std::string pluginName) {
	this->basePlugin = basePlugin;
	this->plugins = plugins;
	this->pluginName = pluginName;

	neededToSetupRender = false;
    neededToPrepareRender = true;
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

	if (basePlugin) {
		delete basePlugin;
		basePlugin = nullptr;
	}
}

void hotjuice::Plugin::addParameter(hotjuice::PluginParameter* parameter) {
	if (basePlugin) basePlugin->pluginParameters.push_back(parameter);
}

hotjuice::BasePlugin* hotjuice::Plugin::getPtrPlugin() {
	return basePlugin;
}

const char* hotjuice::Plugin::getPluginName() {
	return pluginName.c_str();
}

void hotjuice::Plugin::setNeededToSetupRender() {
	neededToSetupRender = true;
}

bool hotjuice::Plugin::isNeededToSetupRender() {
    return neededToSetupRender;
}

bool hotjuice::Plugin::isNeededToPrepareRender() {
    return neededToPrepareRender;
}

void hotjuice::Plugin::setReloaded(){
	if (basePlugin) basePlugin->setReloaded();
}

bool hotjuice::Plugin::isReloaded() {
	if (basePlugin) return basePlugin->isReloaded();
	return false;
}

void hotjuice::Plugin::setup(void* in, void* out) {
	if (basePlugin) basePlugin->setup(in, out);
}

void hotjuice::Plugin::setupRenderer(void* in, void* out) {
	if (basePlugin) {
		basePlugin->setupRenderer(in, out);
		neededToSetupRender = false;
	}
}

void hotjuice::Plugin::prepareRenderer(void* in, void* out) {
    if (basePlugin) {
        basePlugin->prepareRenderer(in, out);
        neededToPrepareRender = false;
    }
}

void hotjuice::Plugin::updateRenderer(void* in, void* out) {
    if (basePlugin) basePlugin->updateRenderer(in, out);
}

void hotjuice::Plugin::cleanupRenderer(void* in, void* out) {
    if (basePlugin) basePlugin->cleanupRenderer(in, out);
}

void hotjuice::Plugin::update(void* in, void* out) {
	if (basePlugin) basePlugin->update(in, out);
}

void hotjuice::Plugin::process(void* in, void* out) {
	if (basePlugin) basePlugin->process(in, out);
}

void hotjuice::Plugin::draw(void* in, void* out) {
	if (basePlugin) basePlugin->draw(in, out);
}

void hotjuice::Plugin::cleanup(void* in, void* out) {
	if (basePlugin) basePlugin->cleanup(in, out);
}

void hotjuice::Plugin::setWindowSize(int w, int h) {
	if (basePlugin) basePlugin->setWindowSize(w, h);
}

void hotjuice::Plugin::setDesktopScale(float desktopScale) {
	if (basePlugin) basePlugin->setDesktopScale(desktopScale);
}

void hotjuice::Plugin::setClipboardCallbacks(std::function<std::string(void)> getCallback, std::function<void(std::string)> setCallback) {
	if (basePlugin) basePlugin->setClipboardCallbacks(getCallback, setCallback);
}

void hotjuice::Plugin::mouseDragged(int x, int y, int button) {
	if (basePlugin) basePlugin->mouseDragged(x, y, button);
}

void hotjuice::Plugin::mouseMoved(int x, int y) {
	if (basePlugin) basePlugin->mouseMoved(x, y);
}

void hotjuice::Plugin::mousePressed(int x, int y, int button) {
	if (basePlugin) basePlugin->mousePressed(x, y, button);
}

void hotjuice::Plugin::mouseReleased(int x, int y, int button) {
	if (basePlugin) basePlugin->mouseReleased(x, y, button);
}

bool hotjuice::Plugin::keyPressed(int key) {
    if (basePlugin) {
        return basePlugin->keyPressed(key);
    } else return false;
}

bool hotjuice::Plugin::keyReleased(int key) {
    if (basePlugin) {
        return basePlugin->keyReleased(key);
    } else return false;
}

void hotjuice::Plugin::custom(char* name, void* in, void* out) {
	if (basePlugin) basePlugin->custom(name, in, out);
}

void hotjuice::Plugin::addCallback(char* name, std::function<void(void*, void*)> callback) {
	if (basePlugin) basePlugin->addCallback(name, callback);
}

void hotjuice::Plugin::clone(BasePlugin* obj) {
	if (basePlugin) basePlugin->clone(obj);
}

void hotjuice::Plugin::setStateString(char* str) {
	if (basePlugin) basePlugin->setStateString(str);
}

char* hotjuice::Plugin::getStateString() {
	if (basePlugin) {
		return basePlugin->getStateString();
	}
	return nullptr;
}


