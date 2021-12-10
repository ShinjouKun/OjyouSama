#pragma once
#include "../../Math/Math.h"

class Emitter;

class NormalAttackParticle
{
public:
	NormalAttackParticle(const Vector3& pos = Vector3::zero, bool looping = false);
	~NormalAttackParticle();

	void setPos(const Vector3& pos);

	void Attack();

private:
	Emitter* mEmitter;
};