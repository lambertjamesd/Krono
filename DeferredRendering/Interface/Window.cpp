
#include "Window.h"

#if defined(_WIN32)
#include "Windows/WindowsWindow.h"
#elif defined(USE_XLIB)
#include "Linux/XLibWindow.h"
#endif

namespace krono
{

Window::Window(const Vector2i& size) :
	mSize(size),
	mCurrentInputStateIndex(0)
{
	mCurrentInputState.SetScreenSize(size);
}


Window::~Window(void)
{
}

Vector2i Window::GetSize() const
{
	return mSize;
}

const InputState& Window::CurrentInputState() const
{
	return mPreviousStates[mCurrentInputStateIndex];
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

void Window::CommitCurrentInputState()
{
	mPreviousStates[mCurrentInputStateIndex].SetPreviousState(NULL);
	size_t nextState = 1 - mCurrentInputStateIndex;
	mPreviousStates[nextState] = mCurrentInputState;
	mPreviousStates[nextState].SetPreviousState(&mPreviousStates[mCurrentInputStateIndex]);
	mCurrentInputStateIndex = nextState;
}

}
