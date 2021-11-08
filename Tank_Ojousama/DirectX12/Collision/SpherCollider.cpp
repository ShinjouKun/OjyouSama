#include "SpherCollider.h"
#include "../Collision/CollisonManager.h"
#include"../Collision/CollisonTree.h"
#include"../Collision/OctTreeManager.h"
SphereCollider::SphereCollider(const Vector3& offset, float radius)
	: offset(offset), rad(radius)
{
	collisonTypes = SPHERE_COLLISON;//球判定をセット
	colT = new CollisonTree();//コリジョンツリー用生成
	colT->colObject =this;//オブジェクトの登録
}

SphereCollider::~SphereCollider()
{
	delete colT;
}

void SphereCollider::Update()
{
	//座標系にずれが出る可能性があるので注意
	Sphere::center = offset + object->GetPosition();
	//Sphere::center = object->GetPosition();
	//Sphere::center = offset;
	Sphere::radius = rad;
	//いったん削除
	colT->Remove();
	////ここでツリーに再登録
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
