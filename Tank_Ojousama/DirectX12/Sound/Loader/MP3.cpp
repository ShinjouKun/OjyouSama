#include "MP3.h"
#include "FrameHeader.h"
#include "SoundFormatConverter.h"
#include "FileSize.h"
#include <cassert>
#include <string>

MP3::MP3():
	mIfs(),
	mFrameHeader(std::make_unique<FrameHeader>()),
	mFormatConverter(std::make_unique<SoundFormatConverter>()),
	mFileSize(0),
	mDataSize(0),
	mDataOffset(0)
{
}

MP3::~MP3() = default;

bool MP3::loadFromFile(WAVEFORMATEX * format, const std::string & fileName)
{
	mIfs.open(fileName, std::ios::in | std::ios::binary);
	if (!mIfs.is_open())
	{
		assert("open ���s");
	}

	//�t�@�C���S�̂̃T�C�Y
	//mFileSize = std::filesystem::file_size(fileName);
	mFileSize = getFileSize(fileName.c_str());
	if (mFileSize == 0)
	{
		//�t�@�C���T�C�Y�̎擾�Ɏ��s
		assert(false);
	}

	//�t�@�C������͂���
	if (!parse())
	{
		assert("��͎��s");
		return false;
	}

	//��͂����f�[�^��MPEGLAYER3WAVEFORMAT�ɋl�ߍ���
	MPEGLAYER3WAVEFORMAT mp3wf = { 0 };
	mFrameHeader->setDataToMpegLayer3WaveFormat(&mp3wf);

	//mp3����pcm�փf�[�^�ϊ�����
	format->wFormatTag = WAVE_FORMAT_PCM;
	format->cbSize = sizeof(WAVEFORMATEX);
	mFormatConverter->mp3ToPCM(&format, mp3wf);

	//�f�[�^�T�C�Y���f�R�[�h
	mFormatConverter->decodeSize(mDataSize, &mDataSize);

	//�t�@�C���|�C���^���f�[�^�̐擪�ɖ߂��Ă���
	seekBegin();

	return true;

}

long MP3::read(BYTE ** buffer, unsigned size)
{
	//pcm�p�̃T�C�Y����mp3�p�̃T�C�Y�ɕύX����
	unsigned decodeSize = 0;
	mFormatConverter->decodeSize(size, &decodeSize, false);
	//mp3�p�̃T�C�Y����f�[�^��ǂݎ��
	mIfs.read(reinterpret_cast<char*>(*buffer), decodeSize);
	//mp3�̃o�b�t�@����pcm�̃o�b�t�@�ɕύX����
	auto count = mIfs.gcount();
	auto readSize = mFormatConverter->convert(buffer, count);

	return readSize;
}

void MP3::seek(long offset, Seek seek)
{
	//pcm�p�̃T�C�Y����mp3�p�̃T�C�Y�ɕύX����
	unsigned decodeOffset = 0;
	mFormatConverter->decodeSize(offset, &decodeOffset, false);

	//�t���O���N���A���ăV�[�N
	mIfs.clear();
	mIfs.seekg(decodeOffset, static_cast<std::ios_base::_Seekdir>(seek));
}

void MP3::seekBegin()
{
	seek(mDataOffset, Seek::BEGIN);
}

unsigned MP3::size() const
{
	return mDataSize;
}

bool MP3::parse()
{
	bool initFrameHeader = false;
	bool initData = false;
	BYTE header[ID3V2_HEADER_SIZE];
	while (!mIfs.eof()) {
		mIfs.read(reinterpret_cast<char*>(header), ID3V2_HEADER_SIZE);

		if (mFrameHeader->isFrameHeader(header))
		{
			//���łɉ�͍ς݂Ȃ玟��
			if (initFrameHeader)
			{
				continue;
			}
			//�t���[���w�b�_�̉�͂�����
			initFrameHeader = mFrameHeader->parseFrameHeader(header);
		}
		else 
		{
			//���łɉ�͍ς݂Ȃ玟��
			if (initData)
			{
				continue;
			}
			//�f�[�^�����̉�͂�����
			initData = parseData(header);
		}

		//�K�v�ȉ�͂��������Ă�����I��
		if (initFrameHeader && initData)
		{
			return true;
		}
	}

	//ID3v1�̉\��
	mIfs.seekg(0, std::ios_base::end - ID3V1_END_SIZE);
	mIfs.read(reinterpret_cast<char*>(header), ID3V1_END_SIZE);
	return parseData2(header);

	//��͎��s
	return false;
}

bool MP3::parseData(const BYTE * header)
{
	//ID3v2�ł���
	if (memcmp(header, "ID3", 3) == 0) {
		unsigned tagSize = ((header[6] << 21) | (header[7] << 14) | (header[8] << 7) | (header[9])) + ID3V2_HEADER_SIZE;
		//�f�[�^�����ւ̃I�t�Z�b�g�l����
		mDataOffset = tagSize;
		//�f�[�^�����̃T�C�Y����
		mDataSize = mFileSize - mDataOffset;

		return true;
	}
	/*
	//ID3v1�ł���
	else
	{
		if (memcmp(header, "TAG", 3) == 0)
		{
			//�����̃^�O���Ȃ�
			mDataSize = mFileSize - ID3V1_END_SIZE;
		}
		else
		{
			//�t�@�C���S�̂�MP3�f�[�^
			mDataSize = mFileSize;
		}
		return true;
	}
	*/
	return false;
}

bool MP3::parseData2(const BYTE * header)
{
	//ID3v1�ł���
	char s[ID3V1_END_SIZE];
	memcpy(s, header, ID3V1_END_SIZE);//byte��char[]�ɕϊ�
	std::string str(s, sizeof(s) / sizeof(s[0]));//char[]��string�ɕϊ�

	if (str.find("TAG") != std::string::npos)
	{
		//�����̃^�O���Ȃ�
		mDataSize = mFileSize - ID3V1_END_SIZE;
		return true;
	}

	mDataSize = mFileSize;
	return true;
}
