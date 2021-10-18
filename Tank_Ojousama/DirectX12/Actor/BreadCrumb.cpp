#include "BreadCrumb.h"
#include "../Collision/SpherCollider.h"

BreadCrumb::BreadCrumb(Vector3 pos, ObjectManager * objManager, shared_ptr<ModelRenderer> modelRender, int num)
	:modelRender(modelRender)
{
	position = pos;
	objectManager = objManager;
	keyNumber = num;
}

BreadCrumb::~BreadCrumb()
{
}

void BreadCrumb::Init()
{
#pragma region モデル初期化

	name = "Bread";
	key = to_string(keyNumber);
	keyName = name + key;
	modelRender->AddModel(keyName, "Resouse/Bullet.obj", "Resouse/Bullet.png");

#pragma endregion

	//個体番号の登録
	AiID = keyNumber;
	objType = ObjectType::BREADCRUMB;
	death = false;

	scale = Vector3(1, 1, 1);

	lifeCount = 0;
	lifeTime = 3 * 60;

	radius = 1.0f;
	SetCollidder(new SphereCollider(position, radius));
}

void BreadCrumb::Update()
{
	ImGuiDebug();
	LifeCountDown(lifeTime);
}

void BreadCrumb::Rend()
{
	//DirectXManager::GetInstance()->SetData3D();
	//modelRender->Draw(keyName, position, angle, scale);
}

void BreadCrumb::OnCollison(BaseCollider* info)
{
	//if (info.object->GetType() == ObjectType::ENEMY)
	//{
	//	death = true;
	//}
}

void BreadCrumb::ImGuiDebug()
{
	////点(プレイヤーとなるもの)情報の可視化
	//float pos[3] = { position.x ,position.y ,position.z };
	//ImGui::SliderFloat3("PointPosition", pos, -500.0f, 500.0f);
	////扇と点が当たってたらチェックがつくよ。
	//ImGui::Checkbox("IsDeath", &death);

	//ImGui::Text("BreadCrumb");
}

void BreadCrumb::LifeCountDown(int count)
{
	if (!death)
	{
		lifeCount++;
		if (lifeCount > count)
		{
			lifeCount = 0;
			death = true;
		}
	}
}
