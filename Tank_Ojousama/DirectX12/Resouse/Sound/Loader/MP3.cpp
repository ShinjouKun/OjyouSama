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
		assert("open 失敗");
	}

	//ファイル全体のサイズ
	//mFileSize = std::filesystem::file_size(fileName);
	mFileSize = getFileSize(fileName.c_str());
	if (mFileSize == 0)
	{
		//ファイルサイズの取得に失敗
		assert(false);
	}

	//ファイルを解析する
	if (!parse())
	{
		assert("解析失敗");
		return false;
	}

	//解析したデータをMPEGLAYER3WAVEFORMATに詰め込む
	MPEGLAYER3WAVEFORMAT mp3wf = { 0 };
	mFrameHeader->setDataToMpegLayer3WaveFormat(&mp3wf);

	//mp3からpcmへデータ変換する
	format->wFormatTag = WAVE_FORMAT_PCM;
	format->cbSize = sizeof(WAVEFORMATEX);
	mFormatConverter->mp3ToPCM(&format, mp3wf);

	//データサイズをデコード
	mFormatConverter->decodeSize(mDataSize, &mDataSize);

	//ファイルポインタをデータの先頭に戻しておく
	seekBegin();

	return true;

}

long MP3::read(BYTE ** buffer, unsigned size)
{
	//pcm用のサイズからmp3用のサイズに変更する
	unsigned decodeSize = 0;
	mFormatConverter->decodeSize(size, &decodeSize, false);
	//mp3用のサイズからデータを読み取る
	mIfs.read(reinterpret_cast<char*>(*buffer), decodeSize);
	//mp3のバッファからpcmのバッファに変更する
	auto count = mIfs.gcount();
	auto readSize = mFormatConverter->convert(buffer, count);

	return readSize;
}

void MP3::seek(long offset, Seek seek)
{
	//pcm用のサイズからmp3用のサイズに変更する
	unsigned decodeOffset = 0;
	mFormatConverter->decodeSize(offset, &decodeOffset, false);

	//フラグをクリアしてシーク
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
			//すでに解析済みなら次へ
			if (initFrameHeader)
			{
				continue;
			}
			//フレームヘッダの解析をする
			initFrameHeader = mFrameHeader->parseFrameHeader(header);
		}
		else 
		{
			//すでに解析済みなら次へ
			if (initData)
			{
				continue;
			}
			//データ部分の解析をする
			initData = parseData(header);
		}

		//必要な解析が完了していたら終了
		if (initFrameHeader && initData)
		{
			return true;
		}
	}

	//ID3v1の可能性
	mIfs.seekg(0, std::ios_base::end - ID3V1_END_SIZE);
	mIfs.read(reinterpret_cast<char*>(header), ID3V1_END_SIZE);
	return parseData2(header);

	//解析失敗
	return false;
}

bool MP3::parseData(const BYTE * header)
{
	//ID3v2である
	if (memcmp(header, "ID3", 3) == 0) {
		unsigned tagSize = ((header[6] << 21) | (header[7] << 14) | (header[8] << 7) | (header[9])) + ID3V2_HEADER_SIZE;
		//データ部分へのオフセット値決定
		mDataOffset = tagSize;
		//データ部分のサイズ決定
		mDataSize = mFileSize - mDataOffset;

		return true;
	}
	/*
	//ID3v1である
	else
	{
		if (memcmp(header, "TAG", 3) == 0)
		{
			//末尾のタグを省く
			mDataSize = mFileSize - ID3V1_END_SIZE;
		}
		else
		{
			//ファイル全体がMP3データ
			mDataSize = mFileSize;
		}
		return true;
	}
	*/
	return false;
}

bool MP3::parseData2(const BYTE * header)
{
	//ID3v1である
	char s[ID3V1_END_SIZE];
	memcpy(s, header, ID3V1_END_SIZE);//byteをchar[]に変換
	std::string str(s, sizeof(s) / sizeof(s[0]));//char[]をstringに変換

	if (str.find("TAG") != std::string::npos)
	{
		//末尾のタグを省く
		mDataSize = mFileSize - ID3V1_END_SIZE;
		return true;
	}

	mDataSize = mFileSize;
	return true;
}
