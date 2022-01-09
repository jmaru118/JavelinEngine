#include "Window.h"

Window* window=nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	switch (msg)
	{

	case WM_CREATE:
	{
		// Event fired when window is created
		Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		//THIS SNIPPET OF CODE WILL GET THE "THIS" POINTER PASSED IN CREATEWINDOWEX

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		//THIS SNIPPET OF CODE, INSTEAD, WILL SAVE THE POINTER "THIS" IN A SPECIAL
			// STRUCTURE DATA IDENTIFIED BY HWND
		window->setHWND(hwnd);
		window->onCreate();
		break;
	}

	case WM_DESTROY:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		//THIS SNIPPET OF CODE WILL RECOVER THE POINTER "THIS" FROM THE 
		// SPECIAL STRUCTURE DATA IDENTIFIED BY HWND
		// Event fired when window is destroyed
		window->onDestroy();
		::PostQuitMessage(0);
		break;
	}

	default:
		return ::DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

bool Window::init()
{
	const wchar_t CLASS_NAME[] = L"MyWindowClass";
	const wchar_t MENU_NAME[] = L"";

	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.lpszMenuName = MENU_NAME;
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!::RegisterClassEx(&wc))
		return false;

	// creation of window
	m_hwnd=::CreateWindowExA(WS_EX_OVERLAPPEDWINDOW, "MyWindowClass",
		"DirectX Application", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, this);

	// if creation fails return false
	if (!m_hwnd)
		return false;

	// show the window
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	// set window running flag to true
	m_is_run = true;

	return true;
}

bool Window::broadcast()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	this->onUpdate();
	Sleep(0);

	return true;
}

bool Window::release()
{
	// Destroy window
	if (!::DestroyWindow(m_hwnd))
		return false;

	return true;
}

bool Window::isRun()
{
	return m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::onCreate()
{

}

void Window::onDestroy()
{
	m_is_run = false;
}
