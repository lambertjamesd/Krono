#include "stdafx.h"
#include "Window.h"

#include "../Windows/WindowsWindow.h"


Window::Window(const Size& size) :
	mSize(size)
{

}


Window::~Window(void)
{
}

Size Window::GetSize() const
{
	return mSize;
}

Auto<Window> Window::Create(const Size& size)
{
#ifdef _WIN32
	return Auto<WindowsWindow>(new WindowsWindow(size));
#else
	return NULL;
#endif
}