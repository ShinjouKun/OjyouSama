#include "SpherCollider.h"

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Update()
{
	//���W�n�ɂ��ꂪ�o��\��������̂Œ���
	Sphere::center = object->GetPosition();
	Sphere::radius = radius;
}
