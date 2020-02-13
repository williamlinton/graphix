#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
class BitmapFileReader
{
public:
	std::vector<DirectX::XMFLOAT4> ReadFile(std::string path);
};

