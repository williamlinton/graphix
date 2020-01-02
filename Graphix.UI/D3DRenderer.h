#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

class D3DRenderer
{
public:
	void Init(HWND handle, int width, int height);
	void Render();
private:
	HWND _handle;
	int _width;
	int _height;
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _rtv;
};

