#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include <memory>

class SourceVoice;
class SoundPlayer;
class BufferSubmitter;

class SoundStreaming
{
public:
	SoundStreaming(SourceVoice& sourceVoice, SoundPlayer& player, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format);
	~SoundStreaming();

	//�o�b�t�@�̏�Ԃ��Ď�����
	void polling();

	void seek(float point);

private:
	SoundStreaming(const SoundStreaming&) = delete;
	SoundStreaming& operator=(const SoundStreaming&) = delete;

	//�o�b�t�@��ǉ�����
	void addBuffer();
	//�Z�J���_���o�b�t�@�Ɏw��T�C�Y���o�b�t�@��ǂݍ���
	long read(unsigned readSize);
	//������Ԃɖ߂�
	void initialize();
	//remainBufferSize���Čv�Z����
	void recomputeRemainBufferSize(unsigned point);

private:
	static constexpr unsigned BUFFER_COUNT = 2;
	static constexpr unsigned BUFFER_QUEUE_MAX = 2;

	SourceVoice& mSourceVoice;
	SoundPlayer& mPlayer;
	std::unique_ptr<BufferSubmitter> mBufferSubmitter;
	std::unique_ptr<ISoundLoader> mLoader;
	//�o�b�t�@
	BYTE* mBuffer[BUFFER_COUNT];
	//�o�b�t�@�ԍ�
	unsigned mPrimary;
	unsigned mSecondary;
	//�t�@�C������ǂݍ��ރT�C�Y
	const unsigned READ_SIZE;
	//�o�b�t�@�̗]��
	unsigned mRemainBufferSize;
	//�ǂݍ��݃J�[�\��
	unsigned mWrite;
	//�t�@�C���̏I�[�܂œǂݍ��񂾂�
	bool mEndOfFile;

};