#include "CameraEye.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"
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
	SetCollidder(Vector3(position.x, position.y, position.z), 10.0f);//Ž‹”F”ÍˆÍ
}

void CameraEye::Update()
{
	ImGuiDebug();
	this->position = objM->GetPlayer().GetPosition();
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
