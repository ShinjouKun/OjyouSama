#pragma once

#include "VoiceDetails.h"
#include "IVoice.h"
#include <xaudio2.h>
#include <memory>

//�}�X�^�[�{�C�X
class MasteringVoice : public IVoice
{
public:
	MasteringVoice(IXAudio2MasteringVoice* XAudio2MasteringVoice);
	~MasteringVoice();

	virtual IXAudio2Voice* getXAudio2Voice()const override;
	virtual const VoiceDetails& getVoiceDetails() const override;
	virtual SoundVolume& getSoundVolume() const override;
	virtual OutputVoices& getOutputVoices() const override;
	virtual SoundEffect& getSoundEffect() const override;

	//�`�����l���}�X�N��Ԃ�
	unsigned getChannelMask()const;

private:
	MasteringVoice(const MasteringVoice&) = delete;
	MasteringVoice& operator=(const MasteringVoice&) = delete;

private:
	IXAudio2MasteringVoice* mXAudio2MasteringVoice;
	VoiceDetails mDetails;
	std::unique_ptr<SoundVolume> mSoundVolume;
	std::unique_ptr<OutputVoices> mOutputVoices;
	std::unique_ptr<SoundEffect> mSoundEffect;
};