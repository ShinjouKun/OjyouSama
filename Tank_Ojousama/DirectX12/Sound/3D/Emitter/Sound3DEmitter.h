#pragma once

#include "Sound3DEmitterParam.h"
#include "../DistanceCurve.h"
#include "../Sound3DCone.h"
#include "../../Loader/WaveFormat.h"
#include "../../../Math/Math.h"
#include <memory>
#include <vector>

class SourceVoice;
class MasteringVoice;
class SubmixVoice;
class DspSetter;

class Sound3DEmitter
{
public:
	Sound3DEmitter(SourceVoice& sourceVoice, const MasteringVoice& masteringVoice, const WaveFormat& format);
	~Sound3DEmitter();

	void update();

	//位置を設定
	void setPosition(const Vector3& pos);
	//ドップラー効果の掛かり具合を設定する
	void setDopplerScale(float scale);
	//ドップラー効果を計算するか
	bool isCalculateDoppler() const;

	void setCalculateLPFDirect(bool value);
	bool isCalculateLPFDirect() const;

	void setCalculateReverb(bool value);
	bool isCalculateReverb() const;

	//エミッターを返す
	const Sound3DEmitterParam& getEmitter() const;

	DspSetter& getDspSetter() const;

private:
	Sound3DEmitter(const Sound3DEmitter&) = delete;
	Sound3DEmitter& operator=(const Sound3DEmitter&) = delete;

	void createReverbSubmixVoice(const MasteringVoice& masteringVoice);
	void initializeEmitter(const WaveFormat& format);

private:
	//エミッター
	Sound3DEmitterParam mEmitter;
	//リバーブ用サブミックスボイス
	std::shared_ptr<SubmixVoice> mReverb;
	//リバーブID
	int mReverbID;
	//方位角
	std::vector<float> mAzimuths;
	//前フレームでの位置
	Vector3 mPreviousPos;
	//ローパスフィルタ計算フラグ
	bool mIsCalculateLPFDirect;
	//3D計算結果
	std::unique_ptr<DspSetter> mDspSetter;

	//エミッターコーン
	static Sound3DCone EMITTER_CONE;
	//通常カーブ
	static DistanceCurvePoint DEFAULT_LINEAR_CURVE_POINTS[2];
	static DistanceCurve DEFAULT_LINEAR_CURVE;
	//LFEカーブ
	static DistanceCurvePoint EMITTER_LFE_CURVE_POINTS[3];
	static DistanceCurve EMITTER_LFE_CURVE;
	//リバーブカーブ
	static DistanceCurvePoint EMITTER_REVERB_CURVE_POINTS[3];
	static DistanceCurve EMITTER_REVERB_CURVE;
	/*
	//エミッターコーン
	static inline Sound3DCone EMITTER_CONE = { 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f };
	//通常カーブ
	static inline DistanceCurvePoint DEFAULT_LINEAR_CURVE_POINTS[2] = { 0.f, 1.f, 1.f, 0.f };
	static inline DistanceCurve DEFAULT_LINEAR_CURVE = { &DEFAULT_LINEAR_CURVE_POINTS[0], 2 };
	//LFEカーブ
	static inline DistanceCurvePoint EMITTER_LFE_CURVE_POINTS[3] = { 0.f, 1.f, 0.25f, 0.f, 1.f, 0.f };
	static inline DistanceCurve EMITTER_LFE_CURVE = { &EMITTER_LFE_CURVE_POINTS[0], 3 };
	//リバーブカーブ
	static inline DistanceCurvePoint EMITTER_REVERB_CURVE_POINTS[3] = { 0.f, 0.5f, 0.75f, 1.f, 1.f, 0.f };
	static inline DistanceCurve EMITTER_REVERB_CURVE = { &EMITTER_LFE_CURVE_POINTS[0], 3 };
	*/
};
