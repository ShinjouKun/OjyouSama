#include "SourceVoice.h"
#include "VoiceDetails.h"
#include "MasteringVoice.h"
#include "OutputVoices.h"
#include "../3D/Emitter/Sound3DEmitter.h"
#include "../Effects/SoundEffect.h"
#include "../Effects/SoundEffectCollection.h"
#include "../Player/SoundPlayer.h"
#include "../Volume/SoundVolume.h"

SourceVoice::SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, const SourceVoiceInitParam& param) :
	mXAudio2SourceVoice(XAudio2SourceVoice),
	mDetails{ format.channels,format.samplesPerSec },
	mSoundVolume(std::make_unique<SoundVolume>(*this, format.channels, masteringVoice.getVoiceDetails().channels)), 
	mSoundData(std::make_unique<SoundData>(*loader, format)),
	mOutputVoices(std::make_unique<OutputVoices>(*this)),
	mSoundEffect(std::make_unique<SoundEffect>(*this)),
	mSoundPlayer(std::make_unique<SoundPlayer>(*this, loader, format, param.maxFrequencyRatio)),
	mEmitter(nullptr)
{
	//3D���Z����Ȃ�
	if (param.isCalculate3D)
	{
		//�����Ő���
		mEmitter = std::make_unique<Sound3DEmitter>(*this, masteringVoice, format);
	}
	//�Đ����Ԑݒ�/�擾�̂��߂ɂ��ׂẴ\�[�X�{�C�X�ŃG�t�F�N�g�t�^
	mSoundEffect->getEffectCollection().getPlayTimer();
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

SoundVolume & SourceVoice::getSoundVolume() const
{
	return *mSoundVolume;
}

OutputVoices & SourceVoice::getOutputVoices() const
{
	return *mOutputVoices;
}

SoundEffect & SourceVoice::getSoundEffect() const
{
	return *mSoundEffect;
}

void SourceVoice::update()
{
	mSoundVolume->update();
	mSoundPlayer->update();
	if (mEmitter)
	{
		mEmitter->update();
	}
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

Sound3DEmitter & SourceVoice::getEmitter() const
{
	return *mEmitter;
}

bool SourceVoice::isCalculate3D() const
{
	return (mEmitter) ? true : false;
}
