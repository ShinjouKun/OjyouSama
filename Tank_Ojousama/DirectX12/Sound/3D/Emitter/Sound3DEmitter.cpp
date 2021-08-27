#include "Sound3DEmitter.h"
#include "DspSetter.h"
#include "../../Effects/SoundEffect.h"
#include "../../Effects/SoundEffectCollection.h"
#include "../../Voice/VoiceDetails.h"
#include "../../Voice/MasteringVoice.h"
#include "../../Voice/OutputVoices.h"
#include "../../Voice/SourceVoice.h"
#include "../../Voice/SubmixVoice.h"
#include "../../Voice/SubmixVoiceInitParam.h"
#include "../../SoundSystem.h"

Sound3DEmitter::Sound3DEmitter(SourceVoice & sourceVoice, const MasteringVoice & masteringVoice, const WaveFormat & format):
	mEmitter(),
	mReverb(nullptr),
	mReverbID(0),
	mPreviousPos(Vector3::zero),
	mIsCalculateLPFDirect(true),
	mDspSetter(nullptr)
{
	//���o�[�u�T�u�~�b�N�X�{�C�X�쐬
	createReverbSubmixVoice(masteringVoice);
	//���o�[�u�T�u�~�b�N�X�{�C�X���o�͐�Ɏw��
	sourceVoice.getOutputVoices().addOutputVoice(*mReverb, true);
	//DSP�쐬
	mDspSetter = std::make_unique<DspSetter>(sourceVoice, masteringVoice, *mReverb, format);
	//�G�~�b�^�[������
	initializeEmitter(format);
}

Sound3DEmitter::~Sound3DEmitter() = default;

void Sound3DEmitter::update()
{
	//mEmitter.velocity = (mEmitter.position - mPreviousPos); / Time::deltaTime;

	mEmitter.velocity = (mEmitter.position - mPreviousPos);
	mPreviousPos = mEmitter.position;
}

void Sound3DEmitter::setPosition(const Vector3 & pos)
{
	mEmitter.position = pos;
}

void Sound3DEmitter::setDopplerScale(float scale)
{
	scale = Math::clamp<float>(scale, 0.f, Math::infinity);
	mEmitter.dopplerScaler = scale;
}

bool Sound3DEmitter::isCalculateDoppler() const
{
	return !Math::nearZero(mEmitter.dopplerScaler);
}

void Sound3DEmitter::setCalculateLPFDirect(bool value)
{
	mIsCalculateLPFDirect = value;
}

bool Sound3DEmitter::isCalculateLPFDirect() const
{
	return mIsCalculateLPFDirect;
}

void Sound3DEmitter::setCalculateReverb(bool value)
{
	auto& effect = mReverb->getSoundEffect();
    (value) ? effect.enable(mReverbID) : effect.disable(mReverbID);
}

bool Sound3DEmitter::isCalculateReverb() const
{
	 return mReverb->getSoundEffect().getEnabled(mReverbID);
}

const Sound3DEmitterParam & Sound3DEmitter::getEmitter() const
{
	return mEmitter;
}

DspSetter & Sound3DEmitter::getDspSetter() const
{
	return *mDspSetter;
}

void Sound3DEmitter::createReverbSubmixVoice(const MasteringVoice & masteringVoice)
{
	SubmixVoiceInitParam param;
	//���o�[�u�̃`�����l������1
	param.channels = 1;
	//�}�X�^�[�{�C�X�ɃT���v���������킹��
	param.inputSampleRate = masteringVoice.getVoiceDetails().sampleRate;
	//���o�[�u�쐬
	mReverb = SoundSystem::instance().createSubmixVoice(param);
	mReverbID = mReverb->getSoundEffect().getEffectCollection().simpleReverb();
	mReverbID = mReverb->getSoundEffect().getEffectCollection().reverb();
}

void Sound3DEmitter::initializeEmitter(const WaveFormat & format)
{
	const auto inCh = format.channels;

	mEmitter.cone = &EMITTER_CONE;
	mEmitter.channelCount = inCh;
	mEmitter.channelRadius = 1.f;
	mAzimuths.resize(inCh);
	mEmitter.channelAzimuths = mAzimuths.data();
	mEmitter.innerRadius = 2.f;
	mEmitter.innerRadiusAngle = Math::PI / 4.f;
	mEmitter.volumeCurve = &DEFAULT_LINEAR_CURVE;
	mEmitter.lfeCurve = &EMITTER_LFE_CURVE;
	mEmitter.lpfDirectCurve = nullptr; //�f�t�H���g�J�[�u���g�p
	mEmitter.lpfReverbCurve = nullptr; //�f�t�H���g�J�[�u���g�p
	mEmitter.reverbCurve = &EMITTER_REVERB_CURVE;
	mEmitter.curveDistanceScaler = 14.f;
	mEmitter.dopplerScaler = 0.f;
}


Sound3DCone Sound3DEmitter::EMITTER_CONE = { 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f };
DistanceCurvePoint Sound3DEmitter::DEFAULT_LINEAR_CURVE_POINTS[2] = { 0.f, 1.f, 1.f, 0.f };
DistanceCurve Sound3DEmitter::DEFAULT_LINEAR_CURVE = { &DEFAULT_LINEAR_CURVE_POINTS[0], 2 };
DistanceCurvePoint Sound3DEmitter::EMITTER_LFE_CURVE_POINTS[3] = { 0.f, 1.f, 0.25f, 0.f, 1.f, 0.f };
DistanceCurve Sound3DEmitter::EMITTER_LFE_CURVE = { &EMITTER_LFE_CURVE_POINTS[0], 3 };
DistanceCurvePoint Sound3DEmitter::EMITTER_REVERB_CURVE_POINTS[3] = { 0.f, 0.5f, 0.75f, 1.f, 1.f, 0.f };
DistanceCurve Sound3DEmitter::EMITTER_REVERB_CURVE = { &EMITTER_LFE_CURVE_POINTS[0], 3 };