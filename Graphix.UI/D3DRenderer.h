#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include "Triangle.h"
#include "Square.h"
#include "Keyboard.h"
#include "Sphere.h"
#include "Terrain.h"

class D3DRenderer
{
public:
	void Init(HWND handle, int width, int height);
	void Render(Keyboard* keyboard);
private:
	HWND _handle;
	int _width;
	int _height;
	IDXGIAdapter* _adapter;
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _rtv;
	//Triangle* _triangle;
	Square* _square;
	Sphere* _sphere;
	Terrain* _terrain;
	ID3D11DepthStencilView* _dsv;
};

