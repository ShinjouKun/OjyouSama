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
	void Update()override;//�X�V
	//inline void SetColPos(Vector3 pos) { this->offset = pos; }//Pos�Z�b�g
	inline void SetRadiuse(float radius) { this->radius = radius; }//���a�Z�b�g
private:
	Vector3 offset;
	float rad;
	// BaseCollider ����Čp������܂���
	virtual void Regist() override;

};
