
#include "Window.h"

#if defined(_WIN32)
#include "Windows/WindowsWindow.h"
#elif defined(USE_XLIB)
#include "Linux/XLibWindow.h"
#endif

namespace krono
{

Window::Window(const Vector2i& size) :
	mSize(size)
{

}


Window::~Window(void)
{
}

Vector2i Window::GetSize() const
{
	return mSize;
}

Auto<Window> Window::Create(const Vector2i& size)
{
#if defined(_WIN32)
	return Auto<WindowsWindow>(new WindowsWindow(size));
#elif defined(USE_XLIB)
	return Auto<XLibWindow>(new XLibWindow(size));
#else
	return NULL;
#endif
}

}
