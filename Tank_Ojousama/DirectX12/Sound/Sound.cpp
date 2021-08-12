#include "Sound.h"
#include "SoundSystem.h"
#include "Voice/SourceVoice.h"
#include "Player/SoundPlayer.h"

Sound::Sound(std::string filename):
	mFilename(filename),
	mSourceVoice(nullptr)
{
	auto& s = SoundSystem::instance();
	SourceVoiceInitParam param;
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
	
}

void Sound::pause()
{
	mSourceVoice->getSoundPlayer().pause();
}

void Sound::stop()
{
	mSourceVoice->getSoundPlayer().stop();
}
