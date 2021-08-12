#pragma once

#include "VoiceDetails.h"
#include "IVoice.h"
#include <xaudio2.h>
#include <memory>

//�}�X�^�[�{�C�X
class MasteringVoice :public IVoice
{
public:
	MasteringVoice(IXAudio2MasteringVoice* XAudio2MasteringVoice);
	~MasteringVoice();

	virtual IXAudio2Voice* getXAudio2Voice()const override;
	virtual const VoiceDetails& getVoiceDetails() const override;

	//�`�����l���}�X�N��Ԃ�
	unsigned getChannelMask()const;

private:
	MasteringVoice(const MasteringVoice&) = delete;
	MasteringVoice& operator=(const MasteringVoice&) = delete;

private:
	IXAudio2MasteringVoice* mXAudio2MasteringVoice;
	VoiceDetails mDetails;

};