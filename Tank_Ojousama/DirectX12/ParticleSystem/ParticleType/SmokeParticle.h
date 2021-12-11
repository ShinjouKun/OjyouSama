#pragma once
#include "../../Math/Math.h"

class Emitter;

//����������
class SmokeParticle
{
public:
	SmokeParticle(const Vector3& pos = Vector3::zero, bool looping = false);
	~SmokeParticle();

	void setPos(const Vector3& pos);

	void Play();

	void Stop();

private:
	Emitter* mEmitter;
};