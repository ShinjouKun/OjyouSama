#include "SoundSystem.h"
#include "XAudio2/SoundBase.h"
#include "SoundManager.h"
#include "Factory/SoundCreater.h"
#include "Voice/SourceVoice.h"

SoundSystem::SoundSystem():
	mBase(std::make_unique<SoundBase>()),
	mCreater(std::make_unique<SoundCreater>(*mBase)),
	mManager(std::make_unique<SoundManager>(mBase->getMasteringVoice()))
{

}

SoundSystem::~SoundSystem() = default;

SoundSystem& SoundSystem::instance()
{
	if (!mInstance)
	{
		mInstance = new SoundSystem();
	}
	return *mInstance;
}

void SoundSystem::finalize()
{
	delete(mInstance);
	mInstance = nullptr;
}

void SoundSystem::update()
{
	mManager->update();
}

std::shared_ptr<SourceVoice> SoundSystem::createSourceVoice(const std::string & fileName, const SourceVoiceInitParam & param, const std::string & directoryPath) const
{
	auto sourceVoice = mCreater->createSourceVoice(directoryPath + fileName, param);
	mManager->add(sourceVoice);
	return sourceVoice;
}

const SoundBase & SoundSystem::getBase() const
{
	return *mBase;
}

SoundSystem* SoundSystem::mInstance = nullptr;