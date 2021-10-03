#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"
#include "../Math/Vector3.h"
#include<cmath>
#include<memory>
using namespace std;

class RayCollison:public BaseCollider,public Ray
{
public:
	RayCollison(const Vector3& point = Vector3(0,0,0),const Vector3& dir = Vector3(0, 0, 0));
	~RayCollison();
	inline void SetDir(const Vector3& d) { this->dir = d; }
private:
	// BaseCollider ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Update() override;
	virtual void Regist() override;
private:
	Vector3 point;
	Vector3 dir;
};
