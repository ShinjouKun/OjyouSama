#pragma once
#include "IEffectCreater.h"
#include "IEffectParameter.h"
#include "ISoundEffect.h"
#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

class SoundEffectCollection;

class SoundEffect :public IEffectCreater, public IEffectParameter
{
public:
	SoundEffect(IVoice& voice);
	~SoundEffect();

	//エフェクトにパラメーターを設定
	virtual void setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize) override;

	//エフェクトのパラメーターを取得
	virtual void getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const override;

	//エフェクトを有効化する
	void enable(int effectID);

	//エフェクトを無効化する
	void disable(int effectID);

	//エフェクトの有効化状態を返す
	bool getEnabled(int effectID);

	//エフェクト集にアクセスする
	SoundEffectCollection& getEffectCollection() const;

private:
	SoundEffect(const SoundEffect&) = delete;
	SoundEffect& operator=(const SoundEffect&) = delete;

	//エフェクトを生成する
	virtual int createEffect(ISoundEffect* target, bool isApply = true) override;
	virtual int createEffect(IUnknown* target, bool isApply = true) override;
	//作成した全エフェクトを適用する
	void apply();
	//IDが有効か
	bool isValidID(int effectID) const;
	//パラメータが有効か
	bool isValidParameters(const void* parameters) const;
	//エフェクトにアクセスできるか
	bool canAccessEffect(int effectID, const void* parameters) const;

private:
	IVoice& mVoice;
	std::unique_ptr<SoundEffectCollection> mEffectCollection;
	std::vector<XAUDIO2_EFFECT_DESCRIPTOR> mDescs;
	bool mIsApplied;
};