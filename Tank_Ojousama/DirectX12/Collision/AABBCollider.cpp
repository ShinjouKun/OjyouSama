#include "AABBCollider.h"
#include "../Collision/CollisonManager.h"
#include"../Collision/CollisonTree.h"
#include"../Collision/OctTreeManager.h"
#pragma once



//AABBCollider::AABBCollider(const Vector3 & min, const Vector3 & max)
//	:boxMin(min),boxMax(max)
//{
//	collisonTypes = AABB_COLLISON;//” ‚Ì“–‚½‚è”»’è‚ðƒZƒbƒg
//	colT = sp<CollisonTree>(new CollisonTree());//“o˜^
//	colT->colObject = this;//ƒIƒuƒWƒFƒNƒg‚Ì“o˜^
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
//	//‚¢‚Á‚½‚ñíœ
//	colT->Remove();
//	//Ä“o˜^
//	CollisonManager::GetInstance()->GetInstanceOct()->
//		Regist(&Vector3(min.x, min.y, min.z),
//			&Vector3(max.x, max.y , max.z), colT);
//}
//
//void AABBCollider::Regist()
//{
//	//Ä“o˜^
//	CollisonManager::GetInstance()->GetInstanceOct()->
//		Regist(&Vector3(min.x, min.y, min.z),
//			&Vector3(max.x, max.y, max.z), colT);
//}
