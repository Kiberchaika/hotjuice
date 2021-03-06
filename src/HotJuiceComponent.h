#pragma once

#if defined(JUCE_APP_VERSION)

#include "../JuceLibraryCode/JuceHeader.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <objc/objc.h>
#else
//#include <GL/gl.h>
//#include <GL/glu.h>
#endif

#include "HotJuicePluginProcessor.h"
#include "PluginManager.h"
#include "Plugin.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class HotJuiceComponent : public OpenGLAppComponent, public KeyListener
{
    
    enum ofKey {
        OF_KEY_SHIFT = 0x1,
        OF_KEY_CONTROL = 0x2,
        OF_KEY_ALT = 0x4,
        OF_KEY_SUPER = 0x10,
        OF_KEY_COMMAND = OF_KEY_SUPER,
        
        OF_KEY_BACKSPACE = 8,
        OF_KEY_DEL = 127,
        
        OF_KEY_RETURN    =    13,
        OF_KEY_ESC        =    27,
        OF_KEY_TAB      =    9,
        
        OF_KEY_F1 = 0xe000,
        OF_KEY_F2 = 0xe001,
        OF_KEY_F3 = 0xe002,
        OF_KEY_F4 = 0xe003,
        OF_KEY_F5 = 0xe004,
        OF_KEY_F6 = 0xe005,
        OF_KEY_F7 = 0xe006,
        OF_KEY_F8 = 0xe007,
        OF_KEY_F9 = 0xe008,
        OF_KEY_F10 = 0xe009,
        OF_KEY_F11 = 0xe00A,
        OF_KEY_F12 = 0xe00B,
        OF_KEY_LEFT = 0xe00C,
        OF_KEY_UP = 0xe00D,
        OF_KEY_RIGHT = 0xe00E,
        OF_KEY_DOWN = 0xe00F,
        OF_KEY_PAGE_UP = 0xe010,
        OF_KEY_PAGE_DOWN = 0xe011,
        OF_KEY_HOME = 0xe012,
        OF_KEY_END = 0xe013,
        OF_KEY_INSERT = 0xe014,
    };

public:
	HotJuicePluginProcessor* processor;
	std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;
	hotjuice::Plugin* plugin;
	bool isTransparent;
    
public:
    //==============================================================================
	HotJuiceComponent();
    ~HotJuiceComponent();

    //==============================================================================
    void initialise() override;
    void shutdown() override;
    void render() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    void mouseMove(const MouseEvent& event) override;
    void mouseDown(const MouseEvent& event) override;
	void mouseUp(const MouseEvent& event) override;
	void mouseDrag(const MouseEvent& event) override;

    void makeTransparent();

#ifndef WIN32
    bool isWindowFocused = false;
    
    void checkMainWindow();
    
    void addKeyboardMonitor();
    void removeKeyboardMonitor();
    
    id keyboardMonitor;
#endif
    
	std::map<int, juce_wchar> keysPressed;
	bool keyAltPressed;
	bool keyCommandPressed;
	bool keyCtrlPressed;
	bool keyShiftPressed;

	int convertKey(int key);

	bool keyPressed(const KeyPress &key, Component *originatingComponent) override;
	bool keyStateChanged(bool isKeyDown, juce::Component *originatingComponent) override;

	bool sendKeyPressedToSubplugin(int keyCode);
	bool sendKeyReleasedToSubPlugin(int keyCode);

 
    
private:
    //==============================================================================
    // Your private member variables go here...

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HotJuiceComponent)
};

#endif
