/*
 ==============================================================================
 
 This file was auto-generated!
 
 ==============================================================================
 */

#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    openGLContext.setOpenGLVersionRequired (OpenGLContext::OpenGLVersion::openGL3_2);
    setSize(800, 600);
	addKeyListener(this);
	setWantsKeyboardFocus(true);

	keyPressedAlt = false;
	keyPressedCommand = false;
	keyPressedCtrl = false;
	keyPressedShift = false;
	
}

MainComponent::~MainComponent()
{
    // This shuts down the GL system and stops the rendering calls.
    shutdownOpenGL();
}


//==============================================================================
void MainComponent::initialise()
{
	processor->needToReinitRender = true;
}

void MainComponent::shutdown()
{
    // Free any GL objects created for rendering here.
}

void MainComponent::render()
{
	if (!processor->isReloading && processor->plugin) {
		if (processor->needToReinitRender) {

			processor->plugin->setupRenderer();

			float desktopScale = openGLContext.getRenderingScale();
			processor->plugin->setDesktopScale(desktopScale);

			processor->needToReinitRender = false;
		}

		float in[2] = { 0, 0 };
		processor->plugin->update(&in, &out);

		processor->plugin->custom("test");
		processor->plugin->custom("test2");


		// This clears the context with a black background.
		OpenGLHelpers::clear(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

		float desktopScale = openGLContext.getRenderingScale();
		processor->plugin->setWindowSize(roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

		processor->plugin->draw(in);
	}

}

void MainComponent::mouseDrag(const MouseEvent & event)
{
    if (processor->plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        processor->plugin->mouseDragged(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

void MainComponent::mouseMove(const MouseEvent& event) {
    if (processor->plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        processor->plugin->mouseMoved(event.x * desktopScale, event.y * desktopScale);
    }
}

void MainComponent::mouseDown(const MouseEvent& event) {
    if (processor->plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        processor->plugin->mousePressed(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

void MainComponent::mouseUp(const MouseEvent& event) {
    if (processor->plugin) {
        float desktopScale = openGLContext.getRenderingScale();
        processor->plugin->mouseReleased(event.x * desktopScale, event.y * desktopScale, 0);
    }
}

int MainComponent::convertKey(int key) {
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

bool MainComponent::keyPressed(const KeyPress & key, Component * originatingComponent)
{
    ///*
    if (key.getModifiers().isAltDown() != keyPressedAlt) {
        keyPressedAlt = !keyPressedAlt;
        if (keyPressedAlt) {
            sendPluginKeyPressed(ofKey::OF_KEY_ALT);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_ALT);
        }
    }

    if (key.getModifiers().isCtrlDown() != keyPressedCtrl) {
        keyPressedCtrl = !keyPressedCtrl;
        if (keyPressedCtrl) {
            sendPluginKeyPressed(ofKey::OF_KEY_CONTROL);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_CONTROL);
        }
    }

    if (key.getModifiers().isShiftDown() != keyPressedShift) {
        keyPressedShift = !keyPressedShift;
        if (keyPressedShift) {
            sendPluginKeyPressed(ofKey::OF_KEY_SHIFT);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_SHIFT);
        }
    }

    if (key.getModifiers().isCommandDown() != keyPressedCommand) {
        keyPressedCommand = !keyPressedCommand;
        if (keyPressedCommand) {
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

bool MainComponent::keyStateChanged(bool isKeyDown, juce::Component * originatingComponent) {
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

void MainComponent::sendPluginKeyPressed(int keyCode) {
    if (processor->plugin) {
        processor->plugin->keyPressed(keyCode);
    }
}

void MainComponent::sendPluginKeyReleased(int keyCode) {
    if (processor->plugin) {
        processor->plugin->keyReleased(keyCode);
    }
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
    // You can add your component specific drawing code here!
    // This will draw over the top of the openGL background.
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

