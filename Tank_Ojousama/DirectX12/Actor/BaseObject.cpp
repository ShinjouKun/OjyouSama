#include "BaseObject.h"
//#include"Matrix4.h"
//#include"BaseCollider.h"
//#include"CollisonManager.h"
#include "../Math/Matrix4.h"
#include "../Collision/BaseCollider.h"
#include "../Collision/CollisonManager.h"
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
		CollisonManager::GetInstance()->CheckAllCollisons();
	}
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

void BaseObject::SetCollidder(BaseCollider * collider)
{
	collider->SetObject(this);
	this->collider = collider;
	//コリジョンマネージャーに登録
	CollisonManager::GetInstance()->AddCollider(collider);
	collider->Update();//更新
}

size_t BaseObject::GetID() const
{
	return AiID;
}

void BaseObject::SetID(size_t id)
{

	AiID = id;
}
