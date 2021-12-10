#include "Emitter.h"
#include "../Compute/Compute.h"
#include "../../Utility/Timer/Timer.h"

#include "../../Utility/Random.h"

Emitter::Emitter(const Vector3& pos, const ParticleSystems& ps, const Burst& burst) :
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

	delete(mCompute);
	mCompute = nullptr;

	mDataList.clear();
	mPendingDataList.clear();
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

	//削除処理
	int deathCount = 0;
	int listSize = static_cast<int>(mDataList.size());
	for (auto&& itr : mDataList)
	{
		++deathCount;
		if (!itr.active)
		{
			if (deathCount == listSize)
			{
				mDataList.pop_back();
				break;
			}
			std::swap(itr, mDataList.back());
			mDataList.pop_back();
			--listSize;
		}
	}

	//追加処理
	int size = static_cast<int>(mDataList.size());
	Random::initialize();
	for (int i = 0; i < 50; ++i)//mBurst.Count
	{
		if (size > MAX_PARTICLE_SIZE - 1) break;
		ParticleData data;
		data.lifeTime = mParticleSystems.StartLifeTime + Random::randomRange(-mPRS.randomLife, mPRS.randomLife);
		data.pos.x = mPos->x;
		data.pos.y = mPos->y;
		data.pos.z = mPos->z;
		data.vec = mParticleSystems.StartVector + Random::randomRange(-mPRS.randomVec, mPRS.randomVec);
		data.temp1 = 0;
		data.col = mParticleSystems.StartColor;
		data.size = mParticleSystems.StartSize3D;
		data.speed = mParticleSystems.StartSpeed;
		data.rotate = Vector3::zero;//mParticleSystems.StartRotation3D;
		data.axis = Vector3::zero;
		data.gravity = mParticleSystems.GravityModifier;
		data.active = true;

		mDataList.emplace_back(data);
		++size;
	}

	std::copy(mPendingDataList.begin(), mPendingDataList.end(), std::back_inserter(mDataList));
	mPendingDataList.clear();

	//コンピュートクラスを更新する
	mData = mCompute->particleUpdate(mDataList.data(), mDataList.size());

	mDataList.assign((ParticleData*)mData, (ParticleData*)mData + mDataList.size());
}

void Emitter::draw()
{
	//コンピュートクラスから描画を呼び出す
	mCompute->particleDraw(static_cast<int>(mDataList.size()));
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
