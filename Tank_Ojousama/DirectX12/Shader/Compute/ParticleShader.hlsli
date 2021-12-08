
cbuffer cbuff0 : register(b0)
{
	matrix mat;//3D�ϊ��s��
	matrix matBillboard;//�r���{�[�h�s��
};

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�ւ̂��Ƃ�
struct VSOutput
{
	float4 svpos: SV_POSITION;//�V�X�e���p���_���W
	float4 color : COLOR;//�J���[
	float3 size : TEXCOORD00;//�T�C�Y
	float3 rotate : TEXCOORD01;//��]
};

struct GSOutput
{
	float4 pos: SV_POSITION;//�V�X�e���p���_���W
	float4 color : COLOR;//�J���[
	float2 uv : TEXCOORD00;//uv�l
};