#pragma once

//����̐��K�������ł�DSP�ݒ���`����\����
struct DistanceCurvePoint
{
	//���K�����ꂽ���� [0.f, 1.f]
	float distance;
	//DSP�R���g���[���ݒ�
	float dspSettings;
};

//���`�Z�O�����g�ō\������閾���I�ȋ敪�I�Ȑ����`���A���K�����ꂽ�����Ɋւ���DSP�̓���𒼐ڒ�`����\����
struct DistanceCurve
{
	//DistanceCurvePoint�z��
	//�����Ɋւ��ď����Ń\�[�g����Ă���K�v������
	DistanceCurvePoint* points;
	//�����Ȑ��|�C���g�̐�
	//���K�����ꂽ�[�_���Œ�2�K�v
	unsigned pointCount;
};