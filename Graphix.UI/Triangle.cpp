#include "Triangle.h"


Triangle::Triangle()
{
}

void Triangle::Init(ID3D11Device* device)
{
	_fr = new FileReader();
	std::vector<unsigned char> psBytes;
	std::vector<unsigned char> vsBytes;
	psBytes = _fr->ReadFile("C:\\Users\\wlinton\\Source\\Repos\\Graphix\\x64\\Debug\\PixelShader.cso");
	vsBytes = _fr->ReadFile("C:\\Users\\wlinton\\Source\\Repos\\Graphix\\x64\\Debug\\VertexShader.cso");
	HRESULT result = device->CreatePixelShader(&psBytes[0], psBytes.size(), 0, &_pixelShader);
	HRESULT result2 = device->CreateVertexShader(&vsBytes[0], vsBytes.size(), 0, &_vertexShader);

	std::vector<Vertex> vertx;
	std::vector<unsigned int> indx;

	Vertex v1;
	v1.Position = DirectX::XMFLOAT4(-1, -1, 5, 1);
	v1.Color = DirectX::XMFLOAT4(1, 0, 0, 1);

	Vertex v2;
	v2.Position = DirectX::XMFLOAT4(0, 1, 5, 1);
	v2.Color = DirectX::XMFLOAT4(0, 1, 0, 1);

	Vertex v3;
	v3.Position = DirectX::XMFLOAT4(1, -1, 5, 1);
	v3.Color = DirectX::XMFLOAT4(0, 0, 1, 1);

	vertx.push_back(v1);
	vertx.push_back(v2);
	vertx.push_back(v3);

	indx.push_back(0);
	indx.push_back(1);
	indx.push_back(2);

	D3D11_BUFFER_DESC descVertex;
	descVertex.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	descVertex.ByteWidth = sizeof(Vertex) * vertx.size();
	descVertex.CPUAccessFlags = 0;
	descVertex.MiscFlags = 0;
	descVertex.StructureByteStride = 0;
	descVertex.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	D3D11_BUFFER_DESC descIndex;
	descIndex.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	descIndex.ByteWidth = sizeof(unsigned int) * indx.size();
	descIndex.CPUAccessFlags = 0;
	descIndex.MiscFlags = 0;
	descIndex.StructureByteStride = 0;
	descIndex.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	vertexData.pSysMem = &vertx[0];
	result = device->CreateBuffer(&descVertex, &vertexData, &_vertexBuffer);

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	indexData.pSysMem = &indx[0];
	result = device->CreateBuffer(&descIndex, &indexData, &_indexBuffer);

	D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	inputLayout[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[0].InputSlot = 0;
	inputLayout[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[0].InstanceDataStepRate = 0;
	inputLayout[0].SemanticIndex = 0;
	inputLayout[0].SemanticName = "POSITION";

	inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[1].InputSlot = 0;
	inputLayout[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[1].InstanceDataStepRate = 0;
	inputLayout[1].SemanticIndex = 0;
	inputLayout[1].SemanticName = "COLOR";

	result = device->CreateInputLayout(inputLayout, 2, &vsBytes[0], vsBytes.size(), &_inputLayout);

	D3D11_RASTERIZER_DESC rastDesc;
	rastDesc.AntialiasedLineEnable = true;
	rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 1.0;
	rastDesc.DepthClipEnable = false;
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = false;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0;

	auto rastResult = device->CreateRasterizerState(&rastDesc, &_rasterizerState);

	int stop = 9000;
}

void Triangle::Render(ID3D11DeviceContext* context)
{
	context->PSSetShader(_pixelShader, 0, 0);
	context->VSSetShader(_vertexShader, 0, 0);
	context->IASetInputLayout(_inputLayout);
	context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->RSSetState(_rasterizerState);
	context->DrawIndexed(3, 0, 0);

	//D3D11_MAPPED_SUBRESOURCE vertexData;
	//context->Map(_vertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &vertexData);

	//context->Unmap(_vertexBuffer, 0);
}
