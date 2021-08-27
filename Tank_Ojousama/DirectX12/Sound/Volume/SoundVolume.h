#pragma once

#include "../Voice/IVoice.h"
#include <memory>

class SoundPan;

class SoundVolume
{
public:
	SoundVolume(IVoice& voice, unsigned inChannels, unsigned outChannels);
	~SoundVolume();

	void update();
	//���ʕύX
	void setVolume(float volume);
	//���ݐݒ肳��Ă��鉹�ʂ�Ԃ�
	float getVolume() const;
	//�p���j���O���N���X��Ԃ�
	SoundPan& getPan() const;

private:
	SoundVolume(const SoundVolume&) = delete;
	SoundVolume& operator=(const SoundVolume&) = delete;

private:
	IVoice& mVoice;
	std::unique_ptr<SoundPan> mSoundPan;
	float mCurrentVolume;
};