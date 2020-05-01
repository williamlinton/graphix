#pragma once
#include <DirectXMath.h>
#include <vector>

struct Vertex
{
	DirectX::XMFLOAT4 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT4 Normal;
};

struct CBPerEntity
{
	DirectX::XMFLOAT4X4 ModelToWorld;
	DirectX::XMFLOAT4X4 WorldToCamera;
	DirectX::XMFLOAT4X4 CameraToProjection;
};

struct Bitmap
{
	int Width;
	int Height;
	std::vector<DirectX::XMFLOAT4> Pixels;
};
