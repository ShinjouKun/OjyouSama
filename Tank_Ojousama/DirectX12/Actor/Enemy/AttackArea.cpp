#include "AttackArea.h"
//#include "../Collision/SpherCollider.h"

AttackArea::AttackArea(const Vector3& pos, const Vector3& ang, ObjectManager * objM, std::shared_ptr<ModelRenderer> modelRender, int num)

{
	position = pos;
	angle = ang;
	objManager = objM;
	number = num;
	mModelRender = modelRender;

	Init();
}

AttackArea::~AttackArea()
{
}

void AttackArea::DeathCountDown()
{
	//仮死亡フラグがtrueの時、カウントダウンをはじめ、カウントが0になったらほんとうにしぬ。
	if (isDestroy)
	{
		destroyCount++;

		if (destroyCount > destroyTime * 60)
		{
			death = true;
		}
	}
}

void AttackArea::SetDestroy(bool value, int time)
{
	isDestroy = value;
	destroyTime = time;
}

void AttackArea::Init()
{
#pragma region モデル初期化

	name = "AttackArea";
	key = to_string(number);
	keyname = name + key;

	mModelRender->AddModel(keyname, "Resouse/bill.obj", "Resouse/bill.png");
	mModelRender->SetAncPoint(keyname, Vector3(-2.0f, -2.0f, -2.0f));

#pragma endregion

	death = false;
	objType = ObjectType::ENEMYBULLET;
	damage = 5;

	destroyCount = 0;

	radius = scale.x * 1.0f;

	isActive = false;
	isDestroy = false;

	spehereCollider = new SphereCollider(position, 1);
	spehereCollider->SetRadiuse(1.0f);
	//SetCollidder(spehereCollider);
}

void AttackArea::Update()
{
	if (!isActive) return;

	//死亡のカウントダウン
	DeathCountDown();

    SetCollidder(spehereCollider);


	ImGuiDebug();
}

void AttackArea::Rend()
{
	if (!isActive) return;

	//DirectXManager::GetInstance()->SetData3D();
	//mModelRender->Draw(keyname, position,Vector3(angle.x, -angle.y, angle.z) , scale);
}

void AttackArea::OnCollison(BaseCollider* info)
{
	if (!isActive) return;
}

void AttackArea::ImGuiDebug()
{
	if (!isActive) return;

	//float pos[3] = { position.x,position.y,position.z };
	//ImGui::SliderFloat3("---Position---", pos, -500.0f, 500.0f);
	//float ang[3] = { angle.x,angle.y,angle.z };
	//ImGui::SliderFloat3("---Angle---", ang, -500.0f, 500.0f);
}

void AttackArea::SetActive(bool value, const Vector3& pos, const Vector3& ang, const Vector3& size)
{
	isActive = value;
	position = pos;
	angle = ang;
	scale = size;
	//半径を再設定
	spehereCollider->SetRadiuse(size.x / 2);
}

void AttackArea::SetDeath(bool value)
{
	death = value;
}
