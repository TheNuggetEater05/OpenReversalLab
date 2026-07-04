#include <Windows.h>

#include "Application/Application.h"

int WINAPI wWinMain(
	_In_ HINSTANCE instance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int show_cmd
)
{
	Application app(instance, L"OpenReversalLab", L"ORLClass");

	try
	{
		app.Run(show_cmd);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_OK);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "Unknown exception", "Error", MB_OK);
	}

	return 0;
}