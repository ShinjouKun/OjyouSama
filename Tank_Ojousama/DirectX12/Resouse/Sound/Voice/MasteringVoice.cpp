#include "MasteringVoice.h"
#include "../Effects/SoundEffect.h"
#include "OutputVoices.h"
#include "../Volume/SoundVolume.h"


MasteringVoice::MasteringVoice(IXAudio2MasteringVoice * XAudio2MasteringVoice):
	mXAudio2MasteringVoice(XAudio2MasteringVoice),
	mDetails(),
	mSoundVolume(nullptr),
	mOutputVoices(std::make_unique<OutputVoices>(*this)),
	mSoundEffect(std::make_unique<SoundEffect>(*this))
{
	//XAudio2�̕W���@�\����{�C�X�����擾
	XAUDIO2_VOICE_DETAILS details = { 0 };
	mXAudio2MasteringVoice->GetVoiceDetails(&details);
	mDetails.channels = details.InputChannels;
	mDetails.sampleRate = details.InputSampleRate;

	//�T�E���h�{�����[���𐶐�
	mSoundVolume = std::make_unique<SoundVolume>(*this, mDetails.channels, mDetails.channels);
}

MasteringVoice::~MasteringVoice()
{
	mXAudio2MasteringVoice->DestroyVoice();
	mXAudio2MasteringVoice = nullptr;
}

IXAudio2Voice* MasteringVoice::getXAudio2Voice() const
{
	return mXAudio2MasteringVoice;
}

const VoiceDetails & MasteringVoice::getVoiceDetails() const
{
	return mDetails;
}

SoundVolume & MasteringVoice::getSoundVolume() const
{
	return *mSoundVolume;
}

OutputVoices & MasteringVoice::getOutputVoices() const
{
	return *mOutputVoices;
}

SoundEffect & MasteringVoice::getSoundEffect() const
{
	return *mSoundEffect;
}

unsigned MasteringVoice::getChannelMask() const
{
	DWORD channnelMask = 0;
	mXAudio2MasteringVoice->GetChannelMask(&channnelMask);
	return static_cast<unsigned>(channnelMask);
}
