#include "Common.hlsli"

float4 main(VertexOutput input) : SV_TARGET
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	return input.Color;
	//return input.Position;
}