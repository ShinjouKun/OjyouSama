#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Voice/SourceVoiceInitParam.h"
#include "../Voice/SubmixVoiceInitParam.h"
#include <xaudio2.h>
#include <memory>
#include <string>

class SoundBase;
class SourceVoice;
class SubmixVoice;

class SoundCreater
{
public:
	SoundCreater(const SoundBase& base);
	~SoundCreater();
	//ソースボイス生成
	std::shared_ptr<SourceVoice> createSourceVoice(const std::string& filePath, const SourceVoiceInitParam& param) const;
	//サブミックスボイスの生成
	std::shared_ptr<SubmixVoice> createSubmixVoice(const SubmixVoiceInitParam& param) const;

private:
	SoundCreater(const SoundCreater&) = delete;
	SoundCreater& operator=(const SoundCreater&) = delete;

	//拡張子を識別し、パーサーを生成する
	std::unique_ptr<ISoundLoader> createLoaderFromFilePath(const std::string& filePath) const;

private:
	const SoundBase& mSoundBase;
};