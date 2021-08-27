#include "SoundCreater.h"
#include "../Loader/MP3.h"
#include "../Loader/WAV.h"
#include "../Voice/SourceVoice.h"
#include "../Voice/SubmixVoice.h"
#include "../XAudio2/SoundBase.h"
#include "../XAudio2/XAudio2.h"
#include "../Util/FileUtil.h"

#include <cassert>

SoundCreater::SoundCreater(const SoundBase& base):
	mSoundBase(base)
{
}

SoundCreater::~SoundCreater() = default;

std::shared_ptr<SourceVoice> SoundCreater::createSourceVoice(const std::string & filePath, const SourceVoiceInitParam & param) const
{
	if (mSoundBase.isNull())
	{
		return nullptr;
	}

	auto loader = createLoaderFromFilePath(filePath);
	if (!loader)
	{
		return nullptr;
	}

	WAVEFORMATEX format = { 0 };
	auto res = loader->loadFromFile(&format, filePath);
	if (!res)
	{
		return nullptr;
	}

	return mSoundBase.getXAudio2().createSourceVoice(mSoundBase.getMasteringVoice(), loader, format, param);
}

std::shared_ptr<SubmixVoice> SoundCreater::createSubmixVoice(const SubmixVoiceInitParam & param) const
{
	//サウンドAPIが使用できない状態ならnullptrを返す
	if (mSoundBase.isNull())
	{
		return nullptr;
	}

	return mSoundBase.getXAudio2().createSubmixVoice(mSoundBase.getMasteringVoice(), param);
}

std::unique_ptr<ISoundLoader> SoundCreater::createLoaderFromFilePath(const std::string & filePath) const
{
	std::unique_ptr<ISoundLoader> loader = nullptr;
	auto ext = FileUtil::getFileExtension(filePath);
	if (ext == ".wav")
	{
		loader = std::make_unique<WAV>();
	}
	else if (ext == ".mp3")
	{
		loader = std::make_unique<MP3>();
	}
	else
	{
		assert("対応していない拡張子です");
	}

	return loader;
}
