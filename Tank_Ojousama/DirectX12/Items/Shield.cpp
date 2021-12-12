#include "Shield.h"
#include "../Collision/SpherCollider.h"


Shield::Shield(const Vector3& pos, const Vector3& ang, ObjectManager* obj, shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, ItemState itemStates, int num, int addHp) :ItemModel(m),itemUseTex(s)
{
	position = pos;
	angle = ang;
	objM = obj;
	objType = ObjectType::ITEM;
	itemName = ItemNames::dome;
	itemState = itemStates;
	//itemHolder = holder;
	guadePoint = addHp;
}

Shield::~Shield()
{
}

void Shield::Init()
{
	death = false;
	isGet = false;
	active = false;
	SetCollidder(Vector3(0, 0, 0), 0.5f);
	alive = 0;
	name = "Repair";
	num = to_string(number);
	numName = name + num;
	ItemModel->AddModel(numName, "Resouse/shield.obj", "Resouse/shield.png");
	ItemModel->SetAncPoint(numName, Vector3(-1.0f, -2.0f, -3.0f));
	itemUseTex->AddTexture(numName, "");
	if (itemState == ItemState::Low)
	{
		guadePoint = 20;
	}
}

void Shield::Update()
{
	angle.y++;
	alive++;
	if (angle.z >= 360 && !isGet)
	{
		angle.z = 0;
	}

	/*if (alive >= alive_max && !isGet)
	{
		death = true;
	}*/

	if (ItemHolder::GetInstance()->GetUseFlag())
	{
		active = true;
	}
}

void Shield::Rend()
{
	if (!isGet)
	{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		ItemModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, angle.z), Vector3(1, 1, 1));
	}
}

void Shield::ImGuiDebug()
{
}

void Shield::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::PLAYER)
	{
		ItemHolder::GetInstance()->AddItem(itemName);
		isGet = true;
	}

	if (col->GetColObject()->GetType() == ObjectType::ENEMYBULLET)
	{
		damege = col->GetColObject()->GetDamage();
		Guade();
	}
}

void Shield::Guade()
{
	if (!active)
	{
		return;
	}

	guadePoint -= damage;

	if (guadePoint <= 0)
	{
		active = false;
		death = true;
	}

}
