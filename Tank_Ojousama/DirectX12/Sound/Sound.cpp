#include "Sound.h"
#include "SoundSystem.h"
#include "SoundManager.h"
#include "Voice/SourceVoice.h"
#include "Player/SoundPlayer.h"
#include "Volume/SoundVolume.h"
#include "3D/Sound3DListener/Sound3DListener.h"
#include "3D/Emitter/Sound3DEmitter.h"

Sound::Sound(std::string filename, bool isCalculate3D) :
	mFilename(filename),
	mSourceVoice(nullptr),
	mListener(nullptr)
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

void Sound::setLisner(const Vector3 & pos)
{
	if (!mListener)
	{
		mListener = std::make_shared<Sound3DListener>();
		auto& s = SoundSystem::instance();
		s.setListener(mListener);
	}
	mListener->setPosition(pos);
}

void Sound::setPos(const Vector3 & pos)
{
	mSourceVoice->getEmitter().setPosition(pos);
}
