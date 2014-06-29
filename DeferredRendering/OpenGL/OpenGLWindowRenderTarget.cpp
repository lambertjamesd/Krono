#include "stdafx.h"
#include "OpenGLWindowRenderTarget.h"
#include <GL/glew.h>

#ifdef _WIN32

#pragma comment (lib, "Opengl32.lib")


OpenGLWindowRenderTarget::OpenGLWindowRenderTarget(Window& window) :
	mHDC(GetDC(window.GetWindowHandle()))
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iFormat = ChoosePixelFormat( mHDC, &pfd );
	SetPixelFormat( mHDC, iFormat, &pfd );

	mHGLRenderContext = wglCreateContext(mHDC);

	MakeActive();
}


OpenGLWindowRenderTarget::~OpenGLWindowRenderTarget(void)
{
	wglDeleteContext(mHGLRenderContext);
}

void OpenGLWindowRenderTarget::GetRenderTargetInternal(void* target) const
{
	*((GLuint*)target) = 0;
}

void OpenGLWindowRenderTarget::Clear(const Colorf& color)
{
	MakeActive();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}
	
void OpenGLWindowRenderTarget::Present(void)
{
	SwapBuffers(mHDC);
}
	
void OpenGLWindowRenderTarget::MakeActive()
{
	wglMakeCurrent(mHDC, mHGLRenderContext);
}

#else

#error OpenGL not current supported on platform

#endif