#include "SoundManager.h"
#include "Voice/SourceVoice.h"
#include "3D/Sound3DCalculator.h"
#include "3D/Sound3DListener/Sound3DListener.h"

SoundManager::SoundManager(const MasteringVoice& masteringVoice):
	mCalculator(std::make_unique<Sound3DCalculator>(masteringVoice)),
	mListener(nullptr)
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
	//不要なサブミックスボイスを削除
	auto itr2 = mSubmixVoices.begin();
	while (itr2 != mSubmixVoices.end())
	{
		if (itr2->use_count() == 1)
		{
			itr2 = mSubmixVoices.erase(itr2);
		}
		else
		{
			++itr2;
		}
	}

	//設定されているリスナーの更新
	if (mListener)
	{
		mListener->update();
	}

	//全ソースボイスの更新
	for (const auto& sound : mSounds)
	{
		sound->update();

		//3D演算処理
		if (sound->isCalculate3D() && mListener)
		{
			mCalculator->calculate(mListener->getListener(), sound->getEmitter());
		}
	}

}

void SoundManager::add(const std::shared_ptr<SourceVoice>& sound)
{
	mSounds.emplace_back(sound);
}

void SoundManager::add(const std::shared_ptr<SubmixVoice>& submixVoice)
{
	mSubmixVoices.emplace_back(submixVoice);
}

void SoundManager::setListener(const std::shared_ptr<Sound3DListener>& listener)
{
	mListener = listener;
}
