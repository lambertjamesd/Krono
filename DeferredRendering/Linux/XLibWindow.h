#pragma once
#include "Interface/Window.h"
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>

namespace krono
{

class XLibWindow :
	public Window
{
public:
	XLibWindow(const Vector2i& size);
	virtual ~XLibWindow(void);
	
	virtual void Show();
	virtual void Update(bool async = true);
	virtual bool IsClosed() const;
	virtual WindowHandle GetWindowHandle() const;

	Display* GetDisplay();
	XVisualInfo* GetVisualInfo();
private:
	XLibWindow(const XLibWindow& other);
	XLibWindow& operator=(const XLibWindow& other);

	::Window mWindowHandle;
	XVisualInfo *mVisualInfo;

	static Display *gDisplay;
	static Atom WM_DELETE_WINDOW;

	static void InitializeDisplay();
};

}
