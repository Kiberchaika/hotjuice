#include <AppKit/NSOpenGL.h>
#include <AppKit/NSWindow.h>
#include "HotJuiceComponent.h"

#if defined(JUCE_APP_VERSION)
void HotJuiceComponent::makeTransparent()
{
	NSView* handle = (NSView*)getWindowHandle();
	NSWindow* window = [handle window];
	[window setAcceptsMouseMovedEvents: YES];
	[window setIgnoresMouseEvents:NO];
    [window setOpaque:NO];

/*	
	[window setLevel:NSMainMenuWindowLevel + 1];
    [window setAlphaValue: 1.0];
	[window setHasShadow: NO];
	[window setMovableByWindowBackground: NO];
	[window setCanBecomeKeyWindow: YES];
	[window setAcceptsMouseMovedEvents: YES];

	setOpaque(false);
*/

	NSOpenGLContext* context = (NSOpenGLContext*)openGLContext.getRawContext();
	GLint aValue = 0;
	[context setValues : &aValue forParameter : NSOpenGLCPSurfaceOpacity];
}

void HotJuiceComponent::checkMainWindow()
{
    bool windowWasFocused = isWindowFocused;
    
    NSView* handle = (NSView*)getWindowHandle();
    if(handle) {
        NSWindow* window = [handle window];
        isWindowFocused = [window isMainWindow];
    } else {
        isWindowFocused = false;
    }
    
    
    if (windowWasFocused && !isWindowFocused) {
        // Just unfocused the window, let's send the key releases.
        // Releasing normal keys
        
        for (auto &key: keysPressed) {
                keysPressed.erase(key.first);

                int k = convertKey(key.first);
                directlySendPluginKeyReleased(k >= 0 ? k : KeyPress(key.first).getTextCharacter());
            }
        
        // Releasing modifier keys

        if (keyShiftPressed) {
            directlySendPluginKeyReleased(ofKey::OF_KEY_SHIFT);
            keyShiftPressed = false;
        }

        if (keyCtrlPressed) {
            directlySendPluginKeyReleased(ofKey::OF_KEY_CONTROL);
            keyCtrlPressed = false;
        }

        if (keyAltPressed) {
            directlySendPluginKeyReleased(ofKey::OF_KEY_ALT);
            keyAltPressed = false;
        }

        if (keyCommandPressed) {
            directlySendPluginKeyReleased(ofKey::OF_KEY_COMMAND);
            keyCommandPressed = false;
        }
    }
}

void HotJuiceComponent::removeKeyboardMonitor()
{
    [NSEvent removeMonitor:keyboardMonitor];
}

void HotJuiceComponent::addKeyboardMonitor()
{
    keyboardMonitor = [NSEvent addLocalMonitorForEventsMatchingMask: NSEventMaskFlagsChanged|NSEventMaskKeyDown|NSEventMaskKeyUp handler:^NSEvent*(NSEvent* event){
        
        // TODO: isWindowFocused sometimes doesn't work the first time you switch to this window.
        // Supposedly this only happens in AudioPluginHost due to a bug in it. Should investigate in other hosts.
        // For now, I'm removing the check for window focus, therefore the key presses will be sent to all windows.
        
//        if (!isWindowFocused) { return event; }
        
        // First, let's check if any of the modifier keys were pressed
        if (event.type == NSEventTypeFlagsChanged) {

            // Shift key
        
            if ((event.keyCode == 56) || (event.keyCode == 60)) { // this is Shift key, left and right
                if ([event modifierFlags] & NSEventModifierFlagShift) {
                    directlySendPluginKeyPressed(ofKey::OF_KEY_SHIFT);
                    keyShiftPressed = true;
                    return nil;
                } else {
                    directlySendPluginKeyReleased(ofKey::OF_KEY_SHIFT);
                    keyShiftPressed = false;
                    return nil;
                }
            }

            // Control key

            if ((event.keyCode == 59) || (event.keyCode == 62)) { // this is Control key, left and right
                if ([event modifierFlags] & NSEventModifierFlagControl) {
                    directlySendPluginKeyPressed(ofKey::OF_KEY_CONTROL);
                    keyCtrlPressed = true;
                    return nil;
                } else {
                    directlySendPluginKeyReleased(ofKey::OF_KEY_CONTROL);
                    keyCtrlPressed = false;
                    return nil;
                }
            }

            // Option key

            if ((event.keyCode == 58) || (event.keyCode == 61)) { // this is Option key, left and right
                if ([event modifierFlags] & NSEventModifierFlagOption) {
                    directlySendPluginKeyPressed(ofKey::OF_KEY_ALT);
                    keyAltPressed = true;
                    return nil;
                } else {
                    directlySendPluginKeyReleased(ofKey::OF_KEY_ALT);
                    keyAltPressed = false;
                    return nil;
                }
            }
            
            // Command key

            if ((event.keyCode == 55) || (event.keyCode == 54)) { // this is Command key, left and right
                if ([event modifierFlags] & NSEventModifierFlagCommand) {
                    directlySendPluginKeyPressed(ofKey::OF_KEY_COMMAND);
                    keyCommandPressed = true;
                    return nil;
                } else {
                    directlySendPluginKeyReleased(ofKey::OF_KEY_COMMAND);
                    keyCommandPressed = false;
                    return nil;
                }
            }
            
        }
        
        
        if (event.type == NSEventTypeKeyDown) {
            auto convertedKey = convertKey(event.keyCode);
            if (convertedKey == -1) convertedKey = [event.characters characterAtIndex:0];
            
            directlySendPluginKeyPressed(convertedKey);
            keysPressed[convertedKey] = KeyPress(convertedKey).getTextCharacter();

            return nil;
        }

        if (event.type == NSEventTypeKeyUp) {
            auto convertedKey = convertKey(event.keyCode);
            if (convertedKey == -1) convertedKey = [event.characters characterAtIndex:0];
            
            directlySendPluginKeyReleased(convertedKey);
            keysPressed.erase(convertedKey);
            return nil;
        }
         

        // To avoid error message, return nil
        return nil;
        }
     ];
    
    /*
    id monitor=[NSEvent addLocalMonitorForEventsMatchingMask:..
    // remove monitor
    [NSEvent removeMonitor:monitor];
     */
    
}
#endif
