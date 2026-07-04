#ifndef INTERFACE_WINDOW_WINDOW_H_
#define INTERFACE_WINDOW_WINDOW_H_

#include <Windows.h>

#include <string>
#include <string_view>
#include <functional>

class Window
{
public:
	Window(HINSTANCE instance,
		const std::wstring_view title,
		const std::wstring_view klass);
	~Window();

	void Show(int show_cmd);
	int Run();

	inline HWND GetHandle() const { return hwnd_; };
	void GetSize(int& width, int& height);

	void SetResizeCallback(std::function<void(int, int)> callback) { on_resize_ = callback; };

private:
	bool RegisterWindowClass();
	bool CreateWin32Window();

	LRESULT HandleMessage(UINT message,
		WPARAM wparam,
		LPARAM lparam);

	static LRESULT CALLBACK WindowProc(
		HWND hwnd,
		UINT message,
		WPARAM wparam,
		LPARAM lparam
	);

private:
	std::wstring window_title_;
	std::wstring window_klass_;

	HWND hwnd_ = nullptr;
	HINSTANCE instance_ = nullptr;

	// callbacks
	std::function<void(int, int)> on_resize_ = nullptr;
};

#endif // INTERFACE_WINDOW_WINDOW_H_
