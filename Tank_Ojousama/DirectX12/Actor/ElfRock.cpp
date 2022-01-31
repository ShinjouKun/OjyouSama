#include "ElfRock.h"
#include "../Collision/BaseCollider.h"
ElfRock::ElfRock(const Vector3 & pos, const Vector3 & ang, ObjectManager* objManager, std::shared_ptr<ModelRenderer> modelRender, int num, int rockScale)
	:mModelRender(modelRender)
{
	position = pos;
	angle = ang;
	number = num;
	mObjManager = objManager;
	mRockScale = rockScale;
}

ElfRock::~ElfRock()
{
}

void ElfRock::Init()
{
	death = false;
	objType = ObjectType::BLOCK;

	name = "ElfRock";
	num = to_string(number);
	numName = name + num;

	SetActive(false);

	switch (mRockScale)
	{
	case 1:
		scale = Vector3(3.0f, 3.0f, 3.0f);
		radius = 1.5f;
		//コライダーの情報をセット
		SetCollidder(Vector3(position.x, position.y, position.z), radius);
		mModelRender->AddModel(numName, "Resouse/stone_s.obj", "Resouse/stone-s_color.png");
		mModelRender->SetAncPoint(numName, Vector3(0.0f, -2.0f, 0.0f));
		break;
	case 2:
		scale = Vector3(2.0f, 2.0f, 2.0f);
		radius = 1.5f;
		//コライダーの情報をセット
		SetCollidder(Vector3(0,0,0), radius);//カメラ用
		//SetCollidder(Vector3(-2,-4,-2), Vector3(2,4,2));//視認範囲
		mModelRender->AddModel(numName, "Resouse/stone_m.obj", "Resouse/stone_ma.png");
		mModelRender->SetAncPoint(numName, Vector3(0.0f, -2.0f, 0.0f));
		break;
	case 3:
		scale = Vector3(4.0f, 4.0f, 4.3f);
		radius = 1.63f;
		//コライダーの情報をセット
		SetCollidder(Vector3(0.0f,0,0), radius);
		mModelRender->AddModel(numName, "Resouse/stone_big.obj", "Resouse/stone_bigA.png");
		mModelRender->SetAncPoint(numName, Vector3(0.0f, -1.5f, 0.0f));
		break;
	}
}

void ElfRock::Update()
{
	ImGuiDebug();
}

void ElfRock::Rend()
{
	if (GetActive())
	{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		mModelRender->Draw(numName, position, angle, scale);
	}
	
}

void ElfRock::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}
}

void ElfRock::ImGuiDebug()
{
	/*ImGui::Checkbox("RockActive", &IsActive);
	float pos[3] = { position.x,position.y,position.z };
	ImGui::SliderFloat3("RockPos", pos, -10, 10);*/
}
