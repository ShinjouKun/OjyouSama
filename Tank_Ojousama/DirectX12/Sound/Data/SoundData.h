#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include "../../Math/Math.h"

//Sound data storage class
struct SoundData
{
	//�g�`�f�[�^�̃T�C�Y
	const unsigned size;
	//�r�b�g���[�g
	const unsigned bitRate;
	//1�T���v���̃o�C�g��
	const unsigned blockAlign;
	//1�b�Ԃ̃o�C�g��
	const unsigned averageBytesPerSec;

	SoundData(const ISoundLoader& loader, const WaveFormat& format) :
		size(loader.size()),
		bitRate(format.bitsPerSample),
		blockAlign(format.blockAlign),
		averageBytesPerSec(format.avgBytesPerSec) {
	}

	//�b�P�ʂ̋Ȃ̒���
	float length() const {
		return (static_cast<float>(size) / static_cast<float>(averageBytesPerSec));
	}

	//�Ȃ͈͓̔��ɃN�����v
	float clamp(float sec) const {
		//�Đ����ԓ��ɃN�����v
		return Math::clamp<float>(sec, 0.f, length());
	}
};