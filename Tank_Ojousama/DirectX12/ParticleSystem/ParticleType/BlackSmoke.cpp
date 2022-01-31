#include "BlackSmoke.h"
#include "../Emitter/Emitter.h"
#include "../Emitter/EmitterData.h"
#include "../ParticleSystem.h"

BlackSmoke::BlackSmoke(const Vector3 & pos, bool looping)
{
	ParticleSystems ps;
	ps.Duration = 0.2f;
	ps.Looping = looping;
	ps.StartDelay = 0.f;
	ps.StartLifeTime = 80.f;
	ps.StartSpeed = 1.f;
	ps.StartRotation3D = Vector3::zero;
	ps.StartColor = Vector4(0.31f, 0.01f, 0.01f, 0.01f);
	ps.GravityModifier = 0.001f;
	ps.StartVector = Vector3(0.0f, 0.0f, 0.0f);
	ps.StartSize3D = Vector3(5.f, 5.f, 1.f);
	Burst burst;
	burst.Count = 20;
	mEmitter = new Emitter(pos, ps, burst);//Resouse/bullet.png
	AmountOfChangeInParticles aocip;
	aocip.rotate = Vector3::zero;
	mEmitter->setAmountOfChangeInParticles(aocip);
	ParticleRandomState prs;
	prs.randomVec = Vector3(0.01f, 0.01f, 0.01f);
	prs.randomLife = 50.0f;
	prs.randomSize3D = Vector3(1.f, 1.f, 0.f);
	prs.randomColor = Vector4(0.f, 0.f, 0.f, 0.1f);
	mEmitter->setParticleRandomState(prs);

	ParticleSystem::instance().add(mEmitter);
}

BlackSmoke::~BlackSmoke()
{
	if (mEmitter != nullptr)
	{
		mEmitter->death();
	}
}

void BlackSmoke::setPos(const Vector3 & pos)
{
	if (mEmitter)
	{
		mEmitter->setPos(pos);
	}
}

void BlackSmoke::Play()
{
	if (mEmitter)
	{
		mEmitter->setIsGo();
	}
}

void BlackSmoke::Stop()
{
	if (mEmitter)
	{
		mEmitter->setStop();
	}
}
