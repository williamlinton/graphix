#include "D3DRenderer.h"

void D3DRenderer::Init(HWND handle, int width, int height)
{
	IDXGIFactory* _dxgiFactory;
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_dxgiFactory);
	IDXGIAdapter* adapter;
	HRESULT adapterResult = _dxgiFactory->EnumAdapters(0, &adapter);
	DXGI_ADAPTER_DESC desc;
	adapter->GetDesc(&desc);

	unsigned int flags = 0;
	flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	flags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL levels = { D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 };
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_STRETCHED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = handle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;

	//ZeroMemory((void *)&swapChainDesc, sizeof(swapChainDesc));
	HRESULT swapChainResult = D3D11CreateDeviceAndSwapChain(adapter, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, 0, flags, &levels, 1, D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, 0, &_deviceContext);

	ID3D11Texture2D* backBuffer;

	HRESULT getBufferResult = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

	HRESULT rtvResult = _device->CreateRenderTargetView(backBuffer, 0, &_rtv);
	int stop = 0;

}

void D3DRenderer::Render()
{
	static int red = 0;
	red = rand() % 255;
	FLOAT color[4] = { red, 0, 0, 255 };
	_deviceContext->ClearRenderTargetView(_rtv, color);
	_swapChain->Present(0, 0);
}
