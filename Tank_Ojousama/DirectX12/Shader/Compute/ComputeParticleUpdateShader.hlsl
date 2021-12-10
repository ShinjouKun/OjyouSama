#include "ComputeShader.hlsli"

//�p�[�e�B�N�����X�V����

StructuredBuffer<ParticleUpdateDatas> input : register(t0);//���͏��
RWStructuredBuffer<ParticleDrawDatas> real : register(u0);//�o�͏��
RWStructuredBuffer<ParticleUpdateDatas> real2 : register(u1);//�o�͏��
[numthreads(8, 1, 1)]
void CS( uint3 gID : SV_DispatchThreadID )
{
	ParticleUpdateDatas uData = input[gID.x];
	ParticleDrawDatas dData;

	//�X�V
	uData.lifeTime -= 1;
	uData.vec.y += uData.gravity;
	uData.pos += uData.vec * uData.speed;

	//�폜�ǂ�����H
	uData.active = (uData.lifeTime <= 0) ? false : true;

	//�X�V�����f�[�^��`��f�[�^�ɓ����
	dData.pos = uData.pos;
	dData.temp = 0;
	dData.color = uData.col;
	dData.size = uData.size;
	dData.temp2 = 0;
	dData.rotate = uData.axis;
	dData.temp3 = 0;

	real[gID.x] = dData;
	real2[gID.x] = uData;
}