#pragma once
#include "../../Math/Math.h"

class Emitter;

//è¨Ç≥Ç¢îöî≠
class Hit
{
public:
	Hit(const Vector3& pos = Vector3::zero, bool looping = false);
	~Hit();

	void setPos(const Vector3& pos);

	void Play();

	void Stop();

private:
	Emitter* mEmitter;
};