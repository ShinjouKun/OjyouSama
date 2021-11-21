#include "DspSetter.h"
#include "../../Player/SoundPlayer.h"
#include "../../Voice/VoiceDetails.h"
#include "../../Voice/OutputVoices.h"
#include "../../Voice/MasteringVoice.h"
#include "../../Voice/SourceVoice.h"
#include "../../Voice/SubmixVoice.h"
#include "../../Volume/SoundPan.h"
#include "../../Volume/SoundVolume.h"
#include "../../../Math/Math.h"
#include <algorithm>

DspSetter::DspSetter(SourceVoice & sourceVoice, const MasteringVoice & masteringVoice, const SubmixVoice & reverb, const WaveFormat & format):
	mSourceVoice(sourceVoice),
	mMasteringVoice(masteringVoice),
	mReverb(reverb),
	mDspSettings(std::make_unique<X3DAUDIO_DSP_SETTINGS>())
{
	//�}�X�^�[�{�C�X���o�͐�Ɏw��
	mSourceVoice.getOutputVoices().addOutputVoice(masteringVoice, true);

	const auto inCh = format.channels;
	const auto outCh = masteringVoice.getVoiceDetails().channels;

	//DSP�ɕK�v�Ȓl��ݒ�
	mDspSettings->SrcChannelCount = inCh;
	mDspSettings->DstChannelCount = outCh;
	mMainVolumes.resize(inCh * outCh);
	mDspSettings->pMatrixCoefficients = mMainVolumes.data();
	mDspSettings->pDelayTimes = nullptr;

	//���o�[�u�{�����[��
	mReverbVolumes.resize(outCh);
}

DspSetter::~DspSetter() = default;

void DspSetter::applyToSourceVoice(const Flag & flag)
{
	//3D�v�Z
	if (flag.check(X3DAUDIO_CALCULATE_LPF_DIRECT))
	{
		mSourceVoice.getSoundVolume().getPan().panOutputVoice(mMasteringVoice, mMainVolumes.data());
	}
	//���[�p�X�t�B���^���ڃp�X
	if (flag.check(X3DAUDIO_CALCULATE_LPF_DIRECT))
	{
		XAUDIO2_FILTER_PARAMETERS param = { XAUDIO2_FILTER_TYPE::LowPassFilter, 2.f * sinf(Math::PI / 6.f * mDspSettings->LPFDirectCoefficient), 1.414f };
		mSourceVoice.getXAudio2Voice()->SetOutputFilterParameters(mMasteringVoice.getXAudio2Voice(), &param);
	}
	//���o�[�u
	if (flag.check(X3DAUDIO_CALCULATE_REVERB))
	{
		std::fill(mReverbVolumes.begin(), mReverbVolumes.end(), mDspSettings->ReverbLevel);
		mSourceVoice.getSoundVolume().getPan().panOutputVoice(mReverb, mReverbVolumes.data());
	}
	//���[�J���t�B���^���o�[�u�p�X
	if (flag.check(X3DAUDIO_CALCULATE_LPF_REVERB))
	{
		XAUDIO2_FILTER_PARAMETERS param = { XAUDIO2_FILTER_TYPE::LowPassFilter, 2.f * sinf(Math::PI / 6.f * mDspSettings->LPFReverbCoefficient), 1.414f };
		mSourceVoice.getXAudio2Voice()->SetOutputFilterParameters(mReverb.getXAudio2Voice(), &param);
	}
	//�h�b�v���[
	if (flag.check(X3DAUDIO_CALCULATE_DOPPLER))
	{
		//mSourceVoice.getSoundPlayer().getFrequency().setFrequencyRatio(mDspSettings->DopplerFactor);
	}
}

X3DAUDIO_DSP_SETTINGS & DspSetter::getDspSetting() const
{
	return *mDspSettings;
}
