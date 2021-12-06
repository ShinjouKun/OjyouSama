#include "Emitter.h"
#include "../Compute/Compute.h"
#include "../../Utility/Timer/Timer.h"

constexpr int MAX_PARTICLE_SIZE = 10000;//パーティクルの最大個数

Emitter::Emitter(const Vector3& pos, const ParticleSystems& ps, const Burst& burst):
	mActive(true),
	mPos(new Vector3(0.0f, 0.0f, 0.0f)),
	mVec(new Vector3(0.0f, 0.0f, 0.0f)),
	mRandomVec(new Vector3(0.0f, 0.0f, 0.0f)),
	mParticleSystems(ps),
	mBurst(burst),
	mDelayTimer(0),
	mAOCIP(),
	mPRS(),
	mPS(),
	mTimer(new Timer()),
	mCompute(new Compute())
{
	mPos->x = pos.x;
	mPos->y = pos.y;
	mPos->z = pos.z;
	mTimer->setTime(mParticleSystems.Duration);
	mCompute->init();
}

Emitter::~Emitter()
{
	delete(mTimer);
	mTimer = nullptr;
	delete(mPos);
	mPos = nullptr;
	delete(mVec);
	mVec = nullptr;
	delete(mRandomVec);
	mRandomVec = nullptr;
}

void Emitter::update()
{
	//死んでいるのに削除されてない場合用
	if (!mActive)return;

	mTimer->update();
	if (mTimer->isTime() && !mParticleSystems.Looping)
	{
		mActive = false;
		return;
	}
	//コンピュートクラスを更新する
	mCompute->emitterUpdate(mBurst.Count);
	mCompute->particleUpdate();
}

void Emitter::draw()
{
	//コンピュートクラスから描画を呼び出す
	mCompute->particleDraw();
}

bool Emitter::isActive() const
{
	return mActive;
}

void Emitter::setPos(const Vector3 & pos)
{
	*mPos = pos;
}

void Emitter::setVec(const Vector3 & vec)
{
	*mVec = vec;
}

void Emitter::setRandomVec(const Vector3 & vec)
{
	*mRandomVec = vec;
}

void Emitter::setParticleSystems(const ParticleSystems & ps)
{
	mParticleSystems = ps;
}

void Emitter::setAmountOfChangeInParticles(const AmountOfChangeInParticles & aocip)
{
	mAOCIP = aocip;
}

void Emitter::setParticleRandomState(const ParticleRandomState & prs)
{
	mPRS = prs;
}

void Emitter::setParticleRotateState(const ParticleRotateState & prs)
{
	mPS = prs;
}

void Emitter::add()
{
	if (static_cast<int>(mDataList.size()) >= MAX_PARTICLE_SIZE)return;

}
