#pragma once

#include "../Voice/IVoice.h"
#include <xaudio2.h>

//���̒�ʂ�����
class SoundPan
{
public:
	SoundPan(IVoice& voice, unsigned inChannels, unsigned outChannels);
	~SoundPan();

	//���ʔz�񂩂特���p������
	void pan(const float volumes[]);

	//�w�肵���{�C�X���p������
	void panOutputVoice(const IVoice& voice, const float volumes[]);

	//�ʒu�ɂ���ĉ����p������
	void panFromPositionX(float positionX);

	//���̒�ʂ𒆐S�ɖ߂�
	void panCenter();

private:
	SoundPan(const SoundPan&) = delete;
	SoundPan& operator=(const SoundPan&) = delete;

	//�o�͐�����肷��
	void selectOutput(const float volumes[]);
	//�p�������s����
	void setOutputMatrix(IXAudio2Voice* outputVoice, unsigned outChannels, const float volumes[]);

private:
	IVoice& mVoice;
	static constexpr float CENTER_VOLUME = 0.707f;
	const unsigned INPUT_CHANNELS;
	const unsigned MASTER_VOICE_CHANNELS;
};