#pragma once

#include <Windows.h>
#include <mmreg.h>
#include <MSAcm.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msacm32.lib")

//�t�H�[�}�b�g�ϊ�
class SoundFormatConverter
{
public:
	SoundFormatConverter();
	~SoundFormatConverter();

	//mp3����pcm�ɕϊ�
	bool mp3ToPCM(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format);
	//�R���o�[�g
	unsigned convert(BYTE** out, unsigned readSize);
	//�f�R�[�h��̃T�C�Y�擾
	bool decodeSize(const unsigned src, unsigned* dst, bool srcToDst = true);

private:
	SoundFormatConverter(const SoundFormatConverter&) = delete;
	SoundFormatConverter& operator=(const SoundFormatConverter&) = delete;

	//�t�H�[�}�b�g�ϊ����\��
	bool isConvert(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format) const;
	//ACM�X�g���[�����J��
	bool openAcmStream(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format);
	//ACM�X�g���[���w�b�_�[���쐬����
	bool createAcmStreamHeader(WAVEFORMATEX** pcmFormat, const MPEGLAYER3WAVEFORMAT& mp3Format);

private:
	HACMSTREAM mAcmStream;
	ACMSTREAMHEADER mAcmStreamHeader;

};