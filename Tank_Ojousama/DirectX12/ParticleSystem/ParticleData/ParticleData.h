#pragma once
#include "../../Math/Math.h"

//�G�~�b�^�[�̍X�V�ɕK�v�ȃf�[�^(�p�[�e�B�N�������f�[�^)
struct EmitterData
{
	Vector3 Pos;
	Vector3 Vec;
	float StartLifeTime;
	float StartSpeed;//�ŏ��̃X�s�[�h
	Vector3 StartVector;//����
	Vector3 StartSize3D;//�ŏ��̑傫��
	Vector3 StartRotation3D;//�ŏ��̉�]
	Vector4 StartColor;//�J�n���̐F
	float GravityModifier;//�d��
	//float Time;//�S�ďo���܂ł̎���
	//int Count;//1�t���[���ɏo����
	//int Cycles;//
	//float Interval;//�C���^�[�o��
	//float Probabillity;//
	float randomSpeed;//�����_���ȃX�s�[�h��ǉ�
	float randomLife;//�����_���Ȑ������Ԃ�ǉ�
	Vector3 randomSize3D;//�����_���ȃT�C�Y��ǉ�
	Vector3 randomRotate3D;//�����_���ȉ�]��ǉ�
	Vector4 randomColor;//�����_���ȃJ���[��ǉ�
	float Angle;
	Vector3 Axis;
};

//�p�[�e�B�N���̍X�V�ɕK�v�ȏ��
struct ParticleData
{
	float lifeTime;//����
	Vector3 pos;//�ʒu
	Vector3 vec;//�x�N�g��
	float temp1;//���������킹
	Vector4 col;//�F
	Vector3 size;//�T�C�Y
	float speed;//�X�s�[�h
	Vector3 rotate;//��]
	Vector3 axis;//��
	float gravity;//�d��
	bool active;//�����Ă��邩�ǂ���
};

//�p�[�e�B�N���̕`��ɕK�v�ȏ��
struct ParticleDrawData
{
	Vector3 pos;
	Vector4 color;
	Vector3 size;
	Vector3 rotate;
};