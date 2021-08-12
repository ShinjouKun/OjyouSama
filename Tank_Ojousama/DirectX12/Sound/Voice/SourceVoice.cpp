#include "SourceVoice.h"
#include "VoiceDetails.h"
#include "MasteringVoice.h"
#include "../Player/SoundPlayer.h"

SourceVoice::SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, const SourceVoiceInitParam& param) :
	mXAudio2SourceVoice(XAudio2SourceVoice),
	mDetails{ format.channels,format.samplesPerSec },
	mSoundData(std::make_unique<SoundData>(*loader, format)),
	mSoundPlayer(std::make_unique<SoundPlayer>(*this, loader, format, param.maxFrequencyRatio))
{
	//3D‰‰ŽZ‚·‚é‚È‚ç
	if (param.isCalculate3D)
	{
		//‚±‚±‚Å¶¬
	}

}

SourceVoice::~SourceVoice() 
{
	mXAudio2SourceVoice->DestroyVoice();
	mXAudio2SourceVoice = nullptr;
}

IXAudio2Voice* SourceVoice::getXAudio2Voice() const 
{
	return mXAudio2SourceVoice;
}

const VoiceDetails& SourceVoice::getVoiceDetails() const 
{
	return mDetails;
}

void SourceVoice::update()
{
	mSoundPlayer->update();
}

IXAudio2SourceVoice* SourceVoice::getXAudio2SourceVoice() const 
{
	return mXAudio2SourceVoice;
}

SoundData& SourceVoice::getSoundData() const 
{
	return *mSoundData;
}

SoundPlayer& SourceVoice::getSoundPlayer() const
{
	return *mSoundPlayer;
}
