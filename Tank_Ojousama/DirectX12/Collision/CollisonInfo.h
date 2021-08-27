#pragma once
#include<cmath>
#include "../Math/Vector3.h"

class BaseObject;
class BaseCollider;

struct CollisonInfo
{
public:
	CollisonInfo(BaseObject* obj, BaseCollider* coll, const Vector3& inter)
	{
		this->object = obj;
		this->collider = coll;
		this->hitInter = inter;
	}
	BaseObject* object = nullptr;
	BaseCollider* collider = nullptr;
	//�Փ˓_
	Vector3 hitInter;
};