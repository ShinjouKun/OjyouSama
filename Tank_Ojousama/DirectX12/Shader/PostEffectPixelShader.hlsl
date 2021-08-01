#include "PostEfeectShader.hlsli"
float4 PSmain(Output input):SV_TARGET
{
	float4 color = tex.Sample(smp,input.uv);
	float gray = color.r*0.229 + color.g*0.587 + color.b*0.114;
	float sepia = 0.06f;
	//return float4(gray+sepia,gray,gray-sepia,1);
	return color;
}