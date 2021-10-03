#include "AABBCollider.h"
#include "../Collision/CollisonManager.h"
#include"../Collision/CollisonTree.h"
#include"../Collision/OctTreeManager.h"
#pragma once

AABBCollider::AABBCollider(const Vector3 & min, const Vector3 & max)
	:boxMin(min),boxMax(max)
{
	collisonTypes = AABB_COLLISON;//” ‚Ì“–‚½‚è”»’è‚ðƒZƒbƒg
	colT = new CollisonTree();//“o˜^
	colT->colObject = this;//ƒIƒuƒWƒFƒNƒg‚Ì“o˜^
}

AABBCollider::~AABBCollider()
{
}
void AABBCollider::Update()
{
	/*float pos[3] = { max.x,max.y,max.z };
	ImGui::SliderFloat3("hako", pos, 0, 10000.0f);*/
	AABB::min = object->GetPosition();
	AABB::max = min + boxMax;
	//‚¢‚Á‚½‚ñíœ
	colT->Remove();
	//Ä“o˜^
	CollisonManager::GetInstance()->GetInstanceOct()->
		Regist(&Vector3(min.x-1.0f, min.y - 1.0f, min.z - 1.0f),
			&Vector3(max.x + 1.0f, max.y + 1.0f, max.z + 1.0f), colT);
}

void AABBCollider::Regist()
{
	//Ä“o˜^
	CollisonManager::GetInstance()->GetInstanceOct()->
		Regist(&Vector3(min.x - 1.0f, min.y - 1.0f, min.z - 1.0f),
			&Vector3(max.x + 1.0f, max.y + 1.0f, max.z + 1.0f), colT);
}
