#pragma once
#include "../../Math/Math.h"

class Emitter;

//火炎(黒くできなかった)
class BlackSmoke
{
public:
	BlackSmoke(const Vector3& pos = Vector3::zero, bool looping = false);
	~BlackSmoke();

	void setPos(const Vector3& pos);

	void Play();

	void Stop();

private:
	Emitter* mEmitter;
};