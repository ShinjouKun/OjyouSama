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
	delete colT;
}
void AABBCollider::Update()
{

	 
	AABB::min = boxMin + object->GetPosition();
	AABB::max = boxMax + object->GetPosition();
	//‚¢‚Á‚½‚ñíœ
	colT->Remove();
	//Ä“o˜^
	CollisonManager::GetInstance()->GetInstanceOct()->
		Regist(&Vector3(min.x-1.0f, min.y - 1.0f, min.z - 1.0f),
			&Vector3(max.x + 1.0f, max.y + 1.0f, max.z + 1.0f), colT);

	//float pos[3] = { min.x,min.y,min.z };
	//ImGui::SliderFloat3("hakomin", pos, -10000, 10000);
	//float posm[3] = { max.x,max.y,max.z };
	//ImGui::SliderFloat3("hakomax", posm, -10000, 10000);
}

void AABBCollider::Regist()
{
	//Ä“o˜^
	CollisonManager::GetInstance()->GetInstanceOct()->
		Regist(&Vector3(min.x - 1.0f, min.y - 1.0f, min.z - 1.0f),
			&Vector3(max.x + 1.0f, max.y + 1.0f, max.z + 1.0f), colT);
}
