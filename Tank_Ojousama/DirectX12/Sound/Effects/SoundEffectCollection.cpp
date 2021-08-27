#include "SoundEffectCollection.h"
#include "PlayerTimer/PlayerTimer.h"
#include "Reverb/Reverb.h"
#include <cassert>

SoundEffectCollection::SoundEffectCollection(IVoice & voice, IEffectCreater & effectCreater, IEffectParameter & effectParameter):
	mEffectCreater(effectCreater)
{
}

SoundEffectCollection::~SoundEffectCollection() = default;

int SoundEffectCollection::reverb()
{
	Reverb reverb;
	return mEffectCreater.createEffect(&reverb);
}

int SoundEffectCollection::simpleReverb()
{
	return 0;
}

int SoundEffectCollection::echo()
{
	return 0;
}

int SoundEffectCollection::equalizer()
{
	return 0;
}

int SoundEffectCollection::volumeMeter()
{
	return 0;
}

void SoundEffectCollection::getPlayTimer()
{
	auto id = mEffectCreater.createEffect(reinterpret_cast<IUnknown*>(new PlayTimer()));
	//assert(id == PLAY_TIMER_ID);
}

int SoundEffectCollection::discreteFourierTransform()
{
	return 0;
}
