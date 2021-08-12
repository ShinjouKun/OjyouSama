#include "SoundManager.h"
#include "Voice/SourceVoice.h"

SoundManager::SoundManager(const MasteringVoice& masteringVoice)
{
}

SoundManager::~SoundManager()
{
	mSounds.clear();
}

void SoundManager::update()
{
	//不要なソースボイスを削除
	auto itr = mSounds.begin();
	while (itr != mSounds.end())
	{
		if (itr->use_count() == 1)
		{
			itr = mSounds.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	//全ソースボイスの更新
	for (const auto& sound : mSounds)
	{
		sound->update();

		//3D演算処理

	}

}

void SoundManager::add(const std::shared_ptr<SourceVoice>& sound)
{
	mSounds.emplace_back(sound);
}