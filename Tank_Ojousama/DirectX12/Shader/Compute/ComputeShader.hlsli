
struct EmitterDatas
{
	float3 pos;
	float temp1;
	float StartSpeed;//�ŏ��̃X�s�[�h
	float3 StartVector;//����
	float StartLifeTime;//����
	float3 StartSize3D;//�ŏ��̑傫��
	float3 StartRotation3D;//�ŏ��̉�]
	float temp2;
	float4 StartColor;//�J�n���̐F
	float GravityModifier;//�d��
	//float Time;//�S�ďo���܂ł̎���
	//int Count;//1�t���[���ɏo����
	//int Cycles;//
	//float Interval;//�C���^�[�o��
	//float Probabillity;//
	float temp5;//
	float randomSpeed;//�����_���ȃX�s�[�h��ǉ�
	float randomLife;//�����_���Ȑ������Ԃ�ǉ�
	float3 randomSize3D;//�����_���ȃT�C�Y��ǉ�
	float temp3;
	float3 randomRotate3D;//�����_���ȉ�]��ǉ�
	float temp4;
	float4 randomColor;//�����_���ȃJ���[��ǉ�
	float angle;
	float3 Axis;
};

struct ParticleUpdateDatas
{
	float lifeTime;//����
	float3 pos;//�ʒu
	float3 vec;//�x�N�g��
	float temp1;//���������킹
	float4 col;//�F
	float3 size;//�T�C�Y
	float speed;//�X�s�[�h
	float3 rotate;//��]
	float gravity;//�d��
	float3 axis;//��
	bool active;//�����Ă��邩�ǂ���
};

struct ParticleDrawDatas
{
	float4 svpos;
	float4 color;
	float3 size;
	float3 rotate;
};


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

cbuffer cbuff0:register(b0)
{
	matrix mat;//3D�ϊ��s��
	matrix matBillboard;//�r���{�[�h�s��
};

float rand(float2 seed)
{
	//fract(sin(dot(float2(-x,x), float2(12.9898, 78.233))) * 43758.5453);
	return frac(sin(dot(seed, float2(12.9898, 78.233))) * 43758.5453);
}

float3 rand3(float2 seed)
{
	return 2.0 * (float3(rand(seed * 1), rand(seed * 2), rand(seed * 3)) - 0.5);
}