#pragma once

#include "BasePlugin.h"

namespace hotjuice {

	enum PluginState {
		PluginStateNone = 0,
		PluginStateSetupRender,
		PluginStateCloseRender
	};

	class Plugin {
		BasePlugin* basePlugin;
		std::vector<Plugin*>* plugins;
		std::string pluginName;
		PluginState pluginState;

		friend class PluginManager;
		
		bool needToReloadData;

	public:
		Plugin(BasePlugin* basePlugin, std::vector<Plugin*>* plugins, std::string namePlugin);
		~Plugin();

        void addParameter(PluginParameter* parameter);

        BasePlugin* getPtrPlugin();
		const char* getPluginName();

		void setState(PluginState pluginState);

		PluginState getState();

        void setNeedToReloadData();
        bool doNeedToReloadData();
        
		void setReloaded();
		bool isReloaded();

		void setup(void* in = nullptr, void* out = nullptr);
		void setupRenderer(void* in = nullptr, void* out = nullptr);
        void prepareToStartRendering(bool shouldReloadData);
        void prepareToStopRendering();
        void closeRenderer(void* in = nullptr, void* out = nullptr);
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
