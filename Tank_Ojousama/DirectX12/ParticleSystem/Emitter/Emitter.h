#pragma once

#include "EmitterData.h"
#include "../ParticleData/ParticleData.h"

#include <list>
#include <vector>
#include <string>

class Timer;
class Compute;

constexpr int MAX_PARTICLE_SIZE = 10000;//パーティクルの最大個数

//各エミッターでgpuパーティクルをやる
class Emitter
{
public:
	Emitter(const Vector3& pos, const ParticleSystems& ps, const Burst& burst, std::string texName = "Resouse/particle.jpg");
	~Emitter();

	void update();

	void draw();

	bool isActive() const;

	void setPos(const Vector3& pos);
	void setVec(const Vector3& vec);
	void setRandomVec(const Vector3& vec);
	void setParticleSystems(const ParticleSystems& ps);
	void setAmountOfChangeInParticles(const AmountOfChangeInParticles& aocip);
	void setParticleRandomState(const ParticleRandomState& prs);
	void setParticleRotateState(const ParticleRotateState& prs);

private:
	void add();

private:
	//コピー禁止
	Emitter(const Emitter&) = delete;
	Emitter& operator=(const Emitter&) = delete;
	Emitter(Emitter&&) = delete;
	Emitter& operator=(Emitter&&) = delete;

private:
	bool mActive;
	bool mEnd;
	Vector3* mPos;//エミッターの位置
	Vector3* mVec;//パーティク用ベクトル
	Vector3* mRandomVec;//ランダム要素用
	int mDelayTimer;
	ParticleSystems mParticleSystems;//パーティクルシステム

	float mRateOverTime = 0.f;//
	float mRateOverDistance = 0.f;//
	Burst mBurst;
	AmountOfChangeInParticles mAOCIP;//変化量
	ParticleRandomState mPRS;//ランダム性
	ParticleRotateState mPS;//パーティクルの回転用

	Timer* mTimer;
	Compute* mCompute;
private:
	//gpgpuに必要なデータ
	std::vector<ParticleData> mDataList;
	std::list<ParticleData> mPendingDataList;
	void* mData;//computeから受取るデータ
};