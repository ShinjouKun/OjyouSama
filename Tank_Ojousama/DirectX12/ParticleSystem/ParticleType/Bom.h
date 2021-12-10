#pragma once
#include "../../Math/Math.h"

class Emitter;

//����������
class Bom
{
public:
	Bom(const Vector3& pos = Vector3::zero, bool looping = false);
	~Bom();

	void setPos(const Vector3& pos);

private:
	Emitter* mEmitter;
};