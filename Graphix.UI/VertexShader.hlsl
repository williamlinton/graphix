#include "Common.hlsli"

VertexOutput main(VertexInput input)
{
	VertexOutput output;
	output.Position = input.Position;
	output.Color = input.Color;
	return output;
	//return input.Color;
}