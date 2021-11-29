#include "Smoke.h"

Smoke::Smoke(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, ItemHolder * holder, ItemState itemStates, int num, int maxAlive, int addHp)
{
	position = pos;
	angle = ang;
	objM = obj;
	objType = ObjectType::ITEM;
	itemName = ItemNames::smock;
	itemState = itemStates;
	itemHolder = holder;
	alive_max = maxAlive;
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
	ItemModel->AddModel(numName, "Resouse/bill.obj", "Resouse/bill.png");
	ItemModel->SetAncPoint(numName, Vector3(-1.0f, -2.0f, -3.0f));
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

	if (alive >= alive_max && !isGet)
	{
		death = true;
	}

	if (itemHolder->GetUseFlag())
	{
		active = true;
	}

	Smoker();
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
}

void Smoke::Smoker()
{
	if (!active)
	{
		return;
	}

	objType = ObjectType::SMOKE;

	
}
