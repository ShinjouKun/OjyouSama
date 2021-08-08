#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "../Math/Vector3.h"

#include<cmath>


class  AABBCollider : public BaseCollider, public AABB
{
public:
	AABBCollider() = default;
	~AABBCollider();
	AABBCollider(const Vector3& min = Vector3(0, 0, 0), const Vector3& max = Vector3(1.0f, 1.0f, 1.0f))
		:min(min), max(max)
	{
		collisonTypes = AABB_COLLISON ;//AABBをセット
	}
	// BaseCollider を介して継承されました
	virtual void Update() override;

	inline void SetMax(const Vector3& max) {this->max = max;}//最大値のセット
private:
	Vector3 min;
	Vector3 max;
};