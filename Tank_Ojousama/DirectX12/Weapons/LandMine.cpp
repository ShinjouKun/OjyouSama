#include "LandMine.h"
#include"Weapon.h"
#include "../Collision/SpherCollider.h"
#include"../Collision/BaseCollider.h"
LandMine::LandMine(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p, ObjectType t, int num)
	:Model(m),Particle(p)
{
	position = pos;
	objM = obj;
	angle = ang;
	setType = t;
	number = num;
}

LandMine::~LandMine()
{
}

void LandMine::Init()
{
	SetBulletType();
	damage = 1;
	objM->SetReloadTime(100);
	name = "Mine";
	num = to_string(number);
	numName = name + num;
	Model->AddModel(numName, "Resouse/mine.obj", "Resouse/mine.png");
	//Model->SetAncPoint("Mine", Vector3(-200.0f, 0.0f, 0.0f));
	ParticleBox = make_shared<ParticleEmitterBox>(Particle);
	ParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");
	alive = 0;
	death = false;
	speed = 1.5f;
	//コライダーの情報をセット
	SetCollidder(Vector3(position.x-2.2f, position.y - 1.5f, position.z), 1.0f);
}

void LandMine::Update()
{
	//おきっぱでもいいけど残ったままなのもあれなので
	alive++;
	if (alive >= 1000)
	{
		death = true;
	}
}

void LandMine::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	Model->Draw(numName, Vector3(position.x-2.2f, position.y - 1.5f, position.z), Vector3(angle.x, angle.y, 0), Vector3(1, 1, 1));
}

void LandMine::ImGuiDebug()
{
}

void LandMine::OnCollison(BaseCollider * col)
{
	if (objType == BULLET && (col->GetColObject()->GetType() == ObjectType::ENEMY || col->GetColObject()->GetType() == ObjectType::BOSS || col->GetColObject()->GetType() == ObjectType::ENEMYBULLET || col->GetColObject()->GetType() == ObjectType::BLOCK))
	{
		ParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}

	if (objType == ENEMYBULLET && (col->GetColObject()->GetType() == ObjectType::PLAYER || col->GetColObject()->GetType() == ObjectType::BULLET || col->GetColObject()->GetType() == ObjectType::BLOCK))
	{
		ParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}
}
