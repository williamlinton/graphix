#pragma once
#include <DirectXMath.h>
#include "GraphicsUtil.h"
class Camera
{
public:
	void SetPosition(float x, float y, float z);
	void Travel(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void Render();
	void GetViewMatrix(DirectX::XMMATRIX* viewMatrix);
private:
	float _positionX;
	float _positionY;
	float _positionZ;

	float _rotationX;
	float _rotationY;
	float _rotationZ;

	DirectX::XMMATRIX _viewMatrix;
};

