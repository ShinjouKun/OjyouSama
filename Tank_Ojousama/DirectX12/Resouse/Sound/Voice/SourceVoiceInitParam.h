#pragma once

#include "../Flag/Flag.h"
#include <xaudio2.h>

struct SourceVoiceInitParam
{
	//�{�C�X�ɑ�����ݒ肷��t���O
	Flag flags;

	//�{�C�X�̍ő勖�e�Đ����x��ݒ肷��
	//�ő�XAUDIO2_MAX_FREQ_RATIO(1024�{)�܂ŉ\
	float maxFrequencyRatio;

	//�R�[���o�b�N
	IXAudio2VoiceCallback* callback;

	//�{�C�X�̏o�͐��ݒ肷��
	//null���ƃ}�X�^�[�{�C�X�����ɂȂ�
	//�ォ��setOutputVoices�Őݒ�\
	const XAUDIO2_VOICE_SENDS* sendList;

	//�G�t�F�N�g�w��
	//null���Ǝg�p���Ȃ�
	//�ォ��setEffectChains�Őݒ�\
	const XAUDIO2_EFFECT_CHAIN* effectChain;

	//3D���Z�����邩
	bool isCalculate3D;
	
	SourceVoiceInitParam() :
		flags(),
		maxFrequencyRatio(XAUDIO2_DEFAULT_FREQ_RATIO),
		callback(nullptr),
		sendList(nullptr),
		effectChain(nullptr),
		isCalculate3D(false) {
	}
};