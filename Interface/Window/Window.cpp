#include "Window.h"

#include <stdexcept>

#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(HINSTANCE instance,
	const std::wstring_view title,
	const std::wstring_view klass) :
	instance_(instance), window_title_(title), window_klass_(klass)
{
	if (!RegisterWindowClass())
		throw std::exception("Window: Failed to register window class.");
	if (!CreateWin32Window())
		throw std::exception("Window: Failed to create win32 window.");
}

Window::~Window()
{
	DestroyWindow(hwnd_);

	UnregisterClass(window_klass_.c_str(), instance_);
}

void Window::Show(int show_cmd)
{
	ShowWindow(hwnd_, show_cmd);
	UpdateWindow(hwnd_);
}

int Window::Run()
{
	MSG message{};

	while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE) > 0)
	{
		if (message.message == WM_QUIT)
			return false;

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return true;
}

void Window::GetSize(int& width, int& height)
{
	RECT rect{};
	GetClientRect(hwnd_, &rect);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
}

LRESULT CALLBACK Window::WindowProc(
	HWND hwnd,
	UINT message,
	WPARAM wparam,
	LPARAM lparam
)
{
	Window* window = nullptr;

	if (message == WM_NCCREATE)
	{
		auto create_struct = reinterpret_cast<CREATESTRUCT*>(lparam);

		window = static_cast<Window*>(create_struct->lpCreateParams);
		window->hwnd_ = hwnd;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
	}
	else
	{
		window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (window)
		return window->HandleMessage(message, wparam, lparam);

	return DefWindowProc(hwnd, message, wparam, lparam);
}

bool Window::RegisterWindowClass()
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = instance_;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = window_klass_.c_str();
	wc.lpfnWndProc = Window::WindowProc;
	wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);

	return RegisterClassEx(&wc) != 0;
}

bool Window::CreateWin32Window()
{
	hwnd_ = CreateWindowEx(0,
		window_klass_.c_str(),
		window_title_.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 720,
		nullptr,
		nullptr,
		instance_,
		this);

	return hwnd_ != nullptr;
}

LRESULT Window::HandleMessage(UINT message,
	WPARAM wparam,
	LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
	{
		int width = LOWORD(lparam);
		int height = HIWORD(lparam);

		if (on_resize_)
			on_resize_(width, height);
		return 0;
	}
	}

	if (ImGui_ImplWin32_WndProcHandler(hwnd_, message, wparam, lparam))
		return true;

	return DefWindowProc(hwnd_, message, wparam, lparam);
}