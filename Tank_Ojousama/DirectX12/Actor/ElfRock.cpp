#include "ElfRock.h"

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

	isActive = true;

	switch (mRockScale)
	{
	case 1:
		scale = Vector3(3.0f, 3.0f, 3.0f);
		radius = 1.5f;
		//コライダーの情報をセット
		SetCollidder(Vector3(position.x, position.y, position.z), radius);
		mModelRender->AddModel(numName, "Resouse/stone_s.obj", "Resouse/stone-s_color.png");
		break;
	case 2:
		scale = Vector3(2.0f, 2.0f, 2.0f);
		radius = 1.5f;
		//コライダーの情報をセット
		SetCollidder(Vector3(position.x, position.y, position.z), radius);
		mModelRender->AddModel(numName, "Resouse/stone_m.obj", "Resouse/stone_ma.png");
		break;
	case 3:
		scale = Vector3(5.0f, 5.0f, 5.0f);
		radius = 2.5f;
		//コライダーの情報をセット
		SetCollidder(Vector3(position.x, position.y, position.z), radius);
		mModelRender->AddModel(numName, "Resouse/stone_big.obj", "Resouse/stone_bigA.png");
		break;
	}
}

void ElfRock::Update()
{
	Vector3 distance = mObjManager->GetPlayer().GetPosition() - position;
	float length = distance.Length();

	if (length > 100.0f)
	{
		isActive = false;
	}
	else
	{
		isActive = true;
	}
}

void ElfRock::Rend()
{
	if (!isActive) return;
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(numName, position, angle, scale);
}

void ElfRock::OnCollison(BaseCollider * col)
{
}

void ElfRock::ImGuiDebug()
{
}
