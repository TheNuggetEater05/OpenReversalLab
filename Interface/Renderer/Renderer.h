#ifndef INTERFACE_RENDERER_RENDERER_H_
#define INTERFACE_RENDERER_RENDERER_H_

// If needed in future, Create IRenderer interface to support other frameworks than D3D11
#include <Windows.h>

#include <d3d11.h>

class Renderer
{
public:
	Renderer(HWND hwnd, int buffer_width, int buffer_height);
	~Renderer();

	void BeginFrame();
	void EndFrame();
	void Resize(int width, int height);

private:
	bool CreateDeviceAndSwapChain(HWND hwnd, int buffer_width, int buffer_height);
	bool CreateRenderTarget();
	void CreateViewport(float width, float height, float top_left_x = 0.0f, float top_left_y = 0.0f);

	void Clear(float r, float g, float b, float a);

private:
	ID3D11Device* device_{};
	IDXGISwapChain* swapchain_{};
	ID3D11DeviceContext* context_{};
	ID3D11RenderTargetView* rtv_{};
	D3D11_VIEWPORT viewport_{};

	HRESULT hr_{};
};

#endif // INTERFACE_RENDERER_RENDERER_H_
