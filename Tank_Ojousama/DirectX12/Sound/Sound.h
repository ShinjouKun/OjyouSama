#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>

class SourceVoice;
class Sound3DListener;

class Sound
{
public:
	Sound(std::string filename, bool isCalculate3D = false);
	~Sound();
	//�T�E���h�Đ�
	void play();
	//�T�E���h���[�v�Đ�
	void playLoop();
	//�ꎞ��~
	void pause();
	//��~
	void stop();
	//���ʂ̕ύX0�`100
	void setVol(float num);
	//���X�i�[�̃Z�b�g
	void setLisner(const Vector3& pos);
	//
	void setPos(const Vector3& pos);

private:
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;

private:
	std::string mFilename;
	std::shared_ptr<SourceVoice> mSourceVoice;
	std::shared_ptr<Sound3DListener> mListener;
};