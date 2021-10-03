#include "Sound.h"
#include "SoundSystem.h"
#include "SoundManager.h"
#include "Voice/SourceVoice.h"
#include "Player/SoundPlayer.h"
#include "Volume/SoundVolume.h"
#include "3D/Emitter/Sound3DEmitter.h"

Sound::Sound(std::string filename, bool isCalculate3D) :
	mFilename(filename),
	mSourceVoice(nullptr)
{
	auto& s = SoundSystem::instance();
	SourceVoiceInitParam param;
	param.isCalculate3D = isCalculate3D;
	mSourceVoice = s.createSourceVoice(mFilename, param);
}

Sound::~Sound()
{
}

void Sound::play()
{
	mSourceVoice->getSoundPlayer().playStreaming();
}

void Sound::playLoop()
{
	if(mSourceVoice->getSoundPlayer().isStop())
		mSourceVoice->getSoundPlayer().playStreaming();
}

void Sound::pause()
{
	mSourceVoice->getSoundPlayer().pause();
}

void Sound::stop()
{
	mSourceVoice->getSoundPlayer().stop();
}

void Sound::setVol(float num)
{
	mSourceVoice->getSoundVolume().setVolume(num);
}

void Sound::setPos(const Vector3 & pos)
{
	mSourceVoice->getEmitter().setPosition(pos);
}

void Sound::setCurveDistanceScaler(float num)
{
	mSourceVoice->getEmitter().setCurveDistanceScaler(num);
}
