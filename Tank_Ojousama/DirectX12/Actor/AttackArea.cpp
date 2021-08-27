#include "AttackArea.h"
#include "../Collision/SpherCollider.h"

AttackArea::AttackArea(Vector3 pos, Vector3 ang, ObjectManager * objM, shared_ptr<ModelRenderer> modelR, int num)
	:modelRender(modelR)
{
	position = pos;
	angle = ang;
	objManager = objM;
	number = num;
}

AttackArea::~AttackArea()
{
}

void AttackArea::Init()
{
	death = false;
	objType = ObjectType::ENEMYBULLET;

	scale = Vector3(1, 1, 1);
	radius = 1.0f;

	isActive = false;

	name = "AttackArea";
	key = to_string(number);
	keyname = name + key;

	modelRender->AddModel(keyname, "Resouse/bill.obj", "Resouse/bill.png");
	modelRender->SetAncPoint(keyname, Vector3(-2.0f, -2.0f, -2.0f));

	SetCollidder(new SphereCollider(position, radius));
}

void AttackArea::Update()
{
	if (!isActive) return;

	ImGuiDebug();
}

void AttackArea::Rend()
{
	if (!isActive) return;

	DirectXManager::GetInstance()->SetData3D();
	modelRender->Draw(keyname, position,Vector3(angle.x, -angle.y, angle.z) , Vector3(1, 1, 1));
}

void AttackArea::OnCollison(const CollisonInfo & info)
{
	if (!isActive) return;
}

void AttackArea::ImGuiDebug()
{
	float pos[3] = { position.x,position.y,position.z };
	ImGui::SliderFloat3("---Position---", pos, -500.0f, 500.0f);
	float ang[3] = { angle.x,angle.y,angle.z };
	ImGui::SliderFloat3("---Angle---", ang, -500.0f, 500.0f);
}

void AttackArea::SetPosition(Vector3 pos)
{
	position = pos;
}

void AttackArea::SetActive(bool value, Vector3 pos, Vector3 ang)
{
	isActive = value;
	position = pos;
	angle = ang;
}

bool AttackArea::GetActive()
{
	return isActive;
}

void AttackArea::SetDeath(bool value)
{
	death = value;
}
