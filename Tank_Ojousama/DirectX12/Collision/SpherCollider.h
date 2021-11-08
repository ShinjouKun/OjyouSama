#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "../Math/Vector3.h"
#include<cmath>
#include<memory>
using namespace std;

class SphereCollider :public BaseCollider, public Sphere
{
public:
	//SphereCollider() = default;
	~SphereCollider();

	SphereCollider( const Vector3& offset = Vector3(0, 0, 0), float radius = 1.0f);
	void Update()override;//更新
	//inline void SetColPos(Vector3 pos) { this->offset = pos; }//Posセット
	inline void SetRadiuse(float radius) { this->radius = radius; }//半径セット
private:
	Vector3 offset;
	float rad;
	// BaseCollider を介して継承されました
	virtual void Regist() override;

};
