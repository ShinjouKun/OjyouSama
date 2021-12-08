#include "BaseObject.h"


#include "../Math/Matrix4.h"
#include "../Collision/BaseCollider.h"
#include "../Collision/CollisonManager.h"
#include "../Collision/SpherCollider.h"
#include "../Collision/AABBCollider.h"
//imgui
#include"../imgui/imgui.h"
#include"../imgui/imgui_impl_win32.h"
#include"../imgui/imgui_impl_dx12.h"
BaseObject::~BaseObject()
{
	if (collider)
	{
		CollisonManager::GetInstance()->RemoveCollider(collider);
		delete collider;//後処理
	}
}

void BaseObject::CollisonUpdate()
{
	if (collider)
	{
		collider->Update();
	}
}

Vector3 BaseObject::RotateX(float angle)
{
	Vector3 v;

	angle = angle + 45.0f;
	angle = angle * PI / 180.0f;

	float sin = sinf(angle);
	float cos = cosf(angle);

	float y = cos + sin;
	float z = -(sin)+cos;

	v = Vector3(0.0f, y, z);
	return v;
}

Vector3 BaseObject::RotateY(float angle)
{
	Vector3 v;
	
	angle = angle + 45.0f;
	angle = angle * PI / 180.0f;

	float sin = sinf(angle);
	float cos = cosf(angle);

	float x = cos + sin;
	float z = -(sin)+cos;

	v = Vector3(x,0.0f,z);
	return v;
}


void BaseObject::SetHP(const int hp)
{
	HP = hp;
}

void BaseObject::SetCollidder(BaseCollider * collider)
{
	collider->SetObject(this);
	this->collider = collider;
	//コリジョンマネージャーに登録
	CollisonManager::GetInstance()->AddCollider(collider);
	collider->Update();//更新

}

void BaseObject::SetCollidder(const Vector3 & center, float rad)
{
	
	this->collider = new SphereCollider(center, rad);
	this->collider->SetObject(this);
	//コリジョンマネージャーに登録
	CollisonManager::GetInstance()->AddCollider(collider);
	collider->Update();//更新
	//八分木登録
	this->collider->Regist();
}

void BaseObject::SetCollidder(const Vector3 & min, const Vector3 & max)
{
	this->collider = new AABBCollider(min, max);
	this->collider->SetObject(this);
	//コリジョンマネージャーに登録
	CollisonManager::GetInstance()->AddCollider(collider);
	collider->Update();
	//八分木登録
	this->collider->Regist();
}

size_t BaseObject::GetID() const
{
	return AiID;
}

void BaseObject::SetID(size_t id)
{

	AiID = id;
}

void BaseObject::SetShotCount(int money)
{
	shotMoney = money;
}

void BaseObject::SetTresureGet(bool val)
{
	getTreasure = val;
}

void BaseObject::SetActive(bool val)
{
	IsActive = val;
}

void BaseObject::SetNumber(int num)
{
	number = num;
}

int BaseObject::GetNumber()
{
	return number;
}