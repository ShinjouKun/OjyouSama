#pragma once

#include "SourceVoiceInitParam.h"
#include "IVoice.h"
#include "../Data/SoundData.h"
#include "../Loader/ISoundLoader.h"
#include "../Loader/WaveFormat.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

class MasteringVoice;
class SoundPlayer;
class Sound3DEmitter;

//IXAudio2SourceVoice
class SourceVoice : public IVoice
{
public:
	SourceVoice(IXAudio2SourceVoice* XAudio2SourceVoice, MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WaveFormat& format, const SourceVoiceInitParam& param);
	~SourceVoice();

	virtual IXAudio2Voice* getXAudio2Voice() const override;
	virtual const VoiceDetails& getVoiceDetails() const override;
	virtual SoundVolume& getSoundVolume() const override;
	virtual OutputVoices& getOutputVoices() const override;
	virtual SoundEffect& getSoundEffect() const override;

	void update();
	IXAudio2SourceVoice* getXAudio2SourceVoice() const;
	//�T�E���h�f�[�^��Ԃ�
	SoundData& getSoundData() const;
	//�T�E���h�Đ��N���X��Ԃ�
	SoundPlayer& getSoundPlayer()const;
	//�G�~�b�^�[��Ԃ�
	Sound3DEmitter& getEmitter() const;
	//3D���Z���邩�ǂ���
	bool isCalculate3D() const;

private:
	SourceVoice(const SourceVoice&) = delete;
	SourceVoice& operator=(const SourceVoice&) = delete;

private:
	IXAudio2SourceVoice* mXAudio2SourceVoice;
	VoiceDetails mDetails;
	std::unique_ptr<SoundData> mSoundData;
	std::unique_ptr<SoundVolume> mSoundVolume;
	std::unique_ptr<OutputVoices> mOutputVoices;
	std::unique_ptr<SoundEffect> mSoundEffect;
	std::unique_ptr<SoundPlayer> mSoundPlayer;
	std::unique_ptr<Sound3DEmitter> mEmitter;
};