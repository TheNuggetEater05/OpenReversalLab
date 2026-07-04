#ifndef INTERFACE_APPLICATION_APPLICATION_H_
#define INTERFACE_APPLICATION_APPLICATION_H_

#include <Windows.h>

#include <string>
#include <memory>

#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include "../Menu/Menu.h"

class Application
{
public:
	Application(HINSTANCE instance,
		const std::wstring_view title,
		const std::wstring_view klass);
	~Application() = default;

	int Run(int show_cmd);

private:
	Window window_;
	std::unique_ptr<Renderer> renderer_{};
	std::unique_ptr<Menu> menu_{};
};

#endif // INTERFACE_APPLICATION_APPLICATION_H_
