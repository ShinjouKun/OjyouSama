#pragma once

#include "../Voice/IVoice.h"
#include <xaudio2.h>

//音の定位を扱う
class SoundPan
{
public:
	SoundPan(IVoice& voice, unsigned inChannels, unsigned outChannels);
	~SoundPan();

	//音量配列から音をパンする
	void pan(const float volumes[]);

	//指定したボイスをパンする
	void panOutputVoice(const IVoice& voice, const float volumes[]);

	//位置によって音をパンする
	void panFromPositionX(float positionX);

	//音の定位を中心に戻す
	void panCenter();

private:
	SoundPan(const SoundPan&) = delete;
	SoundPan& operator=(const SoundPan&) = delete;

	//出力先を決定する
	void selectOutput(const float volumes[]);
	//パンを実行する
	void setOutputMatrix(IXAudio2Voice* outputVoice, unsigned outChannels, const float volumes[]);

private:
	IVoice& mVoice;
	static constexpr float CENTER_VOLUME = 0.707f;
	const unsigned INPUT_CHANNELS;
	const unsigned MASTER_VOICE_CHANNELS;
};