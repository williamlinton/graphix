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

struct TerrainVertexInput
{
	float4 Position : POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
};

struct TerrainVertexOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float4 Normal : NORMAL;
};

cbuffer CBPerEntity : register(b0)
{
	row_major matrix ModelToWorld;
	row_major matrix WorldToCamera;
	row_major matrix CameraToProjection;
};

cbuffer CBLighting : register(b1)
{
	float4 DiffuseLightColor;
	float4 DiffuseLightDirection;
	float DiffuseLightIntensity;
	float Nothing1;
	float Nothing2;
	float Nothing3;
}