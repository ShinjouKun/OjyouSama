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

//3Dサウンドを反映するクラス
class DspSetter
{
public:
	DspSetter(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const SubmixVoice& reverb, const WaveFormat& format);
	~DspSetter();

	//ソースボイスに3D演算した結果を適応
	void applyToSourceVoice(const Flag& flag);

	X3DAUDIO_DSP_SETTINGS& getDspSetting() const;

private:
	DspSetter(const DspSetter&) = delete;
	DspSetter& operator=(const DspSetter&) = delete;

private:
	SourceVoice& mSourceVoice;
	const MasteringVoice& mMasteringVoice;
	//リバーブ用サブミックスボイス
	const SubmixVoice& mReverb;
	//計算結果
	std::unique_ptr<X3DAUDIO_DSP_SETTINGS> mDspSettings;
	//行列係数
	std::vector<float> mMainVolumes;
	std::vector<float> mReverbVolumes;

};