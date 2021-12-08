#include "ShotGunBullet.h"
#include"Weapon.h"
#include "../Collision/SpherCollider.h"
#include"../Collision/BaseCollider.h"

ShotGunBullet::ShotGunBullet(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p, ObjectType t, int num,int upD)
	:Model(m),Particle(p)
{
	position = pos;
	objM = obj;
	angle = ang;
	setType = t;
	number = num;
	UpDamage = upD;
}

ShotGunBullet::~ShotGunBullet()
{
}

void ShotGunBullet::Init()
{
	SetBulletType();
	damage = 3 + UpDamage;
	objM->SetReloadTime(60);
	name = "ShotGun";
	num = to_string(number);
	numName = name + num;
	Model->AddModel(numName, "Resouse/Bullet.obj", "Resouse/Bullet.png");
	ParticleBox = make_shared<ParticleEmitterBox>(Particle);
	ParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");
	alive = 0;
	death = false;
	speed = 2.5f;
	//コライダーの情報をセット
	SetCollidder(Vector3(0.0f,0.0f,0.0f), 0.6f);
}

void ShotGunBullet::Update()
{
	velocity = Vector3(0, 0, -1);
	velocity *= Matrix4::RotateX(angle.x);
	velocity *= Matrix4::RotateY(angle.y);
	position += velocity * speed;

	alive++;
	if (alive >= 20)
	{
		death = true;
	}
}

void ShotGunBullet::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	Model->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, 0), Vector3(0.6f,0.6f,0.6f));
}

void ShotGunBullet::ImGuiDebug()
{
}

void ShotGunBullet::OnCollison(BaseCollider * col)
{
	if (objType == BULLET && (col->GetColObject()->GetType() == ObjectType::ENEMY || col->GetColObject()->GetType() == ObjectType::BOSS || col->GetColObject()->GetType() == ObjectType::ENEMYBULLET || col->GetColObject()->GetType() == ObjectType::BLOCK))
	{
		ParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}

	if (objType == ENEMYBULLET && (col->GetColObject()->GetType() == ObjectType::PLAYER || col->GetColObject()->GetType() == ObjectType::BULLET || col->GetColObject()->GetType() == ObjectType::BLOCK || col->GetColObject()->GetType() == ObjectType::ITEM))
	{
		ParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}
}
