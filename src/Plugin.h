#pragma once

#include "PluginBase.h"

namespace hotjuice {

	class Plugin {
		PluginBase* pluginBase;
		std::vector<Plugin*>* plugins;
		std::string namePlugin;

		friend class PluginManager;
        
        
	public:
		Plugin(PluginBase* pluginBase, std::vector<Plugin*>* plugins, std::string namePlugin);
		~Plugin();

        void addParameter(PluginParameter* parameter);

        PluginBase* getPtrPlugin();
		const char* getNamePlugin();

		void setReloaded();
		bool isReloaded();

		void setup(void* in = nullptr, void* out = nullptr);
		void setupRenderer(void* in = nullptr, void* out = nullptr);
		void update(void* in = nullptr, void* out = nullptr);
		void process(void* in = nullptr, void* out = nullptr);
		void draw(void* in = nullptr, void* out = nullptr);
		void cleanup(void* in = nullptr, void* out = nullptr);

		void setWindowSize(int w, int h);
		void setDesktopScale(float desktopScale);

		void mouseDragged(int x, int y, int button);
		void mouseMoved(int x, int y);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		void keyPressed(int key);
		void keyReleased(int key);

		void custom(char* name, void* in = nullptr, void* out = nullptr);
		void addCallback(char*, std::function<void(void*, void*)> callback);

		void clone(PluginBase* obj);
	};
};
