#include "SpherCollider.h"

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Update()
{
	//座標系にずれが出る可能性があるので注意
	Sphere::center = object->GetPosition();
	Sphere::radius = radius;
}
