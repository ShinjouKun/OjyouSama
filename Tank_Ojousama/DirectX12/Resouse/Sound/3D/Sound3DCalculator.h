#pragma once

#include "Sound3DListener/Sound3DListenerParam.h"
#include "../Flag/Flag.h"

#include <xaudio2.h>
#include <x3daudio.h>
#pragma comment(lib, "XAudio2.lib")

class MasteringVoice;
class Sound3DEmitter;

//3Dサウンド計算クラス
class Sound3DCalculator
{
public:
	Sound3DCalculator(const MasteringVoice& masteringVoice);
	~Sound3DCalculator();

	//3Dサウンド計算をする
	void calculate(const Sound3DListenerParam& listener, const Sound3DEmitter& emitter) const;

private:
	Sound3DCalculator(const Sound3DCalculator&) = delete;
	Sound3DCalculator& operator=(const Sound3DCalculator&) = delete;

private:
	//フラグを設定し返す
	Flag getFlagsFromEmitter(const Sound3DEmitter& emitter) const;

private:
	//3Dサウンドのハンドル
	X3DAUDIO_HANDLE mInstance;
	//初期化に成功しているか
	bool mInitialized;

};