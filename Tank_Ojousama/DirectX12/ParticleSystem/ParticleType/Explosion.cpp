#include "Explosion.h"
#include "../Emitter/Emitter.h"
#include "../Emitter/EmitterData.h"
#include "../ParticleSystem.h"

Explosion::Explosion(const Vector3 & pos, bool looping)
{
	ParticleSystems ps;
	ps.Duration = 0.6f;
	ps.Looping = looping;
	ps.StartDelay = 0.f;
	ps.StartLifeTime = 30.f;
	ps.StartSpeed = 5.f;
	ps.StartRotation3D = Vector3::zero;
	ps.StartColor = Vector4(0.85f, 0.2f, 0.f, 1.f);
	ps.GravityModifier = 0.f;
	ps.StartVector = Vector3(0.0f, 0.0f, 0.f);
	ps.StartSize3D = Vector3(18.f, 18.f, 1.f);
	Burst burst;
	burst.Count = 30;
	mEmitter = new Emitter(pos, ps, burst, "Resouse/bullet.png");
	AmountOfChangeInParticles aocip;
	aocip.rotate = Vector3::zero;
	mEmitter->setAmountOfChangeInParticles(aocip);
	ParticleRandomState prs;
	prs.randomVec = Vector3(0.5f, 0.5f, 0.0f);
	mEmitter->setParticleRandomState(prs);

	ParticleSystem::instance().add(mEmitter);
}

Explosion::~Explosion()
{
}

void Explosion::setPos(const Vector3 & pos)
{
	mEmitter->setPos(pos);
}

void Explosion::Play()
{
	
	mEmitter->setIsGo();
}

void Explosion::Stop()
{
	mEmitter->setStop();
}
