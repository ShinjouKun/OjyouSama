#include "SoundLoop.h"
#include "SoundPlayer.h"
#include "SoundPlayTimer.h"
#include "../Data/SoundData.h"
#include "../Voice/SourceVoice.h"
#include "../../Math/Math.h"

SoundLoop::SoundLoop(SourceVoice & sourceVoice, SoundPlayer & player):
	mPlayer(player),
	mSourceVoice(sourceVoice),
	mLoopBegin(0.f),
	mLoopEnd(0.f),
	mIsLoop(false)
{
}

SoundLoop::~SoundLoop() = default;

void SoundLoop::update()
{
	if (!mIsLoop)return;

	//���݂̍Đ����Ԃ����[�v�ܕԂ��n�_�𒴂��Ă����烋�[�v�̊J�n�n�_�ɖ߂�
	if (mPlayer.getPlayTimer().getPlayTime() >= mLoopEnd - 0.01f) 
	{
		mPlayer.setPlayPoint(mLoopBegin);
		return;
	}

	//��L�̊֐����ƌ덷�ŏI�[���[�v�ł��Ȃ��Ƃ�������
	//�L���[������ۂȂ�
	XAUDIO2_VOICE_STATE state = { 0 };
	mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
	if (state.BuffersQueued == 0) {
		//�Ȃ̍Ō�Ń��[�v���邩
		if (Math::equal(mLoopEnd, mSourceVoice.getSoundData().length())) 
		{
			mPlayer.setPlayPoint(mLoopBegin);
		}
	}
}

void SoundLoop::loopAll()
{
	setValue(0.f, mSourceVoice.getSoundData().length());
}

void SoundLoop::exitLoop()
{
	mIsLoop = false;
}

bool SoundLoop::isLoop() const
{
	return mIsLoop;
}

float SoundLoop::getLoopEnd() const
{
	return mLoopEnd;
}

void SoundLoop::setValue(float begin, float end)
{
	mLoopBegin = begin;
	mLoopEnd = end;
	mIsLoop = true;
}

void SoundLoop::clamp()
{
	const auto& data = mSourceVoice.getSoundData();
	mLoopBegin = data.clamp(mLoopBegin);
	mLoopEnd = data.clamp(mLoopEnd);

}
