#include "Plugin.h"

hotjuice::Plugin::Plugin(PluginBase * plugin, std::vector<Plugin*>* plugins, std::string namePlugin) {
	this->plugin = plugin;
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

	if (plugin) {
		delete plugin;
		plugin = nullptr;
	}
}

hotjuice::PluginBase* hotjuice::Plugin::getPtrPlugin() {
	return plugin;
}

const char* hotjuice::Plugin::getNamePlugin() {
	return namePlugin.c_str();
}

void hotjuice::Plugin::setReloaded(){
	if (plugin) plugin->setReloaded();
}

bool hotjuice::Plugin::isReloaded() {
	if (plugin) return plugin->isReloaded();
	return false;
}

void hotjuice::Plugin::setup(void* in, void* out) {
	if (plugin) plugin->setup(in, out);
}

void hotjuice::Plugin::setupRenderer(void* in, void* out) {
	if (plugin) plugin->setupRenderer(in, out);
}

void hotjuice::Plugin::update(void* in, void* out) {
	if (plugin) plugin->update(in, out);
}

void hotjuice::Plugin::process(void* in, void* out) {
	if (plugin) plugin->process(in, out);
}

void hotjuice::Plugin::draw(void* in, void* out) {
	if (plugin) plugin->draw(in, out);
}

void hotjuice::Plugin::cleanup(void* in, void* out) {
	if (plugin) plugin->cleanup(in, out);
}

void hotjuice::Plugin::setWindowSize(int w, int h) {
	if (plugin) plugin->setWindowSize(w, h);
}

void hotjuice::Plugin::setDesktopScale(float desktopScale) {
	if (plugin) plugin->setDesktopScale(desktopScale);
}

void hotjuice::Plugin::mouseDragged(int x, int y, int button) {
	if (plugin) plugin->mouseDragged(x, y, button);
}

void hotjuice::Plugin::mouseMoved(int x, int y) {
	if (plugin) plugin->mouseMoved(x, y);
}

void hotjuice::Plugin::mousePressed(int x, int y, int button) {
	if (plugin) plugin->mousePressed(x, y, button);
}

void hotjuice::Plugin::mouseReleased(int x, int y, int button) {
	if (plugin) plugin->mouseReleased(x, y, button);
}

void hotjuice::Plugin::keyPressed(int key) {
	if (plugin) plugin->keyPressed(key);
}

void hotjuice::Plugin::keyReleased(int key) {
	if (plugin) plugin->keyReleased(key);
}

void hotjuice::Plugin::custom(char* name, void* in, void* out) {
	if (plugin) plugin->custom(name, in, out);
}

void hotjuice::Plugin::clone(PluginBase* obj) {
	if (plugin) plugin->clone(obj);
}

