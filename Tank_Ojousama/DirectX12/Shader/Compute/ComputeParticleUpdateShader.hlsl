#include "ComputeShader.hlsli"

//パーティクルを更新する

StructuredBuffer<ParticleUpdateDatas> input : register(t0);//入力情報
RWStructuredBuffer<ParticleDrawDatas> real : register(u0);//出力情報
RWStructuredBuffer<ParticleUpdateDatas> real2 : register(u1);//出力情報
[numthreads(8, 1, 1)]
void CS( uint3 gID : SV_DispatchThreadID )
{
	ParticleUpdateDatas uData = input[gID.x];
	ParticleDrawDatas dData;

	//更新
	uData.lifeTime -= 1;
	uData.vec.y += uData.gravity;
	uData.pos += uData.vec * uData.speed;

	//削除どうする？


	//更新したデータを描画データに入れる
	dData.svpos = float4(uData.pos, 0);
	dData.color = uData.col;
	dData.size = uData.size;
	dData.rotate = uData.axis;

	real[gID.x] = dData;
	real2[gID.x] = uData;
}