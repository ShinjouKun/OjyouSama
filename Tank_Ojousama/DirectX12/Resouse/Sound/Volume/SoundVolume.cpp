#include "SoundVolume.h"
#include "SoundPan.h"
#include "../../Math/Math.h"

SoundVolume::SoundVolume(IVoice & voice, unsigned inChannels, unsigned outChannels):
	mVoice(voice),
	mSoundPan(std::make_unique<SoundPan>(voice, inChannels, outChannels)),
	mCurrentVolume(1.f)
{
}

SoundVolume::~SoundVolume() = default;

void SoundVolume::update()
{
}

void SoundVolume::setVolume(float volume)
{
	//‘O‰ñ‚Æ‚Ù‚Ú“¯‚¶’l‚¾‚Á‚½‚ç•ÏX‚µ‚È‚¢
	if (Math::nearZero(mCurrentVolume - volume))
	{
		return;
	}

	mCurrentVolume = volume;
	float targetVolume = mCurrentVolume * mCurrentVolume;
	auto res = mVoice.getXAudio2Voice()->SetVolume(targetVolume);
}

float SoundVolume::getVolume() const
{
	return mCurrentVolume;
}

SoundPan & SoundVolume::getPan() const
{
	return *mSoundPan;
}
