#include "MachineGunAttackParticle.h"
#include "../Emitter/Emitter.h"
#include "../Emitter/EmitterData.h"
#include "../ParticleSystem.h"

MachineGunAttackParticle::MachineGunAttackParticle(const Vector3 & pos, bool looping)
{
	ParticleSystems ps;
	ps.Duration = 0.1f;
	ps.Looping = looping;
	ps.StartDelay = 0.f;
	ps.StartLifeTime = 10.f;
	ps.StartSpeed = 5.f;
	ps.StartRotation3D = Vector3::zero;
	ps.StartColor = Vector4(1.f, 0.8f, 0.2f, 1.f);
	ps.GravityModifier = 0.f;
	ps.StartVector = Vector3(0.0f, 0.0f, 0.f);
	ps.StartSize3D = Vector3(5.f, 5.f, 1.f);
	Burst burst;
	burst.Count = 30;
	mEmitter = new Emitter(pos, ps, burst);
	AmountOfChangeInParticles aocip;
	aocip.rotate = Vector3::zero;
	mEmitter->setAmountOfChangeInParticles(aocip);
	ParticleRandomState prs;
	prs.randomVec = Vector3(0.5f, 0.5f, 0.0f);
	mEmitter->setParticleRandomState(prs);

	mEmitter->setStop();

	ParticleSystem::instance().add(mEmitter);


}

MachineGunAttackParticle::~MachineGunAttackParticle()
{
}

void MachineGunAttackParticle::setPos(const Vector3 & pos)
{
	mEmitter->setPos(pos);
}

void MachineGunAttackParticle::Play()
{
	mEmitter->setIsGo();
}
