#include <AppKit/NSOpenGL.h>
#include "HotJuiceComponent.h"
 
#ifndef  WIN32
void HotJuiceComponent::makeTransparent()
{
	NSOpenGLContext* context = (NSOpenGLContext*)openGLContext.getRawContext();
	GLint aValue = 0;
	[context setValues : &aValue forParameter : NSOpenGLCPSurfaceOpacity];
}
#endif
