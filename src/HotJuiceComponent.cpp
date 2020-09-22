#include "HotJuiceComponent.h"

#if defined(JUCE_APP_VERSION)

enum ofKey {
	OF_KEY_SHIFT = 0x1,
	OF_KEY_CONTROL = 0x2,
	OF_KEY_ALT = 0x4,
	OF_KEY_SUPER = 0x10,
	OF_KEY_COMMAND = OF_KEY_SUPER,

	OF_KEY_BACKSPACE = 8,
	OF_KEY_DEL = 127,

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

    setSize(500, 540);
    addMouseListener(this, true);
	addKeyListener(this);

	keyAltPressed = false;
	keyCommandPressed = false;
	keyCtrlPressed = false;
	keyShiftPressed = false;

	lastFrameTime = std::chrono::high_resolution_clock::now();
}

HotJuiceComponent::~HotJuiceComponent()
{
    // This shuts down the GL system and stops the rendering calls.
    shutdownOpenGL();
}


//==============================================================================
void HotJuiceComponent::initialise()
{
	processor->needToReinitRender = true;
}

void HotJuiceComponent::shutdown()
{
    // Free any GL objects created for rendering here.
}

void HotJuiceComponent::render()
{
	// limit fps
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastFrameTime) < std::chrono::milliseconds{ 1000 / 60 }) return;
	lastFrameTime = std::chrono::high_resolution_clock::now();

	if (!processor->isReloading && processor->plugin) {
		if (processor->needToReinitRender) {
			processor->plugin->setupRenderer();
			processor->plugin->addCallback("hideCursor", [&](void* in, void* out) {
				setMouseCursor(MouseCursor::NoCursor);
			});
			processor->plugin->addCallback("showCursor", [&](void* in, void* out) {
				setMouseCursor(MouseCursor::NormalCursor);
			});
			processor->plugin->addCallback("setCursorPosition", [&](void* in, void* out) {
				if (in != nullptr) {
					int(&pos)[2] = *reinterpret_cast<int(*)[2]>(in);
					Desktop::setMousePosition(localPointToGlobal(juce::Point<int>(pos[0], pos[1])));
				}
			});
			processor->needToReinitRender = false;
		}

		/*
		processor->plugin->custom("test");
		processor->plugin->custom("test2");
		*/
        
		// This clears the context with a black background.
		OpenGLHelpers::clear(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

		float desktopScale = openGLContext.getRenderingScale();
        //float desktopScale = Desktop::getInstance().getDisplays().findDisplayForRect (getScreenBounds()).scale;
        processor->plugin->setDesktopScale(desktopScale);
		processor->plugin->setWindowSize(roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

		processor->plugin->draw();
	}

}

void HotJuiceComponent::mouseDrag(const MouseEvent & event)
{
    if (!processor->isReloading && processor->plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        processor->plugin->mouseDragged(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

void HotJuiceComponent::mouseMove(const MouseEvent& event) {
    if (!processor->isReloading && processor->plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        processor->plugin->mouseMoved(event.x * desktopScale, event.y * desktopScale);
    }
}

void HotJuiceComponent::mouseDown(const MouseEvent& event) {
    if (!processor->isReloading && processor->plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        processor->plugin->mousePressed(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

void HotJuiceComponent::mouseUp(const MouseEvent& event) {
    if (!processor->isReloading && processor->plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        processor->plugin->mouseReleased(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

int HotJuiceComponent::convertKey(int key) {
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
    return -1;
}

bool HotJuiceComponent::keyPressed(const KeyPress & key, Component * originatingComponent)
{
    ///*
    if (key.getModifiers().isAltDown() != keyAltPressed) {
        keyAltPressed = !keyAltPressed;
        if (keyAltPressed) {
            sendPluginKeyPressed(ofKey::OF_KEY_ALT);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_ALT);
        }
    }

    if (key.getModifiers().isCtrlDown() != keyCtrlPressed) {
        keyCtrlPressed = !keyCtrlPressed;
        if (keyCtrlPressed) {
            sendPluginKeyPressed(ofKey::OF_KEY_CONTROL);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_CONTROL);
        }
    }

    if (key.getModifiers().isShiftDown() != keyShiftPressed) {
        keyShiftPressed = !keyShiftPressed;
        if (keyShiftPressed) {
            sendPluginKeyPressed(ofKey::OF_KEY_SHIFT);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_SHIFT);
        }
    }

    if (key.getModifiers().isCommandDown() != keyCommandPressed) {
        keyCommandPressed = !keyCommandPressed;
        if (keyCommandPressed) {
            sendPluginKeyPressed(ofKey::OF_KEY_COMMAND);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_COMMAND);
        }
    }

    int keyCode = key.getKeyCode();
    if (keysPressed.find(keyCode) == keysPressed.end()) {
        keysPressed[keyCode] = key.getTextCharacter();
    }

    int k = convertKey(keyCode);
    sendPluginKeyPressed(k >= 0 ? k : keysPressed[keyCode]);

    return true;
}

bool HotJuiceComponent::keyStateChanged(bool isKeyDown, juce::Component * originatingComponent) {
    for (auto iter = keysPressed.begin(); iter != keysPressed.end(); ++iter) {
        auto keyCode = iter->first;
        auto textCharacter = iter->second;

        if (!KeyPress::isKeyCurrentlyDown(keyCode)) {
            keysPressed.erase(iter);

            int k = convertKey(keyCode);
            sendPluginKeyReleased(k >= 0 ? k : textCharacter);
            break;
        }
    }
    return true;
}

void HotJuiceComponent::sendPluginKeyPressed(int keyCode) {
    if (!processor->isReloading && processor->plugin) {
        processor->plugin->keyPressed(keyCode);
    }
}

void HotJuiceComponent::sendPluginKeyReleased(int keyCode) {
    if (!processor->isReloading && processor->plugin) {
        processor->plugin->keyReleased(keyCode);
    }
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
