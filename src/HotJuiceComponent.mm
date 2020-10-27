#include <AppKit/NSOpenGL.h>
#include "HotJuiceComponent.h"
 
#if defined(JUCE_APP_VERSION)
void HotJuiceComponent::makeTransparent()
{
	setOpaque(false);

	NSOpenGLContext* context = (NSOpenGLContext*)openGLContext.getRawContext();
	GLint aValue = 0;
	[context setValues : &aValue forParameter : NSOpenGLCPSurfaceOpacity];
}
#endif
