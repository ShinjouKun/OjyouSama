#include "TankTrajectory.h"
#include "../Emitter/Emitter.h"
#include "../Emitter/EmitterData.h"
#include "../ParticleSystem.h"

TankTrajectory::TankTrajectory(const Vector3 & pos, bool looping)
{
	ParticleSystems ps;
	ps.Duration = 0.1f;
	ps.Looping = looping;
	ps.StartDelay = 0.f;
	ps.StartLifeTime = 60.f;
	ps.StartSpeed = 1.f;
	ps.StartRotation3D = Vector3::zero;
	ps.StartColor = Vector4(0.3f, 0.3f, 0.3f, 1.f);
	ps.GravityModifier = 0.f;
	ps.StartVector = Vector3(0.0f, 0.0f, 0.f);
	ps.StartSize3D = Vector3(0.3f, 0.3f, 1.f);
	Burst burst;
	burst.Count = 20;
	mEmitter = new Emitter(pos, ps, burst);//Resouse/bullet.png
	AmountOfChangeInParticles aocip;
	aocip.rotate = Vector3::zero;
	mEmitter->setAmountOfChangeInParticles(aocip);
	ParticleRandomState prs;
	prs.randomVec = Vector3(0.05f, 0.f, 0.1f);
	prs.randomLife = 5.0f;
	prs.randomColor = Vector4(0.7f, 0.7f, 0.7f, 0.f);
	mEmitter->setParticleRandomState(prs);

	ParticleSystem::instance().add(mEmitter);
}

TankTrajectory::~TankTrajectory()
{
}

void TankTrajectory::setPos(const Vector3 & pos)
{
	mEmitter->setPos(pos);
}

void TankTrajectory::Play()
{
	mEmitter->setIsGo();
}

void TankTrajectory::Stop()
{
	mEmitter->setStop();
}
