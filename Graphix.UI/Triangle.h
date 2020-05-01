#pragma once
#include <d3d11.h>
#include "FileReader.h"
#include "Structs.h"
#include "Settings.h"
class Triangle
{
public:
	Triangle();
	void Init(ID3D11Device* device);
	void Render(ID3D11DeviceContext* context);
private:
	FileReader* _fr;
	ID3D11PixelShader* _pixelShader;
	ID3D11VertexShader* _vertexShader;
	ID3D11Buffer* _indexBuffer;
	ID3D11Buffer* _vertexBuffer;
	ID3D11InputLayout* _inputLayout;
	ID3D11RasterizerState* _rasterizerState;
};

