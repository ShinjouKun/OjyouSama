#include "Repair.h"
#include "../Collision/SpherCollider.h"
#include "../Sound/Sound.h"
#include"../Scene/BaseScene.h"

Repair::Repair(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, shared_ptr<TexRenderer>s, ItemState itemStates, int num, int aliveNum, int addHp)
	:ItemModel(m),ItemUseTex(s),getSE(nullptr),healSE(nullptr)
{
	position = pos;
	angle = ang;
	objM = obj;
	objType = ObjectType::ITEM;
	itemName = ItemNames::heal;
	itemState = itemStates;
	healPoint = addHp;
	number = num;
	alive = aliveNum;
}

Repair::~Repair()
{
	
}

void Repair::Init()
{
	death = false;
	isGet = false;
	active = false;
	itemFade = 1.0f;
	fadeCount = 0;
	SetCollidder(Vector3(0,0,0), 1.0f);
	alive = 0;
	name = "Repair";
	num = to_string(number);
	numName = name + num;
	ItemModel->AddModel(numName, "Resouse/item.obj", "Resouse/item1.png");
	ItemUseTex->AddTexture(numName, "Resouse/heal.png");
	ItemModel->SetAncPoint(numName, Vector3(0, 2.0f, 0));
	getSE = std::make_shared<Sound>("SE/getItem.mp3", false);
	healSE = std::make_shared<Sound>("SE/heal.mp3", false);
	if (itemState == ItemState::Low)
	{
		healPoint = 20;
	}
}

void Repair::Update()
{
	angle.y++;
	alive++;
	if (angle.z >= 360&&!isGet)
	{
		angle.z = 0;
	}


	if (ItemHolder::GetInstance()->GetUseRepair())
	{
		active = true;
	}

	Heal();
	getSE->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
	healSE->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);

	FallDown();
}

void Repair::Rend()
{
	if (!isGet)
	{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		ItemModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, angle.z), Vector3(1.5f, 1.5f, 1.5f));
	}
	if (active)
	{
		DirectXManager::GetInstance()->SetData2D();
		ItemUseTex->Draw(numName, Vector3(0, 0, 0), 0, Vector2(1, 1), Vector4(1, 1, 1, itemFade));
	}
}

void Repair::ImGuiDebug()
{
}

void Repair::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::PLAYER&&!isGet)
	{
		ItemHolder::GetInstance()->AddItem(itemName);
		isGet = true;
		getSE->play();
	}
}

void Repair::Heal()
{
	if (!active)
	{
		return;
	}
	itemFade -= 0.1f;
	fadeCount++;
	healSE->play();
	if (fadeCount >= 30)
	{
		saveHP = objM->GetPlayer().GetHP() + healPoint;
		fadeCount = 0;
		itemFade = 1.0f;
		objM->GetPlayer().SetHP(saveHP);
		active = false;
		ItemHolder::GetInstance()->SetUseRepair(false);
		death = true;
	}
}

void Repair::FallDown()
{
	if (position.y >= 0.0f)
	{
		position += Vector3(0.0f, -1.0f, 0.0f) * 0.3f;
	}
}
