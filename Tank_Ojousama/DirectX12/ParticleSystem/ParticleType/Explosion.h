#pragma once

#include "../../Math/Math.h"

class Emitter;

//�傫������
class Explosion
{
public:
	Explosion(const Vector3& pos = Vector3::zero, bool looping = false);
	~Explosion();

	void setPos(const Vector3& pos);

	void Play();

	void Stop();

private:
	Emitter* mEmitter;
};