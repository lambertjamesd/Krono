#pragma once

#include "..\Math\Vector2.h"
#include "RenderTarget.h"
#include "../Core/Memory.h"

#ifdef _WIN32
#include <Windows.h>

typedef HWND WindowHandle;
#endif

class Window
{
public:
	virtual ~Window(void);

	Size GetSize() const;

	virtual void Show() =  0;
	virtual void Update(bool async = true) = 0;
	virtual bool IsClosed() const = 0;
	virtual WindowHandle GetWindowHandle() const = 0;

	static Auto<Window> Create(const Size& size);
protected:
	Window(const Size& size);
	Size mSize;
private:
	Window(const Window& other);
	Window& operator=(const Window& other);
};

