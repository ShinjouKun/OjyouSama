#include "SoundPlayer.h"
#include "SoundLoop.h"
#include "SoundPlayTimer.h"
#include "../Data/SoundData.h"
#include "../Streaming/SoundStreaming.h"
#include "../Voice/SourceVoice.h"

SoundPlayer::SoundPlayer(SourceVoice & sourceVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat & format, float maxFrequencyRatio):
	mSourceVoice(sourceVoice),
	mStreaming(std::make_unique<SoundStreaming>(sourceVoice, *this, loader, format)),
	mPlayTimer(std::make_unique<SoundPlayTimer>(sourceVoice, *this)),
	mLoop(std::make_unique<SoundLoop>(sourceVoice, *this)),
	//mFrequency(std::make_unique<Frequency>(sourceVoice, *this, maxFrequencyRatio)),
	mIsPlay(false)
{
}

SoundPlayer::~SoundPlayer() = default;

void SoundPlayer::update()
{
	if (!mIsPlay)return;

	mLoop->update();
	mStreaming->polling();

	//�L���[������ۂȂ�
	XAUDIO2_VOICE_STATE state = { 0 };
	mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
	if (state.BuffersQueued == 0) 
	{
		//�Ȃ̍ŏ��ɖ߂�
		mStreaming->seek(0.f);
		pause();
	}
}

void SoundPlayer::playStreaming()
{
	mIsPlay = true;
	mStreaming->polling();
	mPlayTimer->startTimer();
	auto res = mSourceVoice.getXAudio2SourceVoice()->Start();
	//�G���[����

}

void SoundPlayer::setPlayPoint(float point)
{
	//�K���͈͂ɃN�����v
	float sec = mSourceVoice.getSoundData().clamp(point);

	mPlayTimer->setPlayTime(sec);
	mStreaming->seek(sec);
}

void SoundPlayer::pause()
{
	bool res = pauseAndStop(XAUDIO2_PLAY_TAILS);
	//�G���[����

}

void SoundPlayer::stop()
{
	bool res = pauseAndStop(0);
	//�G���[����

}

bool SoundPlayer::isStop() const
{
	return (!mIsPlay);
}

SoundPlayTimer & SoundPlayer::getPlayTimer() const
{
	return *mPlayTimer;
}

SoundLoop & SoundPlayer::getLoop() const
{
	return *mLoop;
}

bool SoundPlayer::pauseAndStop(unsigned flag)
{
	mIsPlay = false;
	mPlayTimer->stopTimer();
	auto res = mSourceVoice.getXAudio2SourceVoice()->Stop(flag);
	return (SUCCEEDED(res));
}
