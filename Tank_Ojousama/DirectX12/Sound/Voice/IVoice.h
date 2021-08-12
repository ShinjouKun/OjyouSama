#pragma once

#include "VoiceDetails.h"
#include <xaudio2.h>
//ボイスインターフェース
class IVoice
{
public:
	virtual ~IVoice() = default;
	virtual IXAudio2Voice* getXAudio2Voice()const = 0;
	virtual const VoiceDetails& getVoiceDetails() const = 0;
};