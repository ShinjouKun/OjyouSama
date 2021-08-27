#pragma once

#include "../Sound3DCone.h"
#include "../../../Math/Math.h"

//3D�T�E���h�ň������X�i�[�\����
struct Sound3DListenerParam
{
	//�O���x�N�g��
	Vector3 orientFront;
	//��x�N�g��
	//orientFront�ƒ��s���Ă���K�v������
	Vector3 orientTop;
	//���X�i�[�̈ʒu
	Vector3 position;
	//�h�b�v���[�v�Z�Ɏg�p����鑬�x�x�N�g��
	Vector3 velocity;
	//�w�����̎w��
	//NULL���Ɩ��w��(�R�[���������K�p����Ȃ�)
	Sound3DCone* cone;

	Sound3DListenerParam() :
		orientFront(Vector3::forward),
		orientTop(Vector3::up),
		position(Vector3::zero),
		velocity(Vector3::zero),
		cone(nullptr)
	{
	}
};