#include "SoundPlayTimer.h"
#include "SoundPlayer.h"
#include "../Voice/SourceVoice.h"

SoundPlayTimer::SoundPlayTimer(SourceVoice & sourceVoice, SoundPlayer & player):
	mSourceVoice(sourceVoice),
	mPlayer(player)
{
}

SoundPlayTimer::~SoundPlayTimer() = default;

void SoundPlayTimer::setPlayTime(float time)
{
	
}

void SoundPlayTimer::setFrequency(float getFrequency)
{
}

float SoundPlayTimer::getPlayTime() const
{
	return 0.0f;
}

void SoundPlayTimer::startTimer()
{
}

void SoundPlayTimer::stopTimer()
{
}
