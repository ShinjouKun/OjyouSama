#pragma once

#include <xaudio2.h>

class SourceVoice;
class SoundPlayer;

class SoundLoop
{
public:
	SoundLoop(SourceVoice& sourceVoice, SoundPlayer& player);
	~SoundLoop();

	//�X�V
	void update();
	//���[�v��Ԃ��w��
	//void setLoopPoint(float begin, float end = 0.f);
	//�Ȃ̍ŏ�����Ō�܂Ń��[�v
	void loopAll();
	//���[�v���甲����
	void exitLoop();
	//���[�v���Ă���Œ���
	bool isLoop() const;
	//���[�v�̂�����Ԃ�
	float getLoopEnd() const;

private:
	SoundLoop(const SoundLoop&) = delete;
	SoundLoop& operator=(const SoundLoop&) = delete;

	//���[�v�ݒ苤�ʊ֐�
	void setValue(float begin, float end);
	//�Đ����ԓ��ɃN�����v
	void clamp();

private:
	SourceVoice& mSourceVoice;
	SoundPlayer& mPlayer;
	//���[�v�J�n�n�_ sec
	float mLoopBegin;
	//���[�v�ܕԂ��n�_ sec
	float mLoopEnd;
	//���[�v����
	bool mIsLoop;

};