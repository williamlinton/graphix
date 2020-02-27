#include "D3DRenderer.h"

void D3DRenderer::Init(HWND handle, int width, int height)
{
	_height = height;
	_width = width;

	IDXGIFactory* _dxgiFactory;
	HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_dxgiFactory);
	HRESULT adapterResult = _dxgiFactory->EnumAdapters(0, &_adapter);
	DXGI_ADAPTER_DESC desc;
	_adapter->GetDesc(&desc);

	unsigned int flags = 0;
	flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	flags |= D3D11_CREATE_DEVICE_DEBUG;
	//D3D_FEATURE_LEVEL levels = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL levels[1];
	levels[0] = D3D_FEATURE_LEVEL_11_0;
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
	HRESULT swapChainResult = D3D11CreateDeviceAndSwapChain(_adapter, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, 0, flags, levels, 1, D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, 0, &_deviceContext);

	ID3D11Texture2D* backBuffer;

	HRESULT getBufferResult = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

	D3D11_VIEWPORT viewport;
	viewport.Height = _height;
	viewport.MaxDepth = 1.0;
	viewport.MinDepth = 0;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = _width;
	
	_deviceContext->RSSetViewports(1, &viewport);

	//--create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	ID3D11Texture2D* depthBuffer;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	//texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	texDesc.Width = width;
	auto depthBufferResult = _device->CreateTexture2D(&texDesc, 0, &depthBuffer);

	auto dsvResult = _device->CreateDepthStencilView(depthBuffer, &dsvDesc, &_dsv);

	HRESULT rtvResult = _device->CreateRenderTargetView(backBuffer, nullptr, &_rtv);
	_deviceContext->OMSetRenderTargets(1, &_rtv, _dsv);

	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* rastState;
	_device->CreateRasterizerState(&rastDesc, &rastState);
	_deviceContext->RSSetState(rastState);

	//_square = new Square();
	//_square->Init(_device);
	/*_sphere = new Sphere();
	_sphere->Init(_device);*/
	_terrain = new Terrain();
	_terrain->Init(_device);

	int stop = 0;

}

void D3DRenderer::Render(Keyboard* keyboard)
{
	static int red = 0;
	red = rand() % 255;
	FLOAT color[4] = { 0, 0, 0, 255 };
	_deviceContext->ClearDepthStencilView(_dsv, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0, 0);
	_deviceContext->ClearRenderTargetView(_rtv, color);
	_terrain->Render(_deviceContext, keyboard);
	_swapChain->Present(0, 0);
}
