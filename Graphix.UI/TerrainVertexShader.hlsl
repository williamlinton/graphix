#include "Common.hlsli"

TerrainVertexOutput main(TerrainVertexInput input)
{
	TerrainVertexOutput output;
	output.Position = mul(input.Position, ModelToWorld);
	output.Position = mul(output.Position, WorldToCamera);
	output.Position = mul(output.Position, CameraToProjection);
	output.Color = input.Color;
	output.Normal = normalize(input.Normal);
	return output;
}