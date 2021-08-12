#pragma once

#include "SoundBuffer.h"
#include <xaudio2.h>

class SourceVoice;

//�\�[�X�{�C�X�Ƀo�b�t�@��o�^����N���X
class BufferSubmitter 
{
public:
	BufferSubmitter(SourceVoice& sourceVoice);
	~BufferSubmitter();

	void submitBuffer(const SoundBuffer& buffer) const;

	void submitSimpleBuffer(const SimpleSoundBuffer& buffer) const;

private:
	BufferSubmitter(const BufferSubmitter&) = delete;
	BufferSubmitter& operator=(const BufferSubmitter&) = delete;

	//�g�`�f�[�^�̓o�^
	void submitSourceBuffer(const XAUDIO2_BUFFER& buffer) const;
	//SoundBuffer����XAUDIO2_BUFFER�ւ̕ϊ�
	XAUDIO2_BUFFER toBuffer(const SoundBuffer& buffer) const;
	//SimpleSoundBuffer����XAUDIO2_BUFFER�ւ̕ϊ�
	XAUDIO2_BUFFER toBuffer(const SimpleSoundBuffer& buffer) const;

private:
	SourceVoice& mSourceVoice;
};
