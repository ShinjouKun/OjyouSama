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

	//�ʒu��ݒ�
	void setPosition(const Vector3& pos);
	//�h�b�v���[���ʂ̊|������ݒ肷��
	void setDopplerScale(float scale);
	//�h�b�v���[���ʂ��v�Z���邩
	bool isCalculateDoppler() const;

	void setCalculateLPFDirect(bool value);
	bool isCalculateLPFDirect() const;

	void setCalculateReverb(bool value);
	bool isCalculateReverb() const;

	//�G�~�b�^�[��Ԃ�
	const Sound3DEmitterParam& getEmitter() const;

	DspSetter& getDspSetter() const;

private:
	Sound3DEmitter(const Sound3DEmitter&) = delete;
	Sound3DEmitter& operator=(const Sound3DEmitter&) = delete;

	void createReverbSubmixVoice(const MasteringVoice& masteringVoice);
	void initializeEmitter(const WaveFormat& format);

private:
	//�G�~�b�^�[
	Sound3DEmitterParam mEmitter;
	//���o�[�u�p�T�u�~�b�N�X�{�C�X
	std::shared_ptr<SubmixVoice> mReverb;
	//���o�[�uID
	int mReverbID;
	//���ʊp
	std::vector<float> mAzimuths;
	//�O�t���[���ł̈ʒu
	Vector3 mPreviousPos;
	//���[�p�X�t�B���^�v�Z�t���O
	bool mIsCalculateLPFDirect;
	//3D�v�Z����
	std::unique_ptr<DspSetter> mDspSetter;

	//�G�~�b�^�[�R�[��
	static Sound3DCone EMITTER_CONE;
	//�ʏ�J�[�u
	static DistanceCurvePoint DEFAULT_LINEAR_CURVE_POINTS[2];
	static DistanceCurve DEFAULT_LINEAR_CURVE;
	//LFE�J�[�u
	static DistanceCurvePoint EMITTER_LFE_CURVE_POINTS[3];
	static DistanceCurve EMITTER_LFE_CURVE;
	//���o�[�u�J�[�u
	static DistanceCurvePoint EMITTER_REVERB_CURVE_POINTS[3];
	static DistanceCurve EMITTER_REVERB_CURVE;
	/*
	//�G�~�b�^�[�R�[��
	static inline Sound3DCone EMITTER_CONE = { 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f };
	//�ʏ�J�[�u
	static inline DistanceCurvePoint DEFAULT_LINEAR_CURVE_POINTS[2] = { 0.f, 1.f, 1.f, 0.f };
	static inline DistanceCurve DEFAULT_LINEAR_CURVE = { &DEFAULT_LINEAR_CURVE_POINTS[0], 2 };
	//LFE�J�[�u
	static inline DistanceCurvePoint EMITTER_LFE_CURVE_POINTS[3] = { 0.f, 1.f, 0.25f, 0.f, 1.f, 0.f };
	static inline DistanceCurve EMITTER_LFE_CURVE = { &EMITTER_LFE_CURVE_POINTS[0], 3 };
	//���o�[�u�J�[�u
	static inline DistanceCurvePoint EMITTER_REVERB_CURVE_POINTS[3] = { 0.f, 0.5f, 0.75f, 1.f, 1.f, 0.f };
	static inline DistanceCurve EMITTER_REVERB_CURVE = { &EMITTER_LFE_CURVE_POINTS[0], 3 };
	*/
};
