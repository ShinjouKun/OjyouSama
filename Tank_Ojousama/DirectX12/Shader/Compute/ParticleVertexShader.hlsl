#include "ComputeShader.hlsli"

VSOutput VS(
	float4 pos : POSITION,
	float4 color : COLOR,
	float3 size : TEXCOORD00,
	float3 rotate : TEXCOORD01)
{
	VSOutput output;
	output.svpos = pos;
	output.color = color;
	output.size = size;
	output.rotate = rotate;
	return output;
}