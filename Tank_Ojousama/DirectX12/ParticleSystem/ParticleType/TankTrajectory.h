#pragma once

#include "../../Math/Math.h"

class Emitter;

class TankTrajectory
{
public:
	TankTrajectory(const Vector3& pos = Vector3::zero, bool looping = false);
	~TankTrajectory();

	void setPos(const Vector3& pos);

	void Play();

private:
	Emitter* mEmitter;
};