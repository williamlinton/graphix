#include <DirectXMath.h>
#include "GraphicsUtil.h"

float radians(float degrees)
{
	return (DirectX::XM_PI / 180) * degrees;
}

float degrees(float radians)
{
	return (180 / DirectX::XM_PI) * radians;
}
