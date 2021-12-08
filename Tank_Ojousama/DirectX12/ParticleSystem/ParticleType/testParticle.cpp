#include "testParticle.h"
#include "../Emitter/Emitter.h"
#include "../Emitter/EmitterData.h"
#include "../ParticleSystem.h"

testParticle::testParticle(const Vector3& pos, bool looping)
{
	ParticleSystems ps;
	ps.Duration = 100.f;
	ps.Looping = looping;
	ps.StartDelay = 0.f;
	ps.StartLifeTime = 300.f;
	ps.StartSpeed = 1.f;
	ps.StartRotation3D = Vector3::zero;
	ps.StartColor = Vector4(1.f, 1.f, 1.f, 1.f);
	ps.GravityModifier = 0.f;
	ps.StartVector = Vector3(0.2f, 0.5f, 0.f);
	ps.StartSize3D = Vector3(2.f, 2.f, 1.f);
	Burst burst;
	burst.Count = 50;
	Emitter* emitter = new Emitter(pos, ps, burst);
	AmountOfChangeInParticles aocip;
	aocip.rotate = Vector3::zero;
	emitter->setAmountOfChangeInParticles(aocip);
	ParticleRandomState prs;
	prs.randomVec = Vector3(0.5f, 0.1f, 0.f);
	emitter->setParticleRandomState(prs);

	ParticleSystem::instance().add(emitter);
}

testParticle::~testParticle()
{
}