#pragma once

#include "../../Math/Math.h"

//��{�I�Ȑݒ�
struct ParticleSystems
{
	float Duration = 1.0f;//�p�[�e�B�N������o���鎞�Ԃ̒����B���[�v�̏ꍇ�A1�T�C�N���̒���
	bool Looping = false;//���[�v���邩�ǂ���
	float StartDelay = 0.0f;//�J�n����܂ł̎���
	float StartLifeTime = 5.0f;//��������
	float StartSpeed = 1.0f;//�ŏ��̃X�s�[�h
	Vector3 StartVector = Vector3::zero;//����
	Vector3 StartSize3D = Vector3::one;//�ŏ��̑傫��
	Vector3 StartRotation3D = Vector3::zero;//�ŏ��̉�]
	Vector4 StartColor = Vector4::one;//�J�n���̐F
	float GravityModifier = 0.0f;//�d��
};

//1�x�ɏo����
struct Burst
{
	float Time = 1.f;//�S�ďo���܂ł̎���
	int Count = 1000;//1�t���[���ɏo����
	int Cycles = 1;//
	float Interval = 0.01f;//�C���^�[�o��
	float Probabillity = 1.f;//
};

//�G�~�b�^�[����p�[�e�B�N���ɑ΂���ω���(1�t���[�����ɂ��̒l���ǉ�����Ă���)
struct AmountOfChangeInParticles
{
	float speed = 0.0f;//�X�s�[�h
	Vector3 vector = Vector3::zero;//�����̕ω�
	Vector3 size = Vector3::zero;//�T�C�Y
	Vector3 rotate = Vector3::zero;//��]
	Vector4 color = Vector4::zero;//�F
};

//�p�[�e�B�N���̃����_����(�p�[�e�B�N���V�X�e����+����0�`�̒l������)
struct ParticleRandomState
{
	float randomSpeed = 0.0f;//�����_���ȃX�s�[�h��ǉ�
	float randomLife = 0.0f;//�����_���Ȑ������Ԃ�ǉ�
	Vector3 randomSize3D = Vector3::zero;//�����_���ȃT�C�Y��ǉ�
	Vector3 randomRotate3D = Vector3::zero;//�����_���ȉ�]��ǉ�
	Vector4 randomColor = Vector4::zero;//�����_���ȃJ���[��ǉ�
};

//�p�[�e�B�N���̉�]�^���p
struct ParticleRotateState
{
	float angle = 0.0f;
	Vector3 Axis = Vector3::zero;
};