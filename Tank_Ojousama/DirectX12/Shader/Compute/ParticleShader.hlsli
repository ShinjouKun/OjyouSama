
cbuffer cbuff0 : register(b0)
{
	matrix mat;//3D変換行列
	matrix matBillboard;//ビルボード行列
};

//頂点シェーダーからピクセルシェーダへのやりとり
struct VSOutput
{
	float4 svpos: SV_POSITION;//システム用頂点座標
	float4 color : COLOR;//カラー
	float3 size : TEXCOORD00;//サイズ
	float3 rotate : TEXCOORD01;//回転
};

struct GSOutput
{
	float4 pos: SV_POSITION;//システム用頂点座標
	float4 color : COLOR;//カラー
	float2 uv : TEXCOORD00;//uv値
};