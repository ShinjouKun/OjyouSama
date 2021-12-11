#include "NormalAttackParticle.h"
#include "../Emitter/Emitter.h"
#include "../Emitter/EmitterData.h"
#include "../ParticleSystem.h"

NormalAttackParticle::NormalAttackParticle(const Vector3 & pos, bool looping)
{
	ParticleSystems ps;
	ps.Duration = 0.1f;
	ps.Looping = looping;
	ps.StartDelay = 0.f;
	ps.StartLifeTime = 3.f;
	ps.StartSpeed = 1.f;
	ps.StartRotation3D = Vector3::zero;
	ps.StartColor = Vector4(1.f, 0.8f, 0.2f, 1.f);
	ps.GravityModifier = 0.f;
	ps.StartVector = Vector3(0.0f, 0.0f, 0.f);
	ps.StartSize3D = Vector3(0.5f, 0.5f, 1.f);
	Burst burst;
	burst.Count = 50;
	mEmitter = new Emitter(pos, ps, burst);
	AmountOfChangeInParticles aocip;
	aocip.rotate = Vector3::zero;
	mEmitter->setAmountOfChangeInParticles(aocip);
	ParticleRandomState prs;
	prs.randomVec = Vector3(0.5f, 0.5f, 0.0f);
	prs.randomLife = 5.f;
	
	mEmitter->setParticleRandomState(prs);

	

	ParticleSystem::instance().add(mEmitter);
}

NormalAttackParticle::~NormalAttackParticle()
{
}

void NormalAttackParticle::setPos(const Vector3 & pos)
{
	mEmitter->setPos(pos);
}

void NormalAttackParticle::Play()
{
	mEmitter->setIsGo();
}

void NormalAttackParticle::Stop()
{
	mEmitter->setStop();
}
