#include "Square.h"

Square::Square()
{
}

void Square::Init(ID3D11Device* device)
{
	_fr = new FileReader();
	std::vector<unsigned char> psBytes;
	std::vector<unsigned char> vsBytes;
	psBytes = _fr->ReadFile(GetShaderPath("PixelShader.cso"));
	vsBytes = _fr->ReadFile(GetShaderPath("VertexShader.cso"));
	HRESULT result = device->CreatePixelShader(&psBytes[0], psBytes.size(), 0, &_pixelShader);
	HRESULT result2 = device->CreateVertexShader(&vsBytes[0], vsBytes.size(), 0, &_vertexShader);

	// Matrix
	_x = 0;
	_y = 0;
	_z = 0;

	_roll = 0;
	_pitch = 0;
	_yaw = 0;

	// Vertices
	float x = 1;
	float y = 1.0;
	float z = 1.5;

	Vertex v1;
	v1.Position = DirectX::XMFLOAT4(-x - 0.2, y, z, 1);
	v1.Color = DirectX::XMFLOAT4(1, 0, 0, 1);

	Vertex v2;
	v2.Position = DirectX::XMFLOAT4(x - 0.2, y, z, 1);
	v2.Color = DirectX::XMFLOAT4(0, 1, 0, 1);

	Vertex v3;
	v3.Position = DirectX::XMFLOAT4(x - 0.2, -y, z, 1);
	v3.Color = DirectX::XMFLOAT4(0, 0, 1, 1);

	Vertex v4;
	v4.Position = DirectX::XMFLOAT4(-x - 0.2, -y, z, 1);
	v4.Color = DirectX::XMFLOAT4(0, 0, 1, 1);

	_vertx.push_back(v1);
	_vertx.push_back(v2);
	_vertx.push_back(v3);
	_vertx.push_back(v4);

	_indx.push_back(0);
	_indx.push_back(2);
	_indx.push_back(3);

	_indx.push_back(0);
	_indx.push_back(1);
	_indx.push_back(2);

	D3D11_BUFFER_DESC descVertex;
	descVertex.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	descVertex.ByteWidth = sizeof(Vertex) * _vertx.size();
	descVertex.CPUAccessFlags = 0;
	descVertex.MiscFlags = 0;
	descVertex.StructureByteStride = 0;
	descVertex.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	D3D11_BUFFER_DESC descIndex;
	descIndex.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	descIndex.ByteWidth = sizeof(unsigned int) * _indx.size();
	descIndex.CPUAccessFlags = 0;
	descIndex.MiscFlags = 0;
	descIndex.StructureByteStride = 0;
	descIndex.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	D3D11_BUFFER_DESC descConstant;
	descConstant.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	descConstant.ByteWidth = sizeof(CBPerEntity);
	descConstant.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	descConstant.MiscFlags = 0;
	descConstant.StructureByteStride = 0;
	descConstant.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	vertexData.pSysMem = &_vertx[0];
	result = device->CreateBuffer(&descVertex, &vertexData, &_vertexBuffer);

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	indexData.pSysMem = &_indx[0];
	result = device->CreateBuffer(&descIndex, &indexData, &_indexBuffer);

	result = device->CreateBuffer(&descConstant, 0, &_constantBuffer);


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
	rastDesc.DepthClipEnable = true;
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = false;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0;

	auto rastResult = device->CreateRasterizerState(&rastDesc, &_rasterizerState);

	auto modelToWorld = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&_modelToWorld, modelToWorld);

	auto worldToCamera = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&_worldToCamera, worldToCamera);

	auto cameraToProjection = DirectX::XMMatrixIdentity();
	//DirectX::XMStoreFloat4x4(&_cameraToProjection, cameraToProjection);
	auto projMatrix = DirectX::XMMatrixPerspectiveFovLH(radians(45), 800.0 / 600.0, 1, 1000);
	DirectX::XMStoreFloat4x4(&_cameraToProjection, projMatrix);
	

	int stop = 9000;

}

void Square::Render(ID3D11DeviceContext* context, Keyboard* keyboard)
{
	if (keyboard->IsKeyDown('W')) {
		_z += 0.01;
	}
	if (keyboard->IsKeyDown('S')) {
		_z -= 0.01;
	}
	if (keyboard->IsKeyDown('A')) {
		_x -= 0.01;
	}
	if (keyboard->IsKeyDown('D')) {
		_x += 0.01;
	}
	if (keyboard->IsKeyDown('T')) {
		_y += 0.01;
	}
	if (keyboard->IsKeyDown('G')) {
		_y -= 0.01;
	}
	if (keyboard->IsKeyDown('R')) {
		_x = 0;
		_y = 0;
		_z = 0;
	}
	if (keyboard->IsKeyDown('I')) {
		_yaw += 0.001;
	}
	auto matrix = DirectX::XMMatrixTranslation(_x, _y, _z);
	auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(_pitch, _yaw, _roll);
	auto finalMatrix = rotationMatrix * matrix;
	DirectX::XMStoreFloat4x4(&_modelToWorld, finalMatrix);

	context->IASetInputLayout(_inputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->PSSetShader(_pixelShader, 0, 0);
	context->VSSetShader(_vertexShader, 0, 0);

	context->RSSetState(_rasterizerState);
	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;
	context->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	D3D11_MAPPED_SUBRESOURCE subresource;

	context->Map(_constantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	CBPerEntity* cbperentity = (CBPerEntity*)subresource.pData;
	cbperentity->ModelToWorld = _modelToWorld;
	cbperentity->WorldToCamera = _worldToCamera;
	cbperentity->CameraToProjection = _cameraToProjection;
	context->Unmap(_constantBuffer, 0);

	context->VSSetConstantBuffers(0, 1, &_constantBuffer);
	context->DrawIndexed(_indx.size(), 0, 0);

	//D3D11_MAPPED_SUBRESOURCE vertexData;
	//context->Map(_vertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &vertexData);

	//context->Unmap(_vertexBuffer, 0);
}
