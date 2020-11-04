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
    NSView* handle = (NSView*)getWindowHandle();
    if(handle) {
        NSWindow* window = [handle window];
        isMainWindow = [window isMainWindow];
    } else {
        isMainWindow = false;
    }
}

void HotJuiceComponent::checkModifierKeys(void* event)
{
    NSEvent* nsevent = (NSEvent*)event;
    
    if (([nsevent modifierFlags] & NSAlternateKeyMask) != keyAltPressed) {
        keyAltPressed = !keyAltPressed;
        if (keyAltPressed) {
            sendPluginKeyPressed(ofKey::OF_KEY_ALT);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_ALT);
        }
    }
    
    if (([nsevent modifierFlags] & NSControlKeyMask) != keyCtrlPressed) {
        keyCtrlPressed = !keyCtrlPressed;
        if (keyCtrlPressed) {
            sendPluginKeyPressed(ofKey::OF_KEY_CONTROL);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_CONTROL);
        }
    }
    
    if (([nsevent modifierFlags] & NSShiftKeyMask) != keyShiftPressed) {
        keyShiftPressed = !keyShiftPressed;
        if (keyShiftPressed) {
            sendPluginKeyPressed(ofKey::OF_KEY_SHIFT);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_SHIFT);
        }
    }
    
    if (([nsevent modifierFlags] & NSCommandKeyMask) != keyCommandPressed) {
        keyCommandPressed = !keyCommandPressed;
        if (keyCommandPressed) {
            sendPluginKeyPressed(ofKey::OF_KEY_COMMAND);
        }
        else {
            sendPluginKeyReleased(ofKey::OF_KEY_COMMAND);
        }
    }
}

void HotJuiceComponent::addKeyboardMonitor()
{
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskFlagsChanged handler:^NSEvent*(NSEvent* event){
        if(isMainWindow) {
            checkModifierKeys(event);
            return nil;
        }
        return event;
    }];
    
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown handler:^NSEvent*(NSEvent* event){
        if(isMainWindow) {
            checkModifierKeys(event);
            sendPluginKeyPressed(event.keyCode);
        }
        return event;
    }];
    
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyUp handler:^NSEvent*(NSEvent* event){
        if(isMainWindow) {
            checkModifierKeys(event);
            sendPluginKeyReleased(event.keyCode);
        }
        return event;
    }];

    /*
    id monitor=[NSEvent addLocalMonitorForEventsMatchingMask:..
    // remove monitor
    [NSEvent removeMonitor:monitor];
     */
}
#endif
