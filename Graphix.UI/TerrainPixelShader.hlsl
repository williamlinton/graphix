#include "Common.hlsli"

float4 main(TerrainVertexOutput input) : SV_TARGET
{
	float4 color;
	float intensity;

	color = input.Color;
	intensity = saturate(dot(input.Normal, DiffuseLightDirection));

	if (intensity > 0)
	{
		color += DiffuseLightColor * intensity;
	}

	color = saturate(color);
	return color;
}