#include "MasteringVoice.h"

MasteringVoice::MasteringVoice(IXAudio2MasteringVoice * XAudio2MasteringVoice):
	mXAudio2MasteringVoice(XAudio2MasteringVoice),
	mDetails()
{
	//XAudio2�̕W���@�\����{�C�X�����擾
	XAUDIO2_VOICE_DETAILS details = { 0 };
	mXAudio2MasteringVoice->GetVoiceDetails(&details);
	mDetails.channels = details.InputChannels;
	mDetails.sampleRate = details.InputSampleRate;

	//�T�E���h�{�����[���𐶐�

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

unsigned MasteringVoice::getChannelMask() const
{
	DWORD channnelMask = 0;
	mXAudio2MasteringVoice->GetChannelMask(&channnelMask);
	return static_cast<unsigned>(channnelMask);
}
