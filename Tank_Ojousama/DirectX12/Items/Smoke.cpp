#include "Smoke.h"
#include "../Collision/SpherCollider.h"
#include "../Sound/Sound.h"
#include"../Scene/BaseScene.h"

Smoke::Smoke(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, ItemState itemStates, int num, int addHp)
{
	position = pos;
	angle = ang;
	objM = obj;
	objType = ObjectType::ITEM;
	itemName = ItemNames::smock;
	itemState = itemStates;
	smokePoint = addHp;
	num = number;
}

Smoke::~Smoke()
{
}

void Smoke::Init()
{
	death = false;
	isGet = false;
	active = false;
	SetCollidder(Vector3(0, 0, 0), 0.5f);
	alive = 0;
	name = "Smoke";
	num = to_string(number);
	numName = name + num;
	ItemModel->AddModel(numName, "Resouse/smokeModel.obj", "Resouse/smokeModel.png");
	ItemModel->SetAncPoint(numName, Vector3( 0, 2.0f, 0));
	getSE = std::make_shared<Sound>("SE/getItem.mp3", false);
	if (itemState == ItemState::Low)
	{
		smokePoint = 20;
	}
}

void Smoke::Update()
{
	angle.y++;
	alive++;
	if (angle.z >= 360 && !isGet)
	{
		angle.z = 0;
	}


	if (ItemHolder::GetInstance()->GetUseFlag())
	{
		active = true;
	}

	Smoker();
	getSE->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
}

void Smoke::Rend()
{
	if (!isGet)
	{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		ItemModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, angle.z), Vector3(1, 1, 1));
	}
}

void Smoke::ImGuiDebug()
{
}

void Smoke::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::PLAYER)
	{
		ItemHolder::GetInstance()->AddItem(itemName);
		isGet = true;
		getSE->play();
	}
}

void Smoke::Smoker()
{
	if (!active)
	{
		return;
	}

	objType = ObjectType::SMOKE;

	
}
