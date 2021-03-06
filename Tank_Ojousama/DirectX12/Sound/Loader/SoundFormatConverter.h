#pragma once

#include <Windows.h>
#include <mmreg.h>
#include <MSAcm.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msacm32.lib")

//フォーマット変換
class SoundFormatConverter
{
public:
	SoundFormatConverter();
	~SoundFormatConverter();

	//mp3からpcmに変換
	bool mp3ToPCM(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format);
	//コンバート
	unsigned convert(BYTE** out, unsigned readSize);
	//デコード後のサイズ取得
	bool decodeSize(const unsigned src, unsigned* dst, bool srcToDst = true);

private:
	SoundFormatConverter(const SoundFormatConverter&) = delete;
	SoundFormatConverter& operator=(const SoundFormatConverter&) = delete;

	//フォーマット変換が可能か
	bool isConvert(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format) const;
	//ACMストリームを開く
	bool openAcmStream(WAVEFORMATEX** pcmFormat, MPEGLAYER3WAVEFORMAT& mp3Format);
	//ACMストリームヘッダーを作成する
	bool createAcmStreamHeader(WAVEFORMATEX** pcmFormat, const MPEGLAYER3WAVEFORMAT& mp3Format);

private:
	HACMSTREAM mAcmStream;
	ACMSTREAMHEADER mAcmStreamHeader;

};