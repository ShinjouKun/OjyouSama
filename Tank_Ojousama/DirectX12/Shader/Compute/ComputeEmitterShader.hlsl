#include "ComputeShader.hlsli"

//エミッターの情報からパーティクルを生成する

StructuredBuffer<EmitterDatas> input : register(t0);//入力情報
RWStructuredBuffer<ParticleUpdateDatas> real : register(u0);//出力情報

[numthreads(1, 1, 1)]
void CS( uint3 gID : SV_DispatchThreadID )
{
	
	//エミッターは1つしかないから
	EmitterDatas eData = input[0];//input[gID.x]
	ParticleUpdateDatas pData;

	pData.lifeTime = eData.StartLifeTime + rand(float2(eData.randomLife, eData.randomLife + 1));
	pData.pos = eData.pos;
	pData.vec = eData.StartVector;
	pData.temp1 = 0;
	pData.col = eData.StartColor + float4(rand(float2(eData.randomColor.r, eData.randomColor.r+1)), rand(float2(eData.randomColor.g, eData.randomColor.g+1)), rand(float2(eData.randomColor.b, eData.randomColor.b+1)), rand(float2(eData.randomColor.a, eData.randomColor.a+1)));
	pData.size = eData.StartSize3D + rand3(eData.StartSize3D.xy);
	pData.speed = eData.StartSpeed + rand(float2(eData.randomSpeed, eData.randomSpeed + 1));
	pData.rotate = eData.StartRotation3D;
	pData.gravity = eData.GravityModifier;
	pData.axis = eData.Axis;
	pData.active = false;

	real[gID.x] = pData;
}