#pragma once

#include "../Math/Math.h"
#include <memory>
#include <string>

class SourceVoice;

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
	//���ʂ̕ύX0�`1
	void setVol(float num);
	//�G�~�b�^�[�̈ʒu���Z�b�g
	void setPos(const Vector3& pos);
	//��������͈͂̎w��
	void setCurveDistanceScaler(float num);

private:
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;

private:
	std::string mFilename;
	std::shared_ptr<SourceVoice> mSourceVoice;
};