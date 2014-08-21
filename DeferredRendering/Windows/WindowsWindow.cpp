

#include "WindowsWindow.h"

namespace krono
{

WNDCLASS WindowsWindow::gWindowClass = {};
bool WindowsWindow::gWindowClassInitialized = false;
const wchar_t WindowsWindow::gWindowClassName[] = L"Game Window";

WindowsWindow::WindowsWindow(const Vector2i& size) : Window(size)
{
	InitializeClass();
	RECT windowRect = {0, 0, size.x, size.y};
	AdjustWindowRectEx(&windowRect, WS_OVERLAPPEDWINDOW, false, 0);

	mWindowHandle = CreateWindowEx(
		0,                              // Optional window styles.
		gWindowClassName,               // Window class
		L"Game",						// Window text
		WS_OVERLAPPED | WS_SYSMENU,     // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,

		NULL,       // Parent window    
		NULL,       // Menu
		GetModuleHandle(NULL),  // Instance handle
		this        // Additional application data
    );
}


WindowsWindow::~WindowsWindow(void)
{
	DestoryWindow();
}

void WindowsWindow::Show()
{
	ShowWindow(mWindowHandle, SW_SHOW);
}

void WindowsWindow::Update(bool async)
{
	MSG message;
	BOOL messageResult = false;

	if (async)
	{
		messageResult = PeekMessage(&message, mWindowHandle, 0, 0, PM_REMOVE);
	}
	else
	{
		messageResult = GetMessage(&message, mWindowHandle, 0, 0);
	}

	TranslateMessage(&message);
	DispatchMessage(&message);
}

bool WindowsWindow::IsClosed() const
{
	return mWindowHandle == NULL;
}

WindowHandle WindowsWindow::GetWindowHandle() const
{
	return mWindowHandle;
}
	
void WindowsWindow::DestoryWindow()
{
	if (mWindowHandle != NULL)
	{
		DestroyWindow(mWindowHandle);
		mWindowHandle = NULL;
	}
}

void WindowsWindow::InitializeClass()
{
	if (!gWindowClassInitialized)
	{
		gWindowClass.lpfnWndProc   = WindowsProcedure;
		gWindowClass.hInstance     = GetModuleHandle(NULL);
		gWindowClass.lpszClassName = gWindowClassName;

		RegisterClass(&gWindowClass);

		gWindowClassInitialized = true;
	}
}

LRESULT CALLBACK  WindowsWindow::WindowsProcedure(HWND windowHandle, UINT message, WPARAM wParameter, LPARAM lParameter)
{ 
	WindowsWindow *window = 0;

	if(message == WM_NCCREATE)
	{
		// retrieve Window instance from window creation data and associate
		window = reinterpret_cast<WindowsWindow *>(((LPCREATESTRUCT)lParameter)->lpCreateParams);
		SetWindowLongPtr(windowHandle, GWL_USERDATA, reinterpret_cast<long>(window));
	}
	else
	{
		// retrieve associated Window instance
		window = reinterpret_cast<WindowsWindow *>(GetWindowLongPtr(windowHandle, GWL_USERDATA));
	}

    switch(message)
    {
        case WM_CLOSE:
			window->DestoryWindow();
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(windowHandle, message, wParameter, lParameter);
    }
    return 0;
}

}
