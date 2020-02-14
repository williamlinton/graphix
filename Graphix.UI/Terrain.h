#pragma once
#include <d3d11.h>
#include "FileReader.h"
#include "BitmapFileReader.h"
#include "Structs.h"
#include "GraphicsUtil.h"
#include "Keyboard.h"
class Terrain
{
public:
	Terrain();
	void Init(ID3D11Device* device);
	void Render(ID3D11DeviceContext* context, Keyboard* keyboard);
private:
	FileReader* _fr;
	BitmapFileReader* _bfr;
	ID3D11PixelShader* _pixelShader;
	ID3D11VertexShader* _vertexShader;
	ID3D11Buffer* _indexBuffer;
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _constantBuffer;
	ID3D11InputLayout* _inputLayout;
	ID3D11RasterizerState* _rasterizerState;
	std::vector<Vertex> _vertx;
	std::vector<unsigned int> _indx;

	DirectX::XMFLOAT4X4 _modelToWorld;
	DirectX::XMFLOAT4X4 _worldToCamera;
	DirectX::XMFLOAT4X4 _cameraToProjection;

	float _x;
	float _y;
	float _z;

	float _roll;
	float _pitch;
	float _yaw;

};

