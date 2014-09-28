#pragma once

#include "Math/Vector2.h"
#include "RenderTarget.h"
#include "Core/Memory.h"
#include "Input/InputState.h"

#if defined(_WIN32)
#include <Windows.h>
#endif

namespace krono
{

#if defined(_WIN32)

typedef HWND WindowHandle;

#elif defined(USE_XLIB)

typedef unsigned long WindowHandle;

#else

#error no window system found

#endif


class Window
{
public:
	typedef Auto<Window> Ptr;

	virtual ~Window(void);

	Vector2i GetSize() const;

	virtual void Show() =  0;
	virtual void Update(bool async = true) = 0;
	virtual bool IsClosed() const = 0;
	virtual WindowHandle GetWindowHandle() const = 0;
	virtual const InputState& CurrentInputState() const;

	static Auto<Window> Create(const Vector2i& size);
protected:
	Window(const Vector2i& size);
	Vector2i mSize;
	
	InputState mCurrentInputState;
	void CommitCurrentInputState();
private:
	Window(const Window& other);
	Window& operator=(const Window& other);

	InputState mPreviousStates[2];
	size_t mCurrentInputStateIndex;
};

}

