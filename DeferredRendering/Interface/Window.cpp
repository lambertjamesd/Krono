#include "stdafx.h"
#include "Window.h"

#include "../Windows/WindowsWindow.h"


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
#ifdef _WIN32
	return Auto<WindowsWindow>(new WindowsWindow(size));
#else
	return NULL;
#endif
}