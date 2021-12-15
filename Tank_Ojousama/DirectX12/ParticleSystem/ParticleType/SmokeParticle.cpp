#include "SmokeParticle.h"
#include "../Emitter/Emitter.h"
#include "../Emitter/EmitterData.h"
#include "../ParticleSystem.h"

SmokeParticle::SmokeParticle(const Vector3 & pos, bool looping)
{
	ParticleSystems ps;
	ps.Duration = 0.2f;
	ps.Looping = looping;
	ps.StartDelay = 0.f;
	ps.StartLifeTime = 300.f;
	ps.StartSpeed = 1.f;
	ps.StartRotation3D = Vector3::zero;
	ps.StartColor = Vector4(0.02f, 0.02f, 0.02f, 0.1f);
	ps.GravityModifier = 0.001f;
	ps.StartVector = Vector3(0.0f, 0.0f, 0.f);
	ps.StartSize3D = Vector3(6.5f, 6.5f, 1.f);
	Burst burst;
	burst.Count = 50;
	mEmitter = new Emitter(pos, ps, burst);//Resouse/bullet.png
	AmountOfChangeInParticles aocip;
	aocip.rotate = Vector3::zero;
	mEmitter->setAmountOfChangeInParticles(aocip);
	ParticleRandomState prs;
	prs.randomVec = Vector3(0.25f, 0.15f, 0.25f);
	prs.randomLife = 50.0f;
	prs.randomColor = Vector4(0.f, 0.f, 0.f, 0.1f);
	mEmitter->setParticleRandomState(prs);

	ParticleSystem::instance().add(mEmitter);
}

SmokeParticle::~SmokeParticle()
{
	mEmitter->death();
}

void SmokeParticle::setPos(const Vector3 & pos)
{
	mEmitter->setPos(pos);
}

void SmokeParticle::Play()
{
	mEmitter->setIsGo();
}

void SmokeParticle::Stop()
{
	mEmitter->setStop();
}
