#include "PostEfeectShader.hlsli"
float4 PSmain(Output input):SV_TARGET
{
	
	//float4 color = tex.Sample(smp,input.uv);
	//float w, h, l;
	//tex.GetDimensions(0, w, h, l);
	float4 color;
	//float gray = color.r*0.229 + color.g*0.587 + color.b*0.114;
	//float sepia = 0.06f;
	//RGBƒVƒtƒg
	color.r = tex.Sample(smp, input.uv+0.002f).r;
	color.g = tex.Sample(smp, input.uv).g;
	color.b = tex.Sample(smp, input.uv-0.002f).b;
	color.a = 1.0f;
	//return float4(gray+sepia,gray,gray-sepia,1);
	return float4(color.rgb -fmod(color.rgb,0.20f),1);
	//return color;
}