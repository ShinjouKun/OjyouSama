#include "Item.h"
#include "../Collision/BaseCollider.h"

Item::Item(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, int aliv, int n) :itemModel(m)
{
	position = pos;
	angle = ang;
	objM = obj;
	number = n;
	alive = aliv;
}

Item::~Item()
{
}

void Item::Init()
{
	death = false;
	objType = ObjectType::ITEM;
	name = "Item";
	num = to_string(number);
	numName = name + num;
	angle = Vector3(0, 0, 0);

}

void Item::Update()
{
	angle.x++;
	alive--;
	if (angle.x >= 360)
	{
		angle.x = 0;
	}

	if (alive <= 0)
	{
		death = true;
	}
}

void Item::Rend()
{
	DirectXManager::GetInstance()->SetData3D();
	itemModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, 0), Vector3(1, 1, 1));
}

void Item::OnCollison(BaseCollider* col)
{

	if (col->GetColObject()->GetType() == ObjectType::PLAYER);
	{
		death = true;
	}

}

void Item::ImGuiDebug()
{
}