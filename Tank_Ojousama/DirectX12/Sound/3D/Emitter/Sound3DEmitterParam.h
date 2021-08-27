#pragma once

#include "../DistanceCurve.h"
#include "../Sound3DCone.h"
#include "../../../Math/Math.h"

struct Sound3DEmitterParam
{
	//�������̎w��
	//NULL���Ɩ��w��(�R�[���������K������Ȃ�)
	Sound3DCone* cone;
	//�O�����x�N�g��
	Vector3 orientFront;
	//��x�N�g��
	//orientFront�ƒ��s���Ă���K�v������
	Vector3 orientTop;

	//�G�~�b�^�[�̈ʒu
	Vector3 position;
	//�h�b�v���[�v�Z�Ɏg�p����鑬�x�x�N�g��
	Vector3 velocity;

	//�������a [0.f, Math::infinity]
	float innerRadius;
	//�����a�p�x [0.f, Math::PI / 4.f]
	float innerRadiusAngle;

	//�G�~�b�^�[�̐�
	unsigned channelCount;
	//channelCount��1���傫���ꍇ�Ƀ`�����l�����z�u�����ʒu����̋���
	float channelRadius;
	//�`�����l�����a�ɉ��������W�A���ŕ��ʊp�Ƃ��ĕ\�����`�����l���ʒu�̃e�[�u��
	float* channelAzimuths;

	//�{�����[�����x���̋����Ȑ�
	//NULL���Ƌ�����0.f��curveDistanceScaler�̊Ԃɂ���ꍇ�A�������K�p����Ȃ��悤�ɋt��摥�ɏ���������ʂȃf�t�H���g�J�[�u���w�肳���
	DistanceCurve* volumeCurve;
	//LFE���[���I�t�����Ȑ�
	//NULL����[0.f, curveDistanceScaler][curveDistanceScaler, 0.f]
	DistanceCurve* lfeCurve;
	//���[�p�X�t�B���^�[(LPF)���ڃp�X�W�������Ȑ�
	//NULL����[0.f, 1.f][1.f, 0.75f]
	DistanceCurve* lpfDirectCurve;
	//LPF���o�[�u�p�X�W�������J�[�u
	//NULL����[0.f, 0.75f][1.f, 0.75f]
	DistanceCurve* lpfReverbCurve;
	//���o�[�u�Z���h���x���̋����J�[�u
	//NULL����[0.f, 1.f][1.f, 0.f]
	DistanceCurve* reverbCurve;

	//���K�����ꂽ�����Ȑ����X�P�[�����O������A���ʂ����������肷�邽�߂Ɏg�p�����Ȑ������X�P�[���[ [Math::negInfinity, Math::infinity]
	float curveDistanceScaler;
	//�h�b�v���[�V�t�g���ʂ��֒����邽�߂Ɏg�p�����h�b�v���[�V�t�g�X�P�[���[ [0.f, Math::infinity]
	float dopplerScaler;

	Sound3DEmitterParam() :
		cone(nullptr),
		orientFront(Vector3::forward),
		orientTop(Vector3::up),
		position(Vector3::zero),
		velocity(Vector3::zero),
		innerRadius(0.f),
		innerRadiusAngle(0.f),
		channelCount(0),
		channelRadius(0.f),
		channelAzimuths(nullptr),
		volumeCurve(nullptr),
		lfeCurve(nullptr),
		lpfDirectCurve(nullptr),
		lpfReverbCurve(nullptr),
		reverbCurve(nullptr),
		curveDistanceScaler(0.f),
		dopplerScaler(0.f)
	{
	}

};