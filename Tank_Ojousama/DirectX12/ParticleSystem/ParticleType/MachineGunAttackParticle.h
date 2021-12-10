#pragma once

#include "../../Math/Math.h"

class Emitter;

class MachineGunAttackParticle
{
public:
	MachineGunAttackParticle(const Vector3& pos = Vector3::zero, bool looping = false);
	~MachineGunAttackParticle();

	void setPos(const Vector3& pos);

	void Attack();

private:
	Emitter* mEmitter;
};