struct VertexInput
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct VertexOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

cbuffer CBPerEntity : register(b0)
{
	row_major matrix ModelToWorld;
	row_major matrix WorldToCamera;
	row_major matrix CameraToProjection;
};