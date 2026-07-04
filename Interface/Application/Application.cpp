#include "Application.h"

Application::Application(HINSTANCE instance,
	const std::wstring_view title,
	const std::wstring_view klass) :
	window_(instance, title, klass)
{
};

int Application::Run(int show_cmd)
{
	// initialization of classes
	if (renderer_ == nullptr)
	{
		int width{}, height{};
		window_.GetSize(width, height);
		renderer_ = std::make_unique<Renderer>(window_.GetHandle(), width, height);
	}
	if (menu_ == nullptr)
	{
		menu_ = std::make_unique<Menu>();
	}

	// setup callbacks
	bool callbacks_set = false;
	if (!callbacks_set)
	{
		window_.SetResizeCallback(
			[this](int width, int height)
			{
				if (renderer_)
					renderer_->Resize(width, height);
			});

		callbacks_set = true;
	}

	window_.Show(show_cmd);

	bool running = true;
	while (running)
	{
		running = window_.Run();

		renderer_->BeginFrame();

		menu_->Render();

		renderer_->EndFrame();
	}

	return 0;
}
