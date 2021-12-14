#include "LandMine.h"
#include"Weapon.h"
#include "../Collision/SpherCollider.h"
#include"../Collision/BaseCollider.h"
#include "../ParticleSystem/ParticleType/Explosion.h"

LandMine::LandMine(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p, ObjectType t, int num,int upD)
	:Model(m),Particle(p)
{
	position = pos;
	objM = obj;
	angle = ang;
	setType = t;
	number = num;
	UpDamage = upD;
}

LandMine::~LandMine()
{
}

void LandMine::Init()
{
	SetBulletType();
	damage = 2 + UpDamage;
	objM->SetReloadTime(100);
	name = "Mine";
	num = to_string(number);
	numName = name + num;
	Model->AddModel(numName, "Resouse/mine.obj", "Resouse/mine.png");
	Model->SetAncPoint(numName, Vector3(2.0f, 2.0f, 0.0f));
	ParticleBox = make_shared<ParticleEmitterBox>(Particle);
	ParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");
	alive = 0;
	death = false;
	bomFlag = false;
	speed = 1.5f;
	bomSpace = 1.0f;
	//コライダーの情報をセット
	SetCollidder(Vector3(0,2.5f,0), bomSpace);
}

void LandMine::Update()
{
	//おきっぱでもいいけど残ったままなのもあれなので
	alive++;
	
	if (alive >= 300)
	{
		death = true;
	}
	if (bomFlag)
	{
		bomSpace += 0.2f;
	}
	if (bomSpace >= 6.0f)
	{
		death = true;
	}

	if (!mParticle && death)
	{
		mParticle = std::make_shared<Explosion>(position);
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
		bomFlag = true;
		//ParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
	}

	if (objType == ENEMYBULLET && (col->GetColObject()->GetType() == ObjectType::PLAYER || col->GetColObject()->GetType() == ObjectType::BULLET || col->GetColObject()->GetType() == ObjectType::BLOCK || col->GetColObject()->GetType() == ObjectType::ITEM))
	{
		bomFlag = true;
		//ParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
	}

	
}
