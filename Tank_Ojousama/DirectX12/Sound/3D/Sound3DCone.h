#pragma once

#include "../../Math/Math.h"

//3D�T�E���h�ň����R�[���\����
//�w�����̂��鉹�̑傫����\������
struct Sound3DCone
{
	//���W�A���P�ʂ̓����R�[���p�x [0.f, 2PI]
	float innerAngle;
	//���W�A���P�ʂ̊O���R�[���p�x [innerAngle, 2PI]
	float outerAngle;
	//�����̃{�����[���X�P�[���[
	float innerVolume;
	//�O���̃{�����[���X�P�[���[
	float outerVolume;
	//LPF�_�C���N�g�p�X�܂��̓��o�[�u�p�X�̌W���X�P�[���[ [0.f, 1.f]
	float innerLPF;
	//LPF�_�C���N�g�p�X�܂��̓��o�[�u�p�X�̌W���X�P�[���[ [0.f, 1.f]
	float outerLPF;
	//�����̃��o�[�u�Z���h���x���X�P�[���[ [0.f, 2.f]
	float innerReverb;
	//�O���̃��o�[�u�Z���h���x���X�P�[���[ [0.f, 2.f]
	float outerReverb;
};