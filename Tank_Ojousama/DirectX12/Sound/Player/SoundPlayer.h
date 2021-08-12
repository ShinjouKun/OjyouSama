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
	//�X�V
	void update();
	//�X�g���[�~���O�Đ�
	void playStreaming();

	//�Đ��J�n�n�_��ݒ�
	void setPlayPoint(float point);
	//�Đ����ꎞ��~
	void pause();
	//�Đ����~
	void stop();
	//�Đ�����~���Ă��邩
	bool isStop() const;
	//
	SoundPlayTimer& getPlayTimer() const;
	//
	SoundLoop& getLoop() const;

private:
	SoundPlayer(const SoundPlayer&) = delete;
	SoundPlayer& operator=(const SoundPlayer&) = delete;

	//�|�[�Y�E�X�g�b�v���ʏ���
	bool pauseAndStop(unsigned flag);

private:
	SourceVoice& mSourceVoice;
	std::unique_ptr<SoundStreaming> mStreaming;
	std::unique_ptr<SoundPlayTimer> mPlayTimer;
	std::unique_ptr<SoundLoop> mLoop;
	bool mIsPlay;

};