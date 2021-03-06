#include "HotJuiceComponent.h"

#if defined(JUCE_APP_VERSION)

//==============================================================================
HotJuiceComponent::HotJuiceComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    openGLContext.setOpenGLVersionRequired (OpenGLContext::OpenGLVersion::openGL3_2);
    openGLContext.setMultisamplingEnabled(true);

	OpenGLPixelFormat pixelFormat;
	pixelFormat.multisamplingLevel = 8;
	openGLContext.setPixelFormat(pixelFormat);

    setSize(1, 1); 
#ifdef  WIN32
	setWantsKeyboardFocus(true);
#endif
    
	keyAltPressed = false;
	keyCommandPressed = false;
	keyCtrlPressed = false;
	keyShiftPressed = false;

	processor = nullptr;
	plugin = nullptr;

	isTransparent = false;

	lastFrameTime = std::chrono::high_resolution_clock::now();
}

HotJuiceComponent::~HotJuiceComponent()
{
    openGLContext.deactivateCurrentContext();
    openGLContext.detach();
}

#ifdef  WIN32
void HotJuiceComponent::makeTransparent()
{
	HWND hWnd = (HWND)getWindowHandle();
	LONG windowStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	SetWindowLong(hWnd, GWL_EXSTYLE, windowStyle | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_COLORKEY);
}
#endif

//==============================================================================
void HotJuiceComponent::initialise()
{
#ifdef  WIN32
	getTopLevelComponent()->addKeyListener(this);
#endif
    
	if (plugin) {
		plugin->setState(hotjuice::PluginStateSetupRender);
	}

	if (isTransparent) {
		makeTransparent();
	}
    
    OpenGLHelpers::clear(juce::Colours::transparentBlack);


}

void HotJuiceComponent::shutdown()
{
    // Free any GL objects created for rendering here.
    if (plugin) {
        plugin->prepareToStopRendering();
    }
}

void HotJuiceComponent::render()
{
	// limit fps
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastFrameTime) < std::chrono::milliseconds{ 1000 / 30 }) return;
	lastFrameTime = std::chrono::high_resolution_clock::now();

    if(plugin) {
        if (!processor->isReloading) {
            if (plugin->getState() == hotjuice::PluginStateSetupRender) {
                plugin->setupRenderer();
				plugin->prepareToStartRendering(plugin->doNeedToReloadData());

                plugin->setClipboardCallbacks(
                                              [&]() -> std::string { return SystemClipboard::getTextFromClipboard().toStdString(); },
                                              [&](std::string clipboard) -> void { SystemClipboard::copyTextToClipboard(clipboard); }
                                              );
                plugin->addCallback("hideCursor", [&](void* in, void* out) {
                    setMouseCursor(MouseCursor::NoCursor);
                });
                plugin->addCallback("showCursor", [&](void* in, void* out) {
                    setMouseCursor(MouseCursor::NormalCursor);
                });
                plugin->addCallback("setCursorPosition", [&](void* in, void* out) {
                    if (in != nullptr) {
                        int(&pos)[2] = *reinterpret_cast<int(*)[2]>(in);
                        Desktop::setMousePosition(localPointToGlobal(juce::Point<int>(pos[0], pos[1])));
                    }
                });

				plugin->setNeedToReloadData(true);
				plugin->setState(hotjuice::PluginStateNone);
			}
            
            /*
            plugin->custom("test");
            */
            
            float desktopScale = openGLContext.getRenderingScale();
            plugin->setDesktopScale(desktopScale);
            plugin->setWindowSize(roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));
            
            OpenGLHelpers::clear(juce::Colours::transparentBlack);
            plugin->draw();
        }
        else if (plugin->getState() == hotjuice::PluginStateCloseRender) {
            plugin->prepareToStopRendering();
            plugin->closeRenderer();
			plugin->setState(hotjuice::PluginStateNone);
        }
    }
}

void HotJuiceComponent::mouseDrag(const MouseEvent & event)
{
    if (!processor->isReloading && plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        plugin->mouseDragged(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

void HotJuiceComponent::mouseMove(const MouseEvent& event) {
    if (!processor->isReloading && plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        plugin->mouseMoved(event.x * desktopScale, event.y * desktopScale);
    }
}

void HotJuiceComponent::mouseDown(const MouseEvent& event) {
    if (!processor->isReloading && plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        plugin->mousePressed(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

void HotJuiceComponent::mouseUp(const MouseEvent& event) {
    if (!processor->isReloading && plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        plugin->mouseReleased(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

int HotJuiceComponent::convertKey(int key) {
    #ifdef  WIN32
    if (key == KeyPress::F1Key) return ofKey::OF_KEY_F1;
    if (key == KeyPress::F2Key) return ofKey::OF_KEY_F2;
    if (key == KeyPress::F3Key) return ofKey::OF_KEY_F3;
    if (key == KeyPress::F4Key) return ofKey::OF_KEY_F4;
    if (key == KeyPress::F5Key) return ofKey::OF_KEY_F5;
    if (key == KeyPress::F6Key) return ofKey::OF_KEY_F6;
    if (key == KeyPress::F7Key) return ofKey::OF_KEY_F7;
    if (key == KeyPress::F8Key) return ofKey::OF_KEY_F8;
    if (key == KeyPress::F9Key) return ofKey::OF_KEY_F9;
    if (key == KeyPress::F10Key) return ofKey::OF_KEY_F10;
    if (key == KeyPress::F11Key) return ofKey::OF_KEY_F11;
    if (key == KeyPress::F12Key) return ofKey::OF_KEY_F12;
    if (key == KeyPress::leftKey) return ofKey::OF_KEY_LEFT;
    if (key == KeyPress::rightKey) return ofKey::OF_KEY_RIGHT;
    if (key == KeyPress::upKey) return ofKey::OF_KEY_UP;
    if (key == KeyPress::downKey) return ofKey::OF_KEY_DOWN;
    if (key == KeyPress::homeKey) return ofKey::OF_KEY_HOME;
    if (key == KeyPress::endKey) return ofKey::OF_KEY_END;
    if (key == KeyPress::deleteKey) return ofKey::OF_KEY_DEL;
    if (key == KeyPress::backspaceKey) return ofKey::OF_KEY_BACKSPACE;
    #else // Mac key codes
    if (key == 123) return ofKey::OF_KEY_LEFT;
    if (key == 124) return ofKey::OF_KEY_RIGHT;
    if (key == 126) return ofKey::OF_KEY_UP;
    if (key == 125) return ofKey::OF_KEY_DOWN;
    if (key == 51) return ofKey::OF_KEY_BACKSPACE;
    if (key == 36) return ofKey::OF_KEY_RETURN;
    if (key == 53) return ofKey::OF_KEY_ESC;
    if (key == 48) return ofKey::OF_KEY_TAB;
    #endif
    
    return -1;
}

bool HotJuiceComponent::keyPressed(const KeyPress & key, Component * originatingComponent)
{
    int keyCode = key.getKeyCode();
    if (keysPressed.find(keyCode) == keysPressed.end()) {
        keysPressed[keyCode] = key.getTextCharacter();
    }

    int k = convertKey(keyCode);
    auto subpluginResult = sendKeyPressedToSubplugin(k >= 0 ? k : keysPressed[keyCode]);

    return subpluginResult;
}

// This works on Windows, gets called when modifier keys are pressed
bool HotJuiceComponent::keyStateChanged(bool isKeyDown, juce::Component * originatingComponent) {
	 
	if (ModifierKeys::getCurrentModifiers().isAltDown() != keyAltPressed) {
		keyAltPressed = !keyAltPressed;
		if (keyAltPressed) {
			sendKeyPressedToSubplugin(ofKey::OF_KEY_ALT);
		}
		else {
			sendKeyReleasedToSubPlugin(ofKey::OF_KEY_ALT);
		}
	}

	if (ModifierKeys::getCurrentModifiers().isCtrlDown() != keyCtrlPressed) {
		keyCtrlPressed = !keyCtrlPressed;
		if (keyCtrlPressed) {
			sendKeyPressedToSubplugin(ofKey::OF_KEY_CONTROL);
		}
		else {
			sendKeyReleasedToSubPlugin(ofKey::OF_KEY_CONTROL);
		}
	}

	if (ModifierKeys::getCurrentModifiers().isShiftDown() != keyShiftPressed) {
		keyShiftPressed = !keyShiftPressed;
		if (keyShiftPressed) {
			sendKeyPressedToSubplugin(ofKey::OF_KEY_SHIFT);
		}
		else {
			sendKeyReleasedToSubPlugin(ofKey::OF_KEY_SHIFT);
		}
	}

	if (ModifierKeys::getCurrentModifiers().isCommandDown() != keyCommandPressed) {
		keyCommandPressed = !keyCommandPressed;
		if (keyCommandPressed) {
			sendKeyPressedToSubplugin(ofKey::OF_KEY_COMMAND);
		}
		else {
			sendKeyReleasedToSubPlugin(ofKey::OF_KEY_COMMAND);
		}
	}
	 
	for (auto iter = keysPressed.begin(); iter != keysPressed.end(); ++iter) {
        auto keyCode = iter->first;
        auto textCharacter = iter->second;

        if (!KeyPress::isKeyCurrentlyDown(keyCode)) {
            keysPressed.erase(iter);

            int k = convertKey(keyCode);
            sendKeyReleasedToSubPlugin(k >= 0 ? k : textCharacter);
            break;
        }
    }
    return true;
}

bool HotJuiceComponent::sendKeyPressedToSubplugin(int keyCode) {
    if (!processor->isReloading && plugin) {
        return plugin->keyPressed(keyCode);
    }
    return false;
}

bool HotJuiceComponent::sendKeyReleasedToSubPlugin(int keyCode) {
    if (!processor->isReloading && plugin) {
        return plugin->keyReleased(keyCode);
    }
    return false;
}

//==============================================================================
void HotJuiceComponent::paint(Graphics& g)
{
    // You can add your component specific drawing code here!
    // This will draw over the top of the openGL background.
}

void HotJuiceComponent::resized()
{
    // This is called when the HotJuiceComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}


#endif
