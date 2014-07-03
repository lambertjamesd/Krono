#pragma once
#include "..\Interface\Window.h"

class WindowsWindow :
	public Window
{
public:
	WindowsWindow(const Vector2i& size);
	virtual ~WindowsWindow(void);
	
	virtual void Show();
	virtual void Update(bool async = true);
	virtual bool IsClosed() const;
	virtual WindowHandle GetWindowHandle() const;
private:
	WindowsWindow(const WindowsWindow& other);
	WindowsWindow& operator=(const WindowsWindow& other);

	void DestoryWindow();

	HWND mWindowHandle;

	static WNDCLASS gWindowClass;
	static bool gWindowClassInitialized;
	static const wchar_t gWindowClassName[];

	static void InitializeClass();

	static LRESULT CALLBACK  WindowsProcedure(HWND windowHandle, UINT message, WPARAM wParameter, LPARAM lParameter);
};

