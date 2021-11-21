#pragma once

#include "../../Loader/WaveFormat.h"
#include "../../Flag/Flag.h"
#include <xaudio2.h>
#include <x3daudio.h>
#include <memory>
#include <vector>

class SourceVoice;
class MasteringVoice;
class SubmixVoice;

//3D�T�E���h�𔽉f����N���X
class DspSetter
{
public:
	DspSetter(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const SubmixVoice& reverb, const WaveFormat& format);
	~DspSetter();

	//�\�[�X�{�C�X��3D���Z�������ʂ�K��
	void applyToSourceVoice(const Flag& flag);

	X3DAUDIO_DSP_SETTINGS& getDspSetting() const;

private:
	DspSetter(const DspSetter&) = delete;
	DspSetter& operator=(const DspSetter&) = delete;

private:
	SourceVoice& mSourceVoice;
	const MasteringVoice& mMasteringVoice;
	//���o�[�u�p�T�u�~�b�N�X�{�C�X
	const SubmixVoice& mReverb;
	//�v�Z����
	std::unique_ptr<X3DAUDIO_DSP_SETTINGS> mDspSettings;
	//�s��W��
	std::vector<float> mMainVolumes;
	std::vector<float> mReverbVolumes;

};