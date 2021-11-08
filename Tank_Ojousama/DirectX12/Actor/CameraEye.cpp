#include "CameraEye.h"
#include"../Collision/SpherCollider.h"
#include"../Collision/BaseCollider.h"
#include "../Collision/AABBCollider.h"
#include "../Device/Input.h"
CameraEye::CameraEye(Vector3 pos, Vector3 ang, ObjectManager * obj)
{
	position = pos;
	angle = ang;
	objM = obj;
}

CameraEye::~CameraEye()
{
}

void CameraEye::Init()
{
	death = false;
	objType = ObjectType::CAMEAR;
	SetCollidder(Vector3(0,0,-90),10);//���F�͈�
}

void CameraEye::Update()
{
	ImGuiDebug();
	velocity = Vector3(0, 0, 0);
	position = objM->GetPlayer().GetPosition();
	camVel = RotateY(angle.y + 90.0f)*8.0f;
	position = position + camVel;
	if (Input::KeyState(DIK_RIGHT) || Input::pad_data.lZ > 0)
	{
		angle.y += 1.0f;
	}
	if (Input::KeyState(DIK_LEFT) || Input::pad_data.lZ < 0)
	{
		angle.y -= 1.0f;
	}
	
}

void CameraEye::Rend()
{
}

void CameraEye::ImGuiDebug()
{
	float pos[3] = { position.x,position.y,position.z };
	ImGui::SliderFloat3("Cam", pos, 0, 10000.0f);
}

void CameraEye::OnCollison(BaseCollider * col)
{
	
}
