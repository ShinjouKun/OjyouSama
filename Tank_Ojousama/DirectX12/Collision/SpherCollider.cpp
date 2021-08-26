#include "SpherCollider.h"
#include "../Collision/CollisonManager.h"
#include"../Collision/CollisonTree.h"
#include"../Collision/OctTreeManager.h"
SphereCollider::SphereCollider(const Vector3& offset, float radius)
	: offset(offset), radius(radius)
{
	collisonTypes = SPHERE_COLLISON;//��������Z�b�g
	colT = new CollisonTree();//�R���W�����c���[�p����
	colT->colObject =this;//�I�u�W�F�N�g�̓o�^
}

SphereCollider::~SphereCollider()
{
	delete colT;
}

void SphereCollider::Update()
{
	//���W�n�ɂ��ꂪ�o��\��������̂Œ���
	Sphere::center = object->GetPosition();
	Sphere::radius = radius;
	//��������폜
	colT->Remove();
	////�����Ńc���[�ɍēo�^
	CollisonManager::GetInstance()->GetInstanceOct()->
		Regist(&Vector3(center.x - radius, center.y - radius, center.z-  radius),
			&Vector3(center.x + radius, center.y + radius, center.z + radius),
			colT);
}

void SphereCollider::Regist()
{
	CollisonManager::GetInstance()->GetInstanceOct()->
		Regist(&Vector3(center.x - radius, center.y - radius, center.z - radius),
			&Vector3(center.x + radius, center.y + radius, center.z + radius),
			colT);
}
