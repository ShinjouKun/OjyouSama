#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "../Math/Vector3.h"
#include<cmath>
#include<memory>
using namespace std;

class  AABBCollider : public BaseCollider, public AABB
{
public:
	~AABBCollider();
	AABBCollider(const Vector3& min = Vector3(0, 0, 0), const Vector3& max = Vector3(1.0f, 1.0f, 1.0f));
	// BaseCollider を介して継承されました
	 void Update() override;
	inline void SetMax(const Vector3& max) {this->boxMax = max;}//最大値のセット
private:
	Vector3 boxMin;
	Vector3 boxMax;
	// BaseCollider を介して継承されました
	virtual void Regist() override;
};