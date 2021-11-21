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
	//音量変更
	void setVolume(float volume);
	//現在設定されている音量を返す
	float getVolume() const;
	//パンニング専門クラスを返す
	SoundPan& getPan() const;

private:
	SoundVolume(const SoundVolume&) = delete;
	SoundVolume& operator=(const SoundVolume&) = delete;

private:
	IVoice& mVoice;
	std::unique_ptr<SoundPan> mSoundPan;
	float mCurrentVolume;
};