#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include <xaudio2.h>
#include <memory>

class SourceVoice;
class SoundStreaming;
class SoundPlayTimer;
class SoundLoop;

class SoundPlayer
{
public:
	SoundPlayer(SourceVoice& sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, float maxFrequencyRatio);
	~SoundPlayer();
	//更新
	void update();
	//ストリーミング再生
	void playStreaming();

	//再生開始地点を設定
	void setPlayPoint(float point);
	//再生を一時停止
	void pause();
	//再生を停止
	void stop();
	//再生が停止しているか
	bool isStop() const;
	//
	SoundPlayTimer& getPlayTimer() const;
	//
	SoundLoop& getLoop() const;

private:
	SoundPlayer(const SoundPlayer&) = delete;
	SoundPlayer& operator=(const SoundPlayer&) = delete;

	//ポーズ・ストップ共通処理
	bool pauseAndStop(unsigned flag);

private:
	SourceVoice& mSourceVoice;
	std::unique_ptr<SoundStreaming> mStreaming;
	std::unique_ptr<SoundPlayTimer> mPlayTimer;
	std::unique_ptr<SoundLoop> mLoop;
	bool mIsPlay;

};