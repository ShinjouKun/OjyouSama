#pragma once

#include "SoundBuffer.h"
#include <xaudio2.h>

class SourceVoice;

//ソースボイスにバッファを登録するクラス
class BufferSubmitter 
{
public:
	BufferSubmitter(SourceVoice& sourceVoice);
	~BufferSubmitter();

	void submitBuffer(const SoundBuffer& buffer) const;

	void submitSimpleBuffer(const SimpleSoundBuffer& buffer) const;

private:
	BufferSubmitter(const BufferSubmitter&) = delete;
	BufferSubmitter& operator=(const BufferSubmitter&) = delete;

	//波形データの登録
	void submitSourceBuffer(const XAUDIO2_BUFFER& buffer) const;
	//SoundBufferからXAUDIO2_BUFFERへの変換
	XAUDIO2_BUFFER toBuffer(const SoundBuffer& buffer) const;
	//SimpleSoundBufferからXAUDIO2_BUFFERへの変換
	XAUDIO2_BUFFER toBuffer(const SimpleSoundBuffer& buffer) const;

private:
	SourceVoice& mSourceVoice;
};
