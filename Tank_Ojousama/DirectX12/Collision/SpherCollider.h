#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
//#include"Vector3.h"
#include "../Math/Vector3.h"

#include<cmath>

class SphereCollider :public BaseCollider, public Sphere
{
public:
	SphereCollider() = default;
	~SphereCollider();

	SphereCollider(const Vector3& offset = Vector3(0, 0, 0), float radius = 1.0f)
		:offset(offset), radius(radius)
	{
		collisonTypes = SPHERE_COLLISON;//球判定をセット
	}
	void Update()override;//更新
	inline void SetRadiuse(float radius) { this->radius = radius; }//半径セット
private:
	Vector3 offset;
	float radius;
};
