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
		collisonTypes = SPHERE_COLLISON;//��������Z�b�g
	}
	void Update()override;//�X�V
	inline void SetRadiuse(float radius) { this->radius = radius; }//���a�Z�b�g
private:
	Vector3 offset;
	float radius;
};
