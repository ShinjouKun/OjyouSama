#pragma once

class SourceVoice;
class SoundPlayer;

class SoundPlayTimer
{
public:
	SoundPlayTimer(SourceVoice& sourceVoice, SoundPlayer& player);
	~SoundPlayTimer();

	//再生時間の設定
	void setPlayTime(float time);
	//再生速度設定
	void setFrequency(float getFrequency);
	//再生時間を繰り返す
	float getPlayTime() const;
	//再生時間を進める
	void startTimer();
	//再生時間を止める
	void stopTimer();

private:
	SoundPlayTimer(const SoundPlayTimer&) = delete;
	SoundPlayTimer& operator=(const SoundPlayTimer&) = delete;

private:
	SourceVoice& mSourceVoice;
	SoundPlayer& mPlayer;
};