#include "Common.hlsli"

VertexOutput main(VertexInput input)
{
	VertexOutput output;
	output.Position = mul(input.Position, ModelToWorld);
	output.Position = mul(output.Position, WorldToCamera);
	output.Position = mul(output.Position, CameraToProjection);
	output.Color = input.Color;
	return output;
}