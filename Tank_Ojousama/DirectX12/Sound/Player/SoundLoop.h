#pragma once

#include <xaudio2.h>

class SourceVoice;
class SoundPlayer;

class SoundLoop
{
public:
	SoundLoop(SourceVoice& sourceVoice, SoundPlayer& player);
	~SoundLoop();

	//更新
	void update();
	//ループ区間を指定
	//void setLoopPoint(float begin, float end = 0.f);
	//曲の最初から最後までループ
	void loopAll();
	//ループから抜ける
	void exitLoop();
	//ループしている最中か
	bool isLoop() const;
	//ループのおわりを返す
	float getLoopEnd() const;

private:
	SoundLoop(const SoundLoop&) = delete;
	SoundLoop& operator=(const SoundLoop&) = delete;

	//ループ設定共通関数
	void setValue(float begin, float end);
	//再生時間内にクランプ
	void clamp();

private:
	SourceVoice& mSourceVoice;
	SoundPlayer& mPlayer;
	//ループ開始地点 sec
	float mLoopBegin;
	//ループ折返し地点 sec
	float mLoopEnd;
	//ループ中か
	bool mIsLoop;

};