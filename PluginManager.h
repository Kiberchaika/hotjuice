#pragma once

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#endif

#include "Plugin.h"

namespace hotjuice {
	class PluginManager {
	private:
		typedef void*(*FuncCreatePlugin)(const char*);
		FuncCreatePlugin funcCreatePlugin;

		typedef std::function<void()> CallbackLoad;
		CallbackLoad callbackBeforeLoad;
		CallbackLoad callbackAfterLoad;

#if defined(_WIN32)
		HINSTANCE instanceLib;
#elif defined(__APPLE__)
		void* instanceLib;
#endif

		std::string nameTempDir;

		unsigned long long timeLastUpdated;
		unsigned long long timeUpdateInterval;

		unsigned long long tDest;

		bool useHotReloading;

		std::string pathToStoriedPluginCopies;
		std::string pathToLibrary;
		std::vector<std::string> additionalFilesToCopy;
		bool firstCheck;
		bool loaded;

		std::vector<Plugin*> plugins;

		bool tryToLoad(std::string pathLib);

		void setupInternal(std::string pathOriginalLibFolder, std::string pathOriginalLibName);

	public:
		PluginManager();
		~PluginManager();

		void setup(std::string pathOriginalLibFolder, std::string pathOriginalLibName);
		void setupWithHotReloading(std::string pathOriginalLibFolder, std::string pathOriginalLibName, std::string pathToStoriedPluginCopies = "plugins", unsigned long long timeUpdateInterval = 2000);
		void setAdditionalFilesToCopy(std::vector<std::string> additionalFilesToCopy);

		void addCallbackBeforeLoad(CallbackLoad callbackBeforeLoad);
		void addCallbackAfterLoad(CallbackLoad callbackAfterLoad);

		bool tryToLoadIfUpdated();
		Plugin* createPluginObject(const char* namePlugin);
	};
};
