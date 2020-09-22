#pragma once

#if defined(JUCE_APP_VERSION)

#include "../JuceLibraryCode/JuceHeader.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
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
public:
	HotJuicePluginProcessor* processor;
	std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;

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

	std::map<int, juce_wchar> keysPressed;
	bool keyAltPressed;
	bool keyCommandPressed;
	bool keyCtrlPressed;
	bool keyShiftPressed;

	int convertKey(int key);

	bool keyPressed(const KeyPress &key, Component *originatingComponent) override;
	bool keyStateChanged(bool isKeyDown, juce::Component *originatingComponent) override;

	void sendPluginKeyPressed(int keyCode);
	void sendPluginKeyReleased(int keyCode);

private:
    //==============================================================================
    // Your private member variables go here...

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HotJuiceComponent)
};

#endif
