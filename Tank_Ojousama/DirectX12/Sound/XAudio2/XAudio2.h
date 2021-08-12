#pragma once

#include "../Loader/ISoundLoader.h"
#include "../Voice/SourceVoiceInitParam.h"
#include <xaudio2.h>
#include <memory>

#include <d3d12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

class MasteringVoice;
class SourceVoice;

//IXAudio2
class XAudio2
{
public:
	XAudio2(bool* successFlag);
	~XAudio2();

	//Make a MasterVoice
	MasteringVoice* createMasteringVoice() const;

	//Make a SouceVoice
	std::unique_ptr<SourceVoice> createSourceVoice(MasteringVoice& masteringVoice, std::unique_ptr<ISoundLoader>& loader, const WAVEFORMATEX& format, const SourceVoiceInitParam& param) const;

private:
	XAudio2(const XAudio2&) = delete;
	XAudio2& operator=(const XAudio2&) = delete;

	//Make a XAudio2
	bool createXAudio2();

private:
	ComPtr<IXAudio2> mXAudio2;

	static bool mInstantiated;
};