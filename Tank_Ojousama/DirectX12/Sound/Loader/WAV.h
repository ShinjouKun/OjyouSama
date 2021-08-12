#pragma once

#include "ISoundLoader.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

class WAV : public ISoundLoader 
{
	enum class FindFlag
	{
		NONE = 0,
		CHUNK = MMIO_FINDCHUNK, //�w�肳�ꂽ���ʎq�����`�����N������
		RIFF = MMIO_FINDRIFF, //���ʎq�uRIFF�v�Ǝw�肳�ꂽ�`�����N������
		LIST = MMIO_FINDLIST, //���ʎq�uLIST�v�Ǝw�肳�ꂽ�`�����N������
	};

public:
	WAV();
	~WAV();
	virtual bool loadFromFile(WAVEFORMATEX* format, const std::string& fileName) override;
	//�w�肵���T�C�Y���f�[�^��ǂݎ��
	virtual long read(BYTE** buffer, unsigned size) override;
	//�I�t�Z�b�g���V�[�N
	virtual void seek(long offset, Seek seek = Seek::CURRENT) override;
	//�o�b�t�@�����̐擪�܂ŃV�[�N
	virtual void seekBegin() override;
	//�g�`�����̑傫��
	virtual unsigned size() const override;

private:
	WAV(const WAV&) = delete;
	WAV& operator=(const WAV&) = delete;

	//WAV�t�@�C�����J��
	void open(const std::string& fileName);
	//�`�����N�S�̂�ǂݍ���
	long readChunk(void* out, unsigned size) const;
	//�`�����N��ǂݍ���
	bool descend(MMCKINFO* out, const MMCKINFO* parent, FindFlag flag) const;
	bool ascend(MMCKINFO* out);
	//�`�����N��ݒ肷��
	void setChunkID(MMCKINFO* out, const char* ch);
	//FourCC���擾����
	constexpr FOURCC getFourCC(const char* ch) const;
	//WAV�t�@�C����
	constexpr bool isWavFile(const MMCKINFO& riffChunk) const;
	//�`�����N����t�H�[�}�b�g���쐬����
	bool createFormatByChunk(WAVEFORMATEX* format);
	//Wave�t�H�[�}�b�g���쐬����
	void createWaveFormat(WAVEFORMATEX** dst, const PCMWAVEFORMAT& src);

private:
	//Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API)
	HMMIO mHMmio;
	//�ŏ㕔�`�����N(RIFF�`�����N)�ۑ��p
	MMCKINFO mRiffChunk;
	//�t�H�[�}�b�g�`�����N���
	MMCKINFO mFormatChunk;
	//�f�[�^�`�����N���
	MMCKINFO mDataChunk;

	//�`�����NID�̃T�C�Y
	static constexpr unsigned char CHUNK_ID_SIZE = 4;
	//�`�����N�̎��
	static constexpr const char CHUNK_FORMAT[CHUNK_ID_SIZE] = { 'f', 'm', 't', ' ' };
	static constexpr const char CHUNK_DATA[CHUNK_ID_SIZE] = { 'd', 'a', 't', 'a' };
	static constexpr const char CHUNK_RIFF[CHUNK_ID_SIZE] = { 'R', 'I', 'F', 'F' };
	static constexpr const char CHUNK_WAVE[CHUNK_ID_SIZE] = { 'W', 'A', 'V', 'E' };
};
