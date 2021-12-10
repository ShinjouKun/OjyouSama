#include "ComputeShader.hlsli"

//�l�p�`�̒��_��
static const uint vnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_arry[vnum] =
{
	float4(-0.5,-0.5,0,0),//����
	float4(-0.5,0.5,0,0), //����
	float4(0.5,-0.5,0,0), //�E��
	float4(0.5,0.5,0,0)	  //�E��
};

static const float2 uv_array[vnum] =
{
	float2(0,1),//����
	float2(0,0),//����
	float2(1,1),//�E��
	float2(1,0)//�E��
};

[maxvertexcount(vnum)]//�W�I���g���V�F�[�_�ŏo�͂���ő咷�_��
void GS(
	point VSOutput input[1],//���̓f�[�^�Q
	inout TriangleStream< GSOutput > output//�o�̓I�u�W�F�N�g(DataType)
)
{
	//����
	GSOutput o;

	for (uint i = 0; i < vnum; ++i)
	{
		//���S����I�t�Z�b�g���r���{�[�h��](���f�����W)
		float4 offset;
		//���S����I�t�Z�b�g���X�P�[�����O
		offset.x = offset_arry[i].x * input[0].size.x;
		offset.y = offset_arry[i].y * input[0].size.y;
		offset.z = 0;
		offset.w = 0;
		/*
		//��]�p�ϐ�
		float x = offset.x;
		float y = offset.y;
		float z = offset.z;

		//z����]
		offset.x = x * cos(input[0].rotate.z) - y * sin(input[0].rotate.z);
		offset.y = x * sin(input[0].rotate.z) + y * cos(input[0].rotate.z);

		//y����]
		offset.x += x * cos(input[0].rotate.y) + z * sin(input[0].rotate.y);
		offset.z += -x * sin(input[0].rotate.y) + z * cos(input[0].rotate.y);

		//x����]
		offset.y += y * cos(input[0].rotate.x) - z * sin(input[0].rotate.x);
		offset.z += y * sin(input[0].rotate.x) + z * cos(input[0].rotate.x);
		*/
		offset = mul(matBillboard, offset);
		//�I�t�Z�b�g�����炷(���[���h���W)
		o.pos = input[0].svpos + offset;
		//�r���[�v���W�F�N�V�����ϊ�
		o.pos = mul(mat, o.pos);
		o.color = input[0].color;
		o.uv = uv_array[i];
		output.Append(o);
	}
}