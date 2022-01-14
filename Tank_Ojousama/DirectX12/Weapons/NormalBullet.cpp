#include "NormalBullet.h"
#include"Weapon.h"
#include "../Collision/SpherCollider.h"
#include"../Collision/BaseCollider.h"
NormalBullet::NormalBullet(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p, ObjectType t, int num,int upD)
	:BulletModel(m),BulletParticle(p)
{
	position = pos;
	objM = obj;
	angle = ang;
	setType = t;
	number = num;
	UpDamage = upD;
}

NormalBullet::~NormalBullet()
{
}

void NormalBullet::Init()
{
	SetBulletType();
	damage = 5 + UpDamage;
	objM->SetReloadTime(20);
	name = "NormalBullet";
	num = to_string(number);
	numName = name + num;
	BulletModel->AddModel(numName, "Resouse/Bullet.obj", "Resouse/Bullet.png");
	//BulletParticleBox = make_shared<ParticleEmitterBox>(BulletParticle);
	//BulletParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");
	alive = 0;
	death = false;
	speed = 1.5f;
	//コライダーの情報をセット
	SetCollidder(Vector3(0,0,0), 0.5f);
}

void NormalBullet::Update()
{
	alive++;
	if (alive >= 20)
	{
		velocity = Vector3(0, -0.04f, -1);
	}
	else
	{
		velocity = Vector3(0, 0, -1);
	}
	
	velocity *= Matrix4::RotateX(angle.x);
	velocity *= Matrix4::RotateY(angle.y);
	position += velocity * speed;


	if (alive >= 150)
	{
		death = true;
	}
}

void NormalBullet::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	BulletModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, 0), Vector3(0.7f, 0.7f, 0.7f));
}

void NormalBullet::ImGuiDebug()
{
}

void NormalBullet::OnCollison(BaseCollider * col)
{
	if (objType == BULLET && (col->GetColObject()->GetType() == ObjectType::ENEMY || col->GetColObject()->GetType() == ObjectType::BOSS || col->GetColObject()->GetType() == ObjectType::ENEMYBULLET || col->GetColObject()->GetType() == ObjectType::BLOCK))
	{
		//BulletParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}

	if (objType == ENEMYBULLET && (col->GetColObject()->GetType() == ObjectType::PLAYER || col->GetColObject()->GetType() == ObjectType::BULLET || col->GetColObject()->GetType() == ObjectType::BLOCK || col->GetColObject()->GetType() == ObjectType::ITEM))
	{
		//BulletParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}
}
