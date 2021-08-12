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
	//�s�v�ȃ\�[�X�{�C�X���폜
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
	//�S�\�[�X�{�C�X�̍X�V
	for (const auto& sound : mSounds)
	{
		sound->update();

		//3D���Z����

	}

}

void SoundManager::add(const std::shared_ptr<SourceVoice>& sound)
{
	mSounds.emplace_back(sound);
}