#include "Repair.h"
#include "../Collision/SpherCollider.h"

Repair::Repair(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, ItemHolder* holder, ItemState itemStates, int num, int maxAlive, int addHp):ItemModel(m)
{
	position = pos;
	angle = ang;
	objM = obj;
	objType = ObjectType::ITEM;
	itemName = ItemNames::heal;
	itemState = itemStates;
	itemHolder = holder;
	alive_max = maxAlive;
	healPoint = addHp;
}

Repair::~Repair()
{
	
}

void Repair::Init()
{
	death = false;
	isGet = false;
	active = false;
	SetCollidder(Vector3(position.x, position.y, position.z), 0.5f);
	alive = 0;
	name = "Repair";
	num = to_string(number);
	numName = name + num;
	ItemModel->AddModel(numName, "Resouse/bill.obj", "Resouse/bill.png");
	ItemModel->SetAncPoint(numName, Vector3(-1.0f, -2.0f, -3.0f));
	if (itemState == ItemState::Normal)
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

	if (alive >= alive_max&&!isGet)
	{
		death = true;
	}

	if (itemHolder->GetUseFlag())
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
}

void Repair::ImGuiDebug()
{
}

void Repair::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::PLAYER)
	{
		itemHolder->AddItem(itemName);
		isGet = true;
	}
}

void Repair::Heal()
{
	if (!active)
	{
		return;
	}
	
	saveHP=objM->GetPlayer().GetHP()+healPoint;
	
	objM->GetPlayer().SetHP(saveHP);
	active = false;
	itemHolder->SetUseFlag(false);
}
