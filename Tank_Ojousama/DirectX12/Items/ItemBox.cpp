#include "ItemBox.h"
#include "Repair.h"
#include "Shield.h"
#include "../Collision/SpherCollider.h"

ItemBox::ItemBox(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, ItemHolder * holder,int Nomber,int itemNumber,int alive,int addHealth)
	:ItemModel(m),itemNum(itemNumber),objM(obj),iHolder(holder)
{
	itemNum = itemNumber;
	number = Nomber;
	position = pos;
	angle = ang;
	objType = ITEMBOX;
	itemAlive = alive;
	addHp = addHealth;
}

ItemBox::~ItemBox()
{
}

void ItemBox::Init()
{
	death = false;
	name = "ItemBox";
	num = to_string(number);
	numName = name + num;
	ItemModel->AddModel(numName, "Resouse/item_box.obj", "Resouse/item_box.png");
	ItemModel->SetAncPoint(numName, Vector3(-1.0f, -2.0f, -3.0f));
}

void ItemBox::Update()
{
}

void ItemBox::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	ItemModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, angle.z), Vector3(1, 1, 1));
}

void ItemBox::ImGuiDebug()
{
	
}

void ItemBox::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::PLAYER || col->GetColObject()->GetType()== ObjectType::BULLET)
	{
		ItemSpawn();
		death = true;
	}
}

void ItemBox::ItemSpawn()
{
	switch (itemNum)
	{
	case 1:
		objM->Add(new Repair(position, angle, objM, ItemModel, Low, number, itemAlive, addHp));
		break;
	case 2:
		objM->Add(new Repair(position, angle, objM, ItemModel, High, number, itemAlive, addHp));
		break;
	case 3:
		objM->Add(new Shield(position, angle, objM, ItemModel, Low, number, itemAlive, addHp));
		break;
	case 4:
		objM->Add(new Shield(position, angle, objM, ItemModel, High, number, itemAlive, addHp));
		break;
	default:
		break;
	}

}
