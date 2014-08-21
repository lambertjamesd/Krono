
#include "OpenGLWindowRenderTarget.h"

#if defined(USE_XLIB)

#include "Linux/XLibWindow.h"

#endif

namespace krono
{

#ifdef _WIN32

#pragma comment (lib, "Opengl32.lib")


OpenGLWindowRenderTarget::OpenGLWindowRenderTarget(Window& window) :
	WindowRenderTarget(window.GetSize()),
	mHDC(GetDC(window.GetWindowHandle()))
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 0;
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

Auto<Texture2D> OpenGLWindowRenderTarget::GetTexture() const
{
	return Auto<Texture2D>(NULL);
}

void OpenGLWindowRenderTarget::Clear(const Colorf& color)
{
	MakeActive();
	GLuint existingBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&existingBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, existingBuffer);
}
	
void OpenGLWindowRenderTarget::Present(void)
{
	SwapBuffers(mHDC);
}
	
void OpenGLWindowRenderTarget::MakeActive()
{
	wglMakeCurrent(mHDC, mHGLRenderContext);
}


OpenGLRenderTarget::Type OpenGLWindowRenderTarget::GetType() const
{
	return OpenGLRenderTarget::TypeWindow;
}

GLuint OpenGLWindowRenderTarget::GetGLObject() const
{
	return 0;
}

#elif defined(USE_XLIB)

OpenGLWindowRenderTarget::OpenGLWindowRenderTarget(Window& window) :
	WindowRenderTarget(window.GetSize()),
	mWindowHandle(window.GetWindowHandle())
{
	XLibWindow *xLibWindow = dynamic_cast<XLibWindow*>(&window);
	mWindowDisplay = xLibWindow->GetDisplay();

	mContext = glXCreateContext(mWindowDisplay, xLibWindow->GetVisualInfo(), NULL, GL_TRUE);

	MakeActive();
}


OpenGLWindowRenderTarget::~OpenGLWindowRenderTarget(void)
{
	glXMakeCurrent(mWindowDisplay, None, NULL);
	glXDestroyContext(mWindowDisplay, mContext);
}

Auto<Texture2D> OpenGLWindowRenderTarget::GetTexture() const
{
	return Auto<Texture2D>(NULL);
}

void OpenGLWindowRenderTarget::Clear(const Colorf& color)
{
	MakeActive();

	GLuint existingBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&existingBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, existingBuffer);
}
	
void OpenGLWindowRenderTarget::Present(void)
{
	glXSwapBuffers(mWindowDisplay, mWindowHandle);
}

OpenGLRenderTarget::Type OpenGLWindowRenderTarget::GetType() const
{
	return OpenGLRenderTarget::TypeWindow;
}

GLuint OpenGLWindowRenderTarget::GetGLObject() const
{
	return 0;
}

void OpenGLWindowRenderTarget::MakeActive()
{
	glXMakeCurrent(mWindowDisplay, mWindowHandle, mContext);
}


#endif

}
