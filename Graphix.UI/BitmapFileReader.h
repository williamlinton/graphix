#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include "Structs.h"
class BitmapFileReader
{
public:
	Bitmap ReadFile(std::string path);
};

