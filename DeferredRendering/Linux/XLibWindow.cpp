
#include "XLibWindow.h"
#include <iostream>

namespace krono
{

XLibWindow::XLibWindow(const Vector2i& size) :
	Window(size),
	mWindowHandle(0)
{
	InitializeDisplay();
	int screenNumber = DefaultScreen(gDisplay);

	::Window root = DefaultRootWindow(gDisplay);

	GLint attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	mVisualInfo = glXChooseVisual(gDisplay, 0, attributes);
	Colormap colorMap = XCreateColormap(gDisplay, root, mVisualInfo->visual, AllocNone);
	XSetWindowAttributes windowAttributes;

	windowAttributes.colormap = colorMap;
	windowAttributes.event_mask = ExposureMask | KeyPressMask;

	mWindowHandle = XCreateWindow(gDisplay, root,
		0, 0,
		size.x, size.y,
		0,
		mVisualInfo->depth, InputOutput, mVisualInfo->visual,
		CWColormap | CWEventMask,
		&windowAttributes);

	XSelectInput(gDisplay, mWindowHandle, ExposureMask | KeyPressMask);
	XSetWMProtocols(gDisplay, mWindowHandle, &WM_DELETE_WINDOW, 1);
}

XLibWindow::~XLibWindow(void)
{

}

void XLibWindow::Show()
{
	XMapWindow(gDisplay, mWindowHandle);
}

void XLibWindow::Update(bool async)
{
	if (mWindowHandle && (!async || XPending(gDisplay)))
	{
		XEvent event;

		XNextEvent(gDisplay, &event);

		switch (event.type)
		{
			case ClientMessage:
	       			if(event.xclient.data.l[0] == WM_DELETE_WINDOW) 
				{
					XDestroyWindow(gDisplay, mWindowHandle);
					mWindowHandle = 0;
				}
				break;
		}
	}
}

bool XLibWindow::IsClosed() const
{
	return mWindowHandle == 0;
}

WindowHandle XLibWindow::GetWindowHandle() const
{
	return mWindowHandle;
}

Display* XLibWindow::GetDisplay()
{
	return gDisplay;
}

XVisualInfo* XLibWindow::GetVisualInfo()
{
	return mVisualInfo;
}

void XLibWindow::InitializeDisplay()
{
	if (gDisplay == NULL)
	{
		gDisplay = XOpenDisplay(NULL);
		WM_DELETE_WINDOW = XInternAtom(gDisplay, "WM_DELETE_WINDOW", false);
	}
}

Display *XLibWindow::gDisplay = NULL;
Atom XLibWindow::WM_DELETE_WINDOW;

}
