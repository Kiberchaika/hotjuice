#pragma once

#include "BasePlugin.h"

namespace hotjuice {

	class Plugin {
		BasePlugin* basePlugin;
		std::vector<Plugin*>* plugins;
		std::string pluginName;

		friend class PluginManager;
		
        bool neededToSetupRender;
        bool neededToPrepareRender;
        
	public:
		Plugin(BasePlugin* basePlugin, std::vector<Plugin*>* plugins, std::string namePlugin);
		~Plugin();

        void addParameter(PluginParameter* parameter);

        BasePlugin* getPtrPlugin();
		const char* getPluginName();

		void setNeededToSetupRender();
		bool isNeededToSetupRender();

        bool isNeededToPrepareRender();

		void setReloaded();
		bool isReloaded();

		void setup(void* in = nullptr, void* out = nullptr);
		void setupRenderer(void* in = nullptr, void* out = nullptr);
        void prepareRenderer(void* in = nullptr, void* out = nullptr);
        void updateRenderer(void* in = nullptr, void* out = nullptr);
        void cleanupRenderer(void* in = nullptr, void* out = nullptr);
		void update(void* in = nullptr, void* out = nullptr);
		void process(void* in = nullptr, void* out = nullptr);
		void draw(void* in = nullptr, void* out = nullptr);
		void cleanup(void* in = nullptr, void* out = nullptr);

		void setWindowSize(int w, int h);
		void setDesktopScale(float desktopScale);
		void setClipboardCallbacks(std::function<std::string(void)> getCallback, std::function<void(std::string)> setCallback);

		void mouseDragged(int x, int y, int button);
		void mouseMoved(int x, int y);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		bool keyPressed(int key);
		bool keyReleased(int key);

		void custom(char* name, void* in = nullptr, void* out = nullptr);
		void addCallback(char*, std::function<void(void*, void*)> callback);

		void clone(BasePlugin* obj);

		void setStateString(char* str);
		char* getStateString();
	};
};
