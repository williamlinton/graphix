#include "Terrain.h"

Terrain::Terrain()
{
}

void Terrain::Init(ID3D11Device* device)
{
	_fr = new FileReader();
	std::vector<unsigned char> psBytes;
	std::vector<unsigned char> vsBytes;
	psBytes = _fr->ReadFile(GetShaderPath("PixelShader.cso"));
	vsBytes = _fr->ReadFile(GetShaderPath("VertexShader.cso"));
	HRESULT result = device->CreatePixelShader(&psBytes[0], psBytes.size(), 0, &_pixelShader);
	HRESULT result2 = device->CreateVertexShader(&vsBytes[0], vsBytes.size(), 0, &_vertexShader);

	_bfr = new BitmapFileReader();
	Bitmap bitmap = _bfr->ReadFile(GetResourcePath("terrain.bmp"));

	// Matrix
	_x = 0;
	_y = 0;
	_z = 0;

	_roll = 0;
	_pitch = 0;
	_yaw = 0;

#if 1
	int nodeCountX = 128;
	int nodeCountZ = 128;

	double width = 100.0;
	double depth = 100.0;
	double height = 10.0;

	std::vector<DirectX::XMFLOAT4> normalSumByVertex;
	std::vector<int> faceCountByVertex;

	for (int d = 0; d < nodeCountZ; d++)
	{
		for (int w = 0; w < nodeCountX; w++)
		{
			int row = (double)d / (double)nodeCountZ * (double)bitmap.Height;
			int col = (double)w / (double)nodeCountX * (double)bitmap.Width;

			auto heightColor = bitmap.Pixels[col + row * bitmap.Width];

			double x = (double)w / (double)nodeCountX * width - width / 2.0;
			double z = (double)d / (double)nodeCountZ * depth ;
			double y = (double)heightColor.x / 256.0 * height - height / 2.0;
			Vertex vertex;
			vertex.Color = DirectX::XMFLOAT4(255, 255, 255, 255);
			vertex.Position = DirectX::XMFLOAT4(x, y, z, 1);
			_vertx.push_back(vertex);
		}
	}

	normalSumByVertex.resize(_vertx.size());
	faceCountByVertex.resize(_vertx.size());

	for (int d = 0; d < nodeCountZ - 1; d++)
	{
		for (int w = 0; w < nodeCountX - 1; w++)
		{
			_indx.push_back(w + d * nodeCountX);
			_indx.push_back(w + 1 + (d + 1) * nodeCountX);
			_indx.push_back(w + 1 + d * nodeCountX);

			_indx.push_back(w + 1 + (d + 1) * nodeCountX);
			_indx.push_back(w + d * nodeCountX);
			_indx.push_back(w + (d + 1) * nodeCountX);
		}
	}

	std::vector<DirectX::XMFLOAT4> normals;
	int count = _indx.size();
	int n = 0;
	while (n < count - 2)
	{
		double v1x = _vertx[_indx[n]].Position.x;
		double v1y = _vertx[_indx[n]].Position.y;
		double v1z = _vertx[_indx[n]].Position.z;

		double v2x = _vertx[_indx[n + 1]].Position.x;
		double v2y = _vertx[_indx[n + 1]].Position.y;
		double v2z = _vertx[_indx[n + 1]].Position.z;

		double v3x = _vertx[_indx[n + 2]].Position.x;
		double v3y = _vertx[_indx[n + 2]].Position.y;
		double v3z = _vertx[_indx[n + 2]].Position.z;

		double vec1x = v1x - v3x;
		double vec1y = v1y - v3y;
		double vec1z = v1z - v3z;

		double vec2x = v3x - v2x;
		double vec2y = v3y - v2y;
		double vec2z = v3z - v2z;

		double normalx = (vec1y * vec2z) - (vec1z * vec2y);
		double normaly = (vec1z * vec2x) - (vec1x * vec2z);
		double normalz = (vec1x * vec2y) - (vec1y * vec2x);

		normalSumByVertex[_indx[n]].x += normalx;
		normalSumByVertex[_indx[n]].y += normaly;
		normalSumByVertex[_indx[n]].z += normalz;
		faceCountByVertex[_indx[n]] += 1;

		normalSumByVertex[_indx[n + 1]].x += normalx;
		normalSumByVertex[_indx[n + 1]].y += normaly;
		normalSumByVertex[_indx[n + 1]].z += normalz;
		faceCountByVertex[_indx[n + 1]] += 1;

		normalSumByVertex[_indx[n + 2]].x += normalx;
		normalSumByVertex[_indx[n + 2]].y += normaly;
		normalSumByVertex[_indx[n + 2]].z += normalz;
		faceCountByVertex[_indx[n + 2]] += 1;

		normals.push_back(DirectX::XMFLOAT4(normalx, normaly, normalz, 1));

		n += 3;
	}

	normals.clear();

	for (int i = 0; i < _vertx.size(); i++)
	{
		_vertx[i].Normal = DirectX::XMFLOAT4(
			normalSumByVertex[i].x / faceCountByVertex[i],
			normalSumByVertex[i].y / faceCountByVertex[i],
			normalSumByVertex[i].z / faceCountByVertex[i],
			1
		);
	}


#else
	for (int h = 0; h < bitmap.Height; h++)
	{
		for (int w = 0; w < bitmap.Width; w++)
		{
			double x = w - (bitmap.Width / 2);
			auto heightColor = bitmap.Pixels[w + h * bitmap.Width];
			double y = heightColor.x / 256.0 * 10.0 - 10.0 / 2;
			double z = h;
			Vertex vertex;
			vertex.Color = DirectX::XMFLOAT4(255, 255, 255, 255);
			vertex.Position = DirectX::XMFLOAT4(x, y, z, 1);
			_vertx.push_back(vertex);
		}
	}

	for (int h = 0; h < bitmap.Height - 1; h++)
	{
		for (int w = 0; w < bitmap.Width - 1; w++)
		{
			_indx.push_back(w + h * bitmap.Width);
			_indx.push_back(w + 1 + (h + 1) * bitmap.Width);
			_indx.push_back(w + 1 + h * bitmap.Width);

			_indx.push_back(w + 1 + (h + 1) * bitmap.Width);
			_indx.push_back(w + h * bitmap.Width);
			_indx.push_back(w + (h + 1) * bitmap.Width);
		}
	}
#endif

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
	rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 1.0;
	rastDesc.DepthClipEnable = true;
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
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
	
	_camera = new Camera();
	_camera->SetPosition(_x, _y, _z);

	int stop = 9000;

}

void Terrain::Render(ID3D11DeviceContext* context, Keyboard* keyboard)
{
	float x, y, z;
	x = y = z = 0;
	float pitch, yaw, roll;
	if (keyboard->IsKeyDown('W')) {
		z += 0.01;
	}
	if (keyboard->IsKeyDown('S')) {
		z -= 0.01;
	}
	if (keyboard->IsKeyDown('A')) {
		x -= 0.01;
	}
	if (keyboard->IsKeyDown('D')) {
		x += 0.01;
	}
	if (keyboard->IsKeyDown('Q')) {
		y += 0.01;
	}
	if (keyboard->IsKeyDown('E')) {
		y -= 0.01;
	}
	if (keyboard->IsKeyDown('R')) {
		_camera->SetPosition(0, 0, 0);
		_yaw = 0;
		_pitch = 0;
		_roll = 0;
	}
	if (keyboard->IsKeyDown('J')) {
		_yaw -= 0.1;
	}
	if (keyboard->IsKeyDown('L')) {
		_yaw += 0.1;
	}
	if (keyboard->IsKeyDown('I')) {
		_pitch += 0.1;
	}
	if (keyboard->IsKeyDown('K')) {
		_pitch -= 0.1;
	}
	if (keyboard->IsKeyDown('U')) {
		_roll -= 0.1;
	}
	if (keyboard->IsKeyDown('O')) {
		_roll += 0.1;
	}

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

	if (x != 0 || y != 0 || z != 0)
	{
		_camera->Travel(x, y, z);
	}
	_camera->SetRotation(_pitch, _yaw, _roll);
	_camera->Render();
	DirectX::XMMATRIX viewMatrix;
	_camera->GetViewMatrix(&viewMatrix);
	DirectX::XMStoreFloat4x4(&_worldToCamera, viewMatrix);

	cbperentity->ModelToWorld = _modelToWorld;
	//auto matrix = DirectX::XMMatrixTranslation(_x, _y, _z);
	//auto rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(_pitch, _yaw, _roll);
	//auto finalMatrix = matrix * rotationMatrix ;
	//DirectX::XMStoreFloat4x4(&_worldToCamera, finalMatrix);
	cbperentity->WorldToCamera = _worldToCamera;
	cbperentity->CameraToProjection = _cameraToProjection;
	context->Unmap(_constantBuffer, 0);

	context->VSSetConstantBuffers(0, 1, &_constantBuffer);
	context->DrawIndexed(_indx.size(), 0, 0);

	//D3D11_MAPPED_SUBRESOURCE vertexData;
	//context->Map(_vertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &vertexData);

	//context->Unmap(_vertexBuffer, 0);
}
