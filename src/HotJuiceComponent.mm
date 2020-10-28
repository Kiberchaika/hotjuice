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
#endif
