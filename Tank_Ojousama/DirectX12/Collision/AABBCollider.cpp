#include "AABBCollider.h"
#include "../Collision/CollisonManager.h"
#include"../Collision/CollisonTree.h"
#include"../Collision/OctTreeManager.h"
#pragma once



//AABBCollider::AABBCollider(const Vector3 & min, const Vector3 & max)
//	:boxMin(min),boxMax(max)
//{
//	collisonTypes = AABB_COLLISON;//���̓����蔻����Z�b�g
//	colT = sp<CollisonTree>(new CollisonTree());//�o�^
//	colT->colObject = this;//�I�u�W�F�N�g�̓o�^
//}
//
//AABBCollider::~AABBCollider()
//{
//}
//void AABBCollider::Update()
//{
//	/*float pos[3] = { max.x,max.y,max.z };
//	ImGui::SliderFloat3("hako", pos, 0, 10000.0f);*/
//	AABB::min = object->GetPosition();
//	AABB::max = min + boxMax;
//	//��������폜
//	colT->Remove();
//	//�ēo�^
//	CollisonManager::GetInstance()->GetInstanceOct()->
//		Regist(&Vector3(min.x, min.y, min.z),
//			&Vector3(max.x, max.y , max.z), colT);
//}
//
//void AABBCollider::Regist()
//{
//	//�ēo�^
//	CollisonManager::GetInstance()->GetInstanceOct()->
//		Regist(&Vector3(min.x, min.y, min.z),
//			&Vector3(max.x, max.y, max.z), colT);
//}
