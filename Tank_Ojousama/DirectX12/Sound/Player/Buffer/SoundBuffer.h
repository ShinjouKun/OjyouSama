#pragma once

#include <xaudio2.h>

//�g�`�f�[�^�̍\����
struct SoundBuffer 
{
	//true���ƃo�b�t�@�̌㑱���Ȃ����Ƃ�����
	//�Đ���A�{�C�X�͎����I�ɏI����~��ԂɂȂ�
	bool isEndOfStream;
	//�w�b�_�����܂܂Ȃ��g�`�f�[�^�{�̂̐擪�A�h���X
	const BYTE* buffer;
	//�g�`�f�[�^�S�̂̃T�C�Y(�P��: �o�C�g)
	//XAUDIO2_MAX_BUFFER_BYTES(2GB)�����
	unsigned size;
	//�Đ��̊J�n�ʒu(�P��: �b)
	//������O�̃f�[�^�͖��������
	//0���Ɠ�����Đ�
	float playBegin;
	//�Đ��̈�̒���(�P��: �b)
	//playBegin + playLength�����̃f�[�^�͖��������
	//0���ƑS�̍Đ�
	float playLength;
	//���[�v�̊J�n�ʒu(�P��: �b)
	//���[�v���Đ܂�Ԃ��ۂɖ߂�ʒu
	float loopBegin;
	//���[�v�̒���(�P��: �b)
	//loopBegin + loopLength�����ۂ̐܂�Ԃ��ʒu
	float loopLength;
	//���[�v�����
	//0�Ń��[�v�Ȃ��AXAUDIO2_LOOP_INFINITE�ŉi�v���[�v
	unsigned loopCount;
	//�R�[���o�b�N�ŕԂ����R���e�L�X�g�l
	void* context;

	SoundBuffer() :
		isEndOfStream(false),
		buffer(nullptr),
		size(0),
		playBegin(0.f),
		playLength(0.f),
		loopBegin(0.f),
		loopLength(0.f),
		loopCount(0),
		context(nullptr) 
	{
	}
};

//�ȈՔŔg�`�f�[�^�̍\����
struct SimpleSoundBuffer 
{
	//�t���O
	bool isEndOfStream;
	//�w�b�_�����܂܂Ȃ��g�`�f�[�^�{�̂̐擪�A�h���X
	const BYTE* buffer;
	//�o�^����g�`�f�[�^�̃T�C�Y
	unsigned size;

	SimpleSoundBuffer() :
		isEndOfStream(false),
		buffer(nullptr),
		size(0) 
	{
	}
};
