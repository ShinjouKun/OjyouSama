#pragma once

#include <memory>
#include <string>

class SourceVoice;

class Sound
{
public:
	Sound(std::string filename);
	~Sound();
	//�T�E���h�Đ�
	void play();
	//�T�E���h���[�v�Đ�
	void playLoop();
	//3D�T�E���h�Đ�
	//void play3D();
	//3D�T�E���h���[�v�Đ�
	//void playLoop3D();
	//�ꎞ��~
	void pause();
	//��~
	void stop();
	//���ʂ̕ύX0�`100
	//void setVol(int num);

private:
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;

private:
	std::string mFilename;
	std::shared_ptr<SourceVoice> mSourceVoice;
};