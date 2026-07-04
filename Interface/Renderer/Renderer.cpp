#include "Renderer.h"

#include <stdexcept>

#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui.h>

Renderer::Renderer(HWND hwnd, int buffer_width, int buffer_height)
{
	CreateDeviceAndSwapChain(hwnd, buffer_width, buffer_height);
	CreateRenderTarget();
	CreateViewport(static_cast<float>(buffer_width), static_cast<float>(buffer_height));

	// ImGui initialization
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device_, context_);
}

Renderer::~Renderer()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	device_->Release();
	context_->Release();
	swapchain_->Release();
	rtv_->Release();
}

bool Renderer::CreateDeviceAndSwapChain(HWND hwnd, int buffer_width, int buffer_height)
{
	DXGI_SWAP_CHAIN_DESC swap_desc{};
	swap_desc.BufferCount = 1;
	swap_desc.BufferDesc.Width = buffer_width;
	swap_desc.BufferDesc.Height = buffer_height;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.OutputWindow = hwnd;
	swap_desc.SampleDesc.Count = 1;
	swap_desc.SampleDesc.Quality = 0;
	swap_desc.Windowed = true;

	D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
	int feature_levels_count = 1;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&feature_levels,
		feature_levels_count,
		D3D11_SDK_VERSION,
		&swap_desc,
		&swapchain_,
		&device_,
		nullptr,
		&context_)))
	{
		throw std::exception("Renderer: Failed to create device and swap chain.");
		return false;
	}

	return true;
}

bool Renderer::CreateRenderTarget()
{
	ID3D11Texture2D* back_buffer{};
	hr_ = swapchain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));

	if (FAILED(hr_) || back_buffer == nullptr)
	{
		throw std::exception("Renderer: Failed to get back buffer");
		return false;
	}

	device_->CreateRenderTargetView(back_buffer, nullptr, &rtv_);

	back_buffer->Release();

	context_->OMSetRenderTargets(1, &rtv_, nullptr);
}

void Renderer::CreateViewport(float width, float height, float top_left_x, float top_left_y)
{
	viewport_.Width = width;
	viewport_.Height = height;
	viewport_.TopLeftX = top_left_x;
	viewport_.TopLeftY = top_left_y;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	context_->RSSetViewports(1, &viewport_);
}

void Renderer::BeginFrame()
{
	context_->OMSetRenderTargets(1, &rtv_, nullptr);
	context_->RSSetViewports(1, &viewport_);

	Clear(0.1f, 0.1f, 0.1f, 1.0f);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Renderer::Clear(float r, float g, float b, float a)
{
	float clear_color[4] = { r, g, b, a };
	context_->ClearRenderTargetView(rtv_, clear_color);
}

void Renderer::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	swapchain_->Present(1, 0);
}

void Renderer::Resize(int width, int height)
{
	if (!swapchain_ || !device_ || !context_)
		return;

	context_->OMSetRenderTargets(0, nullptr, nullptr);

	if (rtv_)
	{
		rtv_->Release();
		rtv_ = nullptr;
	}

	hr_ = swapchain_->ResizeBuffers(0,
		width, height,
		DXGI_FORMAT_UNKNOWN,
		0);

	if (FAILED(hr_))
	{
		throw std::exception("Renderer: Failed to resize buffers.");
		return;
	}

	ID3D11Texture2D* back_buffer{};
	hr_ = swapchain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	if (FAILED(hr_) || back_buffer == nullptr)
	{
		throw std::exception("Renderer: Failed to get back buffer");
		return;
	}

	device_->CreateRenderTargetView(back_buffer, nullptr, &rtv_);
	back_buffer->Release();

	viewport_.Width = static_cast<float>(width);
	viewport_.Height = static_cast<float>(height);

	context_->RSSetViewports(1, &viewport_);
}
