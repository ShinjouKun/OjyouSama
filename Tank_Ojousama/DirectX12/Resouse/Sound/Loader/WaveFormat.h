#pragma once

#include <xaudio2.h>

//���ׂĂ̔�PCM�t�H�[�}�b�g�Ɏg�p�����g���g�`�t�H�[�}�b�g�\��
struct WaveFormat 
{
	//�t�H�[�}�b�g�^�C�v
	unsigned short formatTag;
	//�`�����l����
	unsigned short channels;
	//�T���v�����O���[�g
	unsigned long samplesPerSec;
	//�o�b�t�@����p
	unsigned long avgBytesPerSec;
	//�f�[�^�̃u���b�N�T�C�Y
	unsigned short blockAlign;
	//�r�b�g���[�g
	unsigned short bitsPerSample;
	//�悭�킩���
	unsigned short cbSize;

	WaveFormat() :
		formatTag(0),
		channels(0),
		samplesPerSec(0),
		avgBytesPerSec(0),
		blockAlign(0),
		bitsPerSample(0),
		cbSize(0) {
	}

	WaveFormat(const WAVEFORMATEX& format) :
		formatTag(format.wFormatTag),
		channels(format.nChannels),
		samplesPerSec(format.nSamplesPerSec),
		avgBytesPerSec(format.nAvgBytesPerSec),
		blockAlign(format.nBlockAlign),
		bitsPerSample(format.wBitsPerSample),
		cbSize(format.cbSize) {
	}
};
