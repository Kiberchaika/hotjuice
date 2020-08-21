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

#define PLUGIN_ADD_FUNCTION(className, funcName) hotjuice::PluginUtils::addCustomFunction(typeid(className), #funcName, (hotjuice::PluginBase::CustomFunc)&className::funcName)

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

#define PLUGIN_LOADER_WITH_FUNCTIONS(className, code) \
static int _loader_##className = ([]() { \
 \
PLUGIN_ADD_CONSTRUCTOR(className) \
code \
 \
}(), 0);

namespace hotjuice {
    class PluginParameter {
    public:
        // parameter base class
        std::type_index type_index = std::type_index(typeid(NULL));
        std::string name = "";
        
        PluginParameter(){};
    };

    template<typename TYPE>
    class PluginParameterOfType: public PluginParameter {
    public:
        std::function<TYPE()> getValue;

        std::function<void(TYPE)> setValue;
        
        PluginParameterOfType(std::string parameterName,
                              std::function<TYPE()> getter,
                              std::function<void(TYPE)> setter) {
            type_index = std::type_index(typeid(TYPE));
            name = parameterName;
            getValue = getter;
            setValue = setter;
        }
        
        #if defined(JUCE_APP_VERSION)

        juce::RangedAudioParameter* parameterPointer;
        

        PluginParameterOfType(juce::AudioParameterFloat* parameter) {
            name = parameter->name.toStdString();
            parameterPointer = parameter;
            type_index = std::type_index(typeid(float));
            getValue = [&]()->float{
                return ((juce::AudioParameterFloat*)parameterPointer)->get();
                
            };
            setValue = [&](float v){ ((juce::AudioParameterFloat*)parameterPointer)->setValueNotifyingHost(v);
            };
        }
        
        #endif

    };


	class PluginBase {
	private:
		bool reloaded;
		std::map<std::string, std::function<void(void*, void*)>> mapOfCallbacks;

	public:
		typedef void(PluginBase::*CustomFunc)(void*, void*);
        
        std::vector<PluginParameter*> pluginParameters;
        
        template<typename TYPE>
        PluginParameterOfType<TYPE>* getParameter(std::string name)
        {
            for (auto &parameter: pluginParameters) {
                if (parameter->name == name) return (hotjuice::PluginParameterOfType<TYPE>*)parameter;
            }
            return nullptr;
        }

		PluginBase();
		virtual ~PluginBase();

		virtual PluginBase* getPtrPlugin();

		virtual void setReloaded();
		virtual bool isReloaded();

		virtual void setup(void* in = nullptr, void* out = nullptr);
		virtual void setupRenderer(void* in = nullptr, void* out = nullptr);
		virtual void update(void* in = nullptr, void* out = nullptr);
		virtual void process(void* in = nullptr, void* out = nullptr);
		virtual void draw(void* in = nullptr, void* out = nullptr);
		virtual void cleanup(void* in = nullptr, void* out = nullptr);

		virtual void setWindowSize(int w, int h);
		virtual void setDesktopScale(float desktopScale);

		virtual void mouseDragged(int x, int y, int button);
		virtual void mouseMoved(int x, int y);
		virtual void mousePressed(int x, int y, int button);
		virtual void mouseReleased(int x, int y, int button);

		virtual void keyPressed(int key);
		virtual void keyReleased(int key);

		virtual void custom(char* name = nullptr, void* in = nullptr, void* out = nullptr);
		virtual void addCallback(char* name, std::function<void(void*, void*)> callback);
		virtual void callback(char* name = nullptr, void* in = nullptr, void* out = nullptr);
		virtual void clone(PluginBase* obj);
	};

	namespace PluginUtils {
		void addConstrutor(std::string name, std::function<void*()> function);
		void addCustomFunction(const std::type_info& type, char* name, PluginBase::CustomFunc func);
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
