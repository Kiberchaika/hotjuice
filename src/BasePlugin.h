#pragma once

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#endif

#include <typeindex>
#include <typeinfo>

#include <functional>
#include <string>
#include <vector>
#include <chrono>
#include <map>

#if defined(JUCE_APP_VERSION)
#include "../JuceLibraryCode/JuceHeader.h"
#else
#endif

#define PLUGIN_ADD_FUNCTION(className, funcName) addCustomFunction(#funcName, (hotjuice::BasePlugin::CustomFunc)&className::funcName)

#define PLUGIN_ADD_CONSTRUCTOR(className) \
	hotjuice::PluginUtils::addConstrutor(#className, []() { \
		return new className(); \
	});

#define PLUGIN_LOADER(className) \
static int _loader_##className = ([]() { \
 \
PLUGIN_ADD_CONSTRUCTOR(className) \
 \
}(), 0);

namespace hotjuice {
    struct PluginParameter {
        // parameter base class
        std::type_index type_index = std::type_index(typeid(NULL));
        std::string name = "";
    };

    template<typename TYPE>
    class PluginParameterOfType: public PluginParameter {
    public:
        std::function<TYPE()> getValue;
        std::function<void(TYPE)> setValue;
        
		PluginParameterOfType(std::string parameterName, std::function<TYPE()> getter, std::function<void(TYPE)> setter) {
			type_index = std::type_index(typeid(TYPE));
			name = parameterName;
			getValue = getter;
			setValue = setter;
		}

        #if defined(JUCE_APP_VERSION)
        juce::RangedAudioParameter* parameterPointer;
		PluginParameterOfType(juce::AudioParameterFloat * parameter) {
			name = parameter->name.toStdString();
			parameterPointer = parameter;
			type_index = std::type_index(typeid(float));
			getValue = [&]()->float {
				return ((juce::AudioParameterFloat*)parameterPointer)->get();

			};
			setValue = [&](float v) {
				float normalizedValue = ((juce::AudioParameterFloat*)parameterPointer)->convertTo0to1(v);
				((juce::AudioParameterFloat*)parameterPointer)->setValueNotifyingHost(normalizedValue);
			};
		}
        #endif
    };


	class BasePlugin {
	public:
		typedef void(BasePlugin::*CustomFunc)(void*, void*);

	private:

		bool reloaded;
		std::map<std::string, std::function<void(void*, void*)>> mapOfCallbacks;
		std::map<std::string, hotjuice::BasePlugin::CustomFunc> mapOfCustomFunctions;

	public:
        
        std::vector<PluginParameter*> pluginParameters;
        
        template<typename TYPE>
        PluginParameterOfType<TYPE>* getParameter(std::string name)
        {
            for (auto &parameter: pluginParameters) {
                if (parameter->name == name) return (hotjuice::PluginParameterOfType<TYPE>*)parameter;
            }
            return nullptr;
        }

		BasePlugin();
		virtual ~BasePlugin();

		virtual BasePlugin* getPtrPlugin();

		virtual void setReloaded();
		virtual bool isReloaded();

		virtual void setup(void* in = nullptr, void* out = nullptr);
        virtual void setupRenderer(void* in = nullptr, void* out = nullptr);
        virtual void prepareToStartRendering(bool shouldReloadData);
        virtual void prepareToStopRendering();
        virtual void closeRenderer(void* in = nullptr, void* out = nullptr);
       
		virtual void update(void* in = nullptr, void* out = nullptr);
		virtual void process(void* in = nullptr, void* out = nullptr);
		virtual void draw(void* in = nullptr, void* out = nullptr);
		virtual void cleanup(void* in = nullptr, void* out = nullptr);

		virtual void setWindowSize(int w, int h);
		virtual void setDesktopScale(float desktopScale);
		virtual void setClipboardCallbacks(std::function<std::string(void)> getCallback, std::function<void(std::string)> setCallback);

		virtual void mouseDragged(int x, int y, int button);
		virtual void mouseMoved(int x, int y);
		virtual void mousePressed(int x, int y, int button);
		virtual void mouseReleased(int x, int y, int button);

		virtual bool keyPressed(int key);
		virtual bool keyReleased(int key);

		virtual void clone(BasePlugin* obj);

		virtual void setStateString(char* str);
		virtual char* getStateString();

		void custom(char* name = nullptr, void* in = nullptr, void* out = nullptr);
		void callback(char* name = nullptr, void* in = nullptr, void* out = nullptr);

		void addCallback(char* name, std::function<void(void*, void*)> callback);
		void addCustomFunction(std::string name, BasePlugin::CustomFunc func);
	};

	namespace PluginUtils {
		void addConstrutor(std::string name, std::function<void*()> function);
	};
	
	};

#if defined(_WIN32)
#define HOTJUICE_API __declspec(dllexport)
#else
#define HOTJUICE_API
#endif

extern "C" {
	//HOTJUICE_API void initGL(void* ptr);
	//HOTJUICE_API void* getGL();
	//HOTJUICE_API void deinitGL();
	HOTJUICE_API void* createPlugin(const char* name);
}
