#pragma once

#include "../Flag/Flag.h"
#include <xaudio2.h>

//�T�u�~�b�N�X�{�C�X�쐬���̏����p�����[�^
struct SubmixVoiceInitParam
{
	//�{�C�X�̃`�����l����
	unsigned channels;
	//�{�C�X�̃T���v�����O���g��
	unsigned inputSampleRate;
	//�{�C�X�ɑ�����ݒ肷��t���O
	Flag flags;
	//�����̃T�u�~�b�N�X�{�C�X�����݂���Ƃ��̏������Ԃ̐��l
	//���G�ȃ��[�e�B���O��g��ŏ������Ԃ����ɉe������悤�ȂƂ��Ɏg�p����
	//�����������������珇�ɏ������s����
	unsigned processingStage;
	//�{�C�X�̏o�͐��ݒ肷��
	//null���ƃ}�X�^�[�{�C�X�����ɂȂ�
	//�ォ��setOutputVoices�Őݒ�\
	const XAUDIO2_VOICE_SENDS* sendList;
	//�G�t�F�N�g�w��
	//null���Ǝg�p���Ȃ�
	//�ォ��setEffectChains�Őݒ�\
	const XAUDIO2_EFFECT_CHAIN* effectChain;

	SubmixVoiceInitParam() :
		channels(0),
		inputSampleRate(0),
		flags(),
		processingStage(0),
		sendList(nullptr),
		effectChain(nullptr)
	{
	}
};