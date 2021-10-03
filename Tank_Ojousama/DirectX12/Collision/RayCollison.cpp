#include "RayCollison.h"
#include "../Collision/CollisonManager.h"
#include"../Collision/CollisonTree.h"
#include"../Collision/OctTreeManager.h"
RayCollison::RayCollison(const Vector3 & point, const Vector3 & dir)
	:point(point),dir(dir)
{
	collisonTypes = RAY_COLLISON;//��������Z�b�g
	colT = new CollisonTree();//�R���W�����c���[�p����
	colT->colObject = this;//�I�u�W�F�N�g�̓o�^
}

RayCollison::~RayCollison()
{
	delete colT;
}

void RayCollison::Update()
{
	Ray::point = object->GetPosition();
	Ray::dir = dir;

	colT->Remove();
	CollisonManager::GetInstance()->GetInstanceOct()->
		Regist(&Vector3(point.x - 1.0f, point.y - 1.0f, point.z - 1.0f),
			&Vector3(point.x + 1.0f, point.y + 1.0f, point.z + 1.0f),
			colT);
}

void RayCollison::Regist()
{
	CollisonManager::GetInstance()->GetInstanceOct()->
		Regist(&Vector3(point.x - 1.0f, point.y - 1.0f, point.z - 1.0f),
			&Vector3(point.x + 1.0f, point.y + 1.0f, point.z + 1.0f),
			colT);
}
