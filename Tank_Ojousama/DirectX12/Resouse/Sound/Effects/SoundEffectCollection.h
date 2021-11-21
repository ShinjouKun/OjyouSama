#pragma once

#include "IEffectCreater.h"
#include "IEffectParameter.h"
#include "../Voice/IVoice.h"
#include <memory>

//class SoundFilter;

//全てのエフェクトにアクセスできる
class SoundEffectCollection
{
public:
	SoundEffectCollection(IVoice& voice, IEffectCreater& effectCreater, IEffectParameter& effectParameter);
	~SoundEffectCollection();

	//リバーブ効果
	int reverb();
	//簡易版リバーブ
	int simpleReverb();
	//エコーをかける
	int echo();
	//イコライザー
	int equalizer();
	//ボリュームメーター
	int volumeMeter();
	//再生時間設定/取得
	void getPlayTimer();
	//フーリエ変換
	int discreteFourierTransform();
	//フィルター管理クラスを返す
	//SoundFilter& getFilter() const;
private:
	SoundEffectCollection(const SoundEffectCollection&) = delete;
	SoundEffectCollection& operator=(const SoundEffectCollection&) = delete;

public:
	static constexpr int PLAY_TIMER_ID = 0;

private:
	IEffectCreater& mEffectCreater;
	//std::unique_ptr<SoundFilter> mFilter;
};