#pragma once

#include "Voice/SourceVoiceInitParam.h"
#include <memory>
#include <string>

class SoundBase;
class SourceVoice;
class SoundCreater;
class SoundManager;

class SoundSystem
{
private:
	SoundSystem();
public:
	~SoundSystem();
	//インスタンスを返す
	static SoundSystem& instance();
	//終了処理
	void finalize();
	//更新
	void update();
	//リスナーの登録
	//void setListener(const std::shared_ptr<Sound3DListener>& listener);
	
	std::shared_ptr<SourceVoice> createSourceVoice(const std::string& fileName, const SourceVoiceInitParam& param, const std::string& directoryPath = "Resouse\\Sound\\") const;

	const SoundBase& getBase() const;

private:
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&&) = delete;
	SoundSystem& operator=(SoundSystem&&) = delete;

private:
	std::unique_ptr<SoundBase> mBase;
	std::unique_ptr<SoundCreater> mCreater;
	std::unique_ptr<SoundManager> mManager;

	static SoundSystem* mInstance;
};