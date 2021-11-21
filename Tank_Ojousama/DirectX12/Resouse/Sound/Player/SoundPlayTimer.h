#pragma once

class SourceVoice;
class SoundPlayer;

class SoundPlayTimer
{
public:
	SoundPlayTimer(SourceVoice& sourceVoice, SoundPlayer& player);
	~SoundPlayTimer();

	//�Đ����Ԃ̐ݒ�
	void setPlayTime(float time);
	//�Đ����x�ݒ�
	void setFrequency(float getFrequency);
	//�Đ����Ԃ��J��Ԃ�
	float getPlayTime() const;
	//�Đ����Ԃ�i�߂�
	void startTimer();
	//�Đ����Ԃ��~�߂�
	void stopTimer();

private:
	SoundPlayTimer(const SoundPlayTimer&) = delete;
	SoundPlayTimer& operator=(const SoundPlayTimer&) = delete;

private:
	SourceVoice& mSourceVoice;
	SoundPlayer& mPlayer;
};