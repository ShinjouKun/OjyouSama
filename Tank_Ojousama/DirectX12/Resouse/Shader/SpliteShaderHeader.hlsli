cbuffer cbuff0:register(b0)
{
	float4 col;//RGBA
	matrix mat;//3D���W�ϊ�
};

//���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂����Ɏg�p
struct VSOutput
{
	float4 svpos : SV_POSITION;//�V�X�e���p���_���W
	float2 uv:TEXCOORD;//uv�l
};