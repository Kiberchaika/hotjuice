#include "PluginManager.h"
#include "Utils.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <dlfcn.h>
#endif

#include <iostream>
#include <ctime>


hotjuice::PluginManager::PluginManager() {
	nameTempDir = "";
	callbackBeforeLoad = nullptr;
	callbackAfterLoad = nullptr;
	instanceLib = NULL;

	funcCreatePlugin = NULL;

	tDest = 0;

	useHotReloading = false;

	loaded = false;
	firstCheck = true;
}

hotjuice::PluginManager::~PluginManager() {
#ifdef _WIN32
	if (instanceLib) {
		FreeLibrary(instanceLib);
	}
#elif __APPLE__
	if (instanceLib) {
		dlclose(instanceLib);
	}
#endif

	Utils::clearSubdirectories(pathToStoriedPluginCopies); 
}

void hotjuice::PluginManager::setupInternal(std::string pathOriginalLibFolder, std::string pathOriginalLibName) {
#if defined (_WIN32) && !defined (_DEBUG)  
	std::string pathOriginalLibFilename = pathOriginalLibName + ".dll";
#elif defined (_WIN32) && defined (_DEBUG)  
	std::string pathOriginalLibFilename = pathOriginalLibName + "_debug.dll";
#elif defined (__APPLE__)
	std::string pathOriginalLibFilename = "lib" + pathOriginalLibName + ".dylib";
#endif

	this->pathToLibrary = Utils::getFileAbsolutePath(Utils::joinFilePath(pathOriginalLibFolder, pathOriginalLibFilename));
}

void hotjuice::PluginManager::setup(std::string pathOriginalLibFolder, std::string pathOriginalLibName) {
	setupInternal(pathOriginalLibFolder, pathOriginalLibName);

	this->useHotReloading = false;

	tryToLoadIfUpdated();
}

void hotjuice::PluginManager::setupWithHotReloading(std::string pathOriginalLibFolder, std::string pathOriginalLibName, std::string pathToStoriedPluginCopies, unsigned long long timeUpdateInterval) {
	setupInternal(pathOriginalLibFolder, pathOriginalLibName);

	this->pathToStoriedPluginCopies = pathToStoriedPluginCopies;
	this->timeUpdateInterval = timeUpdateInterval;
	this->useHotReloading = true;

	Utils::clearSubdirectories(pathToStoriedPluginCopies);

	tryToLoadIfUpdated(); 
}

void hotjuice::PluginManager::setAdditionalFilesToCopy(std::vector<std::string> additionalFilesToCopy) {
	this->additionalFilesToCopy = additionalFilesToCopy;
}

void hotjuice::PluginManager::addCallbackBeforeLoad(CallbackLoad callbackBeforeLoad) {
	this->callbackBeforeLoad = callbackBeforeLoad;
}

void hotjuice::PluginManager::addCallbackAfterLoad(CallbackLoad callbackAfterLoad) {
	this->callbackAfterLoad = callbackAfterLoad;
}

bool hotjuice::PluginManager::tryToLoad(std::string pathLib) {

#ifdef _WIN32
	SetDllDirectoryA(Utils::getEnclosingDirectory(pathLib).c_str()); // fix for Windows
	instanceLib = LoadLibraryA(pathLib.c_str());
#elif __APPLE__
	// change id of dylib to temp id
    if(useHotReloading) {
        Utils::Execute("install_name_tool -id /" + Utils::getFileBaseName(pathToLibrary) + (nameTempDir != "" ? "." + nameTempDir : "") + "." + Utils::getFileExtension(pathLib) + " \"" + pathLib + "\"");
    }
    instanceLib = dlopen(pathLib.c_str(), RTLD_LOCAL);
#endif


	if (!instanceLib) {
#ifdef _WIN32
		std::cout << "could not load the dynamic library: " << pathLib << std::endl;
		unsigned long error = GetLastError();
		std::cout << "error: " << error << std::endl;
#elif __APPLE__
		std::cout << "could not load the dynamic library: " << pathLib << std::endl;
        char* error = dlerror();
        if(error) {
            std::cout << "error: " << error << " " << std::endl;
        }
#endif

		funcCreatePlugin = NULL;

		loaded = false;
	}
	else {
		// resolve function address here
#ifdef _WIN32
		funcCreatePlugin = (FuncCreatePlugin)GetProcAddress(instanceLib, "createPlugin");
#elif __APPLE__
		funcCreatePlugin = (FuncCreatePlugin)dlsym(instanceLib, "createPlugin");
#endif

		loaded = true;

		// callback before
		if (callbackBeforeLoad) (callbackBeforeLoad)();

		// hotswap
		for (size_t i = 0; i < plugins.size(); i++) {
			BasePlugin* ptr = (BasePlugin*)(funcCreatePlugin)((char*)plugins[i]->pluginName.c_str());
			ptr->setReloaded();
			ptr->clone(plugins[i]->basePlugin);
            
            ptr->pluginParameters = plugins[i]->basePlugin->pluginParameters;

			delete plugins[i]->basePlugin;
			plugins[i]->basePlugin = ptr;
		} 

		// callback after
		if (callbackAfterLoad) (callbackAfterLoad)();
	}

	return loaded;
}

bool hotjuice::PluginManager::tryToLoadIfUpdated() {
	bool reloaded = false;

	if (Utils::checkFileExits(pathToLibrary) && (firstCheck || Utils::getElapsedTimeMillis() - timeLastUpdated > timeUpdateInterval)) {
		firstCheck = false;
		if (!loaded && !useHotReloading) {
			reloaded = tryToLoad(pathToLibrary);
		}
		else if(useHotReloading) {
			unsigned long long tSrc = Utils::getFileModificationTime(pathToLibrary);
			if (tSrc > tDest) {
				
				// check for open for read
				FILE *fp = fopen(pathToLibrary.c_str(), "rb");
				if (fp != NULL) {
					fclose(fp);

					std::cout << "reload" << std::endl;
 
					tDest = tSrc;

					nameTempDir = std::to_string(tSrc) + "_" + std::to_string(rand() % 100000); // random folder

					std::string tempDir = Utils::joinFilePath(pathToStoriedPluginCopies, nameTempDir);
					Utils::createDirectory(tempDir);
					std::string pathLib = Utils::joinFilePath(tempDir, Utils::getFileName(pathToLibrary));


#ifdef _WIN32
					HINSTANCE instanceLibPrevious = instanceLib;
#elif __APPLE__
					void* instanceLibPrevious = instanceLib;
#endif

					if (Utils::copyFileFromTo(pathToLibrary, pathLib)) {
#ifdef _WIN32
						Utils::copyFileFromTo(
							Utils::joinFilePath(Utils::getEnclosingDirectory(pathToLibrary), Utils::getFileBaseName(pathToLibrary) + ".pdb"), 
							Utils::joinFilePath(tempDir, Utils::getFileBaseName(pathToLibrary) + ".pdb")
						);
#endif

						for(int i = 0; i < additionalFilesToCopy.size(); i++) {
							Utils::copyFileFromTo(
								Utils::joinFilePath(Utils::getEnclosingDirectory(pathToLibrary), additionalFilesToCopy[i]),
								Utils::joinFilePath(Utils::getEnclosingDirectory(pathLib), additionalFilesToCopy[i])
							);
						}

						reloaded = tryToLoad(pathLib);
                    }
                    
#if defined(JUCE_APP_VERSION)
                    // disable for share GL context for JUCE
#else
                    if (loaded) {
#ifdef _WIN32
                        if (instanceLibPrevious) {
                            FreeLibrary(instanceLibPrevious);
                        }
#elif __APPLE__
                        if (instanceLibPrevious) {
                            dlclose(instanceLibPrevious);
                        }
#endif
                    }
#endif
				}
			}
		}

		timeLastUpdated = Utils::getElapsedTimeMillis();
	}

	return reloaded;
}

hotjuice::Plugin* hotjuice::PluginManager::createPluginObject(const char * pluginName) {
	if (!loaded || funcCreatePlugin == NULL) return nullptr;

	BasePlugin* basePlugin = (BasePlugin*)(funcCreatePlugin)(pluginName);
	if (basePlugin == NULL) {
		std::cout << "could not find the plugin " << pluginName << std::endl;
		return nullptr;
	}

	Plugin* plugin = new Plugin(basePlugin, &plugins, pluginName);
	plugins.push_back(plugin);

	return plugin;
}
