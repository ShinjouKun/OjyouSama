#pragma once

#include "ISoundLoader.h"
#include <mmreg.h>
#include <fstream>
#include <memory>

class FrameHeader;
class SoundFormatConverter;

class MP3 : public ISoundLoader 
{
public:
	MP3();
	~MP3();
	virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) override;
	virtual long read(BYTE** buffer, unsigned size) override;
	virtual void seek(long offset, Seek seek = Seek::CURRENT) override;
	virtual void seekBegin() override;
	virtual unsigned size() const override;

private:
	MP3(const MP3&) = delete;
	MP3& operator=(const MP3&) = delete;

	//�t�@�C�������[�v���ĉ�͂���
	bool parse();
	//�f�[�^�����̉�͂�����
	bool parseData(const BYTE* header);
	bool parseData2(const BYTE* header);//ID3v1�p

private:
	std::ifstream mIfs;
	std::unique_ptr<FrameHeader> mFrameHeader;
	std::unique_ptr<SoundFormatConverter> mFormatConverter;
	//�t�@�C���S�̂̃T�C�Y
	unsigned mFileSize;
	//�f�[�^�����̃T�C�Y
	unsigned mDataSize;
	//�t�@�C���̐擪����f�[�^�ւ̃I�t�Z�b�g�l
	unsigned mDataOffset;

	//ID3v2�w�b�_�̃T�C�Y
	static constexpr unsigned ID3V2_HEADER_SIZE = 10;
	//ID3v1�����^�O�̃T�C�Y
	static constexpr unsigned ID3V1_END_SIZE = 128;
};