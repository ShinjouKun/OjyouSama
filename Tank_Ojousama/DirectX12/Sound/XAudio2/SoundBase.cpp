#include "SoundBase.h"
#include "XAudio2.h"
#include "../Voice/MasteringVoice.h"
#include <xaudio2.h>
#include <cassert>

SoundBase::SoundBase() :
	mXAudio2(nullptr),
	mMasteringVoice(nullptr),
	mSucceedInitialize(false)
{
	//インスタンスを1つに制御
	assert(!mInstantiated);
	mInstantiated = true;

	mSucceedInitialize = comInitialize();
	if (!mSucceedInitialize)
	{
		return;
	}

	mXAudio2 = new XAudio2(&mSucceedInitialize);
	mMasteringVoice = mXAudio2->createMasteringVoice();
	if (!mMasteringVoice) 
	{
		CoUninitialize();
		mSucceedInitialize = false;
	}

}

SoundBase::~SoundBase() {
	delete(mMasteringVoice);
	mMasteringVoice = nullptr;
	delete(mXAudio2);
	mXAudio2 = nullptr;

	if (!mSucceedInitialize) {
		CoUninitialize();
	}

	mInstantiated = false;
}

XAudio2& SoundBase::getXAudio2() const 
{
	return *mXAudio2;
}

MasteringVoice& SoundBase::getMasteringVoice() const 
{
	return *mMasteringVoice;
}

bool SoundBase::isNull() const
{
	return !mSucceedInitialize;
}

bool SoundBase::comInitialize() 
{
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) 
	{
		//Debug::logError("Failed sound com initialized.");
		return false;
	}
	return true;
}

bool SoundBase::mInstantiated = false;