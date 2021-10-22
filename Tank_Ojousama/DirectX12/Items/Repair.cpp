#include "Repair.h"
#include "../Collision/SpherCollider.h"
#include"../Actor/Player.h"

Repair::Repair(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, ItemHolder* holder, ItemNames name, int num, int maxAlive)
{
	position = pos;
	angle = ang;
	objM = obj;
	objType = ObjectType::ITEM;
	itemName = name;
	itemHolder = holder;
	alive_max = maxAlive;
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
	ItemModel->AddModel(numName, "Resouse/item.obj", "Resouse/item1.png");
}

void Repair::Update()
{
	angle.x++;
	alive++;
	if (angle.x >= 360||!isGet)
	{
		angle.x = 0;
	}

	if (alive >= alive_max||!isGet)
	{
		death = true;
	}

	if (itemHolder->GetUseFlag())
	{
		active = true;
	}

	Heal(player);
}

void Repair::Rend()
{
	if (!isGet)
	{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		ItemModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, 0), Vector3(1, 1, 1));
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

void Repair::Heal(Player* player)
{
	if (!active)
	{
		return;
	}
	
	saveHP=objM->GetPlayer().GetHP()+healPoint;
	
	objM->GetPlayer().SetHP(saveHP);
}
