#include "SpherCollider.h"

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Update()
{
	//À•WŒn‚É‚¸‚ê‚ªo‚é‰Â”\«‚ª‚ ‚é‚Ì‚Å’ˆÓ
	Sphere::center = object->GetPosition();
	Sphere::radius = radius;
}
