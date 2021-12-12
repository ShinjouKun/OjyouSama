#include "Repair.h"
#include "../Collision/SpherCollider.h"

Repair::Repair(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, ItemState itemStates, int num, int aliveNum, int addHp):ItemModel(m),ItemUseTex(s)
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
	SetCollidder(Vector3(0,0,0), 0.5f);
	alive = 0;
	name = "Repair";
	num = to_string(number);
	numName = name + num;
	ItemModel->AddModel(numName, "Resouse/item.obj", "Resouse/item1.png");
	ItemUseTex->AddTexture(numName, "Resouse/heal.png");
	ItemModel->SetAncPoint(numName, Vector3(1.0f, 2.0f, 3.0f));
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


	if (ItemHolder::GetInstance()->GetUseFlag())
	{
		active = true;
	}

	Heal();
}

void Repair::Rend()
{
	if (!isGet)
	{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		ItemModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, angle.z), Vector3(1, 1, 1));
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
	if (col->GetColObject()->GetType() == ObjectType::PLAYER)
	{
		ItemHolder::GetInstance()->AddItem(itemName);
		isGet = true;
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
	
	if (fadeCount >= 30)
	{
		saveHP = objM->GetPlayer().GetHP() + healPoint;
		fadeCount = 0;
		itemFade = 1.0f;
		objM->GetPlayer().SetHP(saveHP);
		active = false;
		ItemHolder::GetInstance()->SetUseFlag(false);
		death = true;
	}
}
