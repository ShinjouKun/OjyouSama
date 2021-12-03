#include "MashinGun.h"
#include"Weapon.h"
#include "../Collision/SpherCollider.h"
#include"../Collision/BaseCollider.h"
#include<random>
MashinGun::MashinGun(const Vector3& pos, const Vector3& ang, ObjectManager* obj,
	shared_ptr<ModelRenderer>m, shared_ptr<ParticleManager>p, ObjectType t, int num)
	:Model(m), Particle(p)
{
	position = pos;
	objM = obj;
	angle = ang;
	setType = t;
	number = num;
}

MashinGun::~MashinGun()
{
}

void MashinGun::Init()
{
	SetBulletType();
	damage = 1;
	objM->SetReloadTime(2);
	name = "MashinGun";
	num = to_string(number);
	numName = name + num;
	Model->AddModel(numName, "Resouse/bullet.obj", "Resouse/bullet.png");
	ParticleBox = make_shared<ParticleEmitterBox>(Particle);
	ParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");
	alive = 0;
	death = false;
	speed = 3.5f;
	//�R���C�_�[�̏����Z�b�g
	SetCollidder(Vector3(0.0f,0.0f,0.0f), 0.2f);
	random_device rnd;
	default_random_engine eng(rnd());
	uniform_int_distribution<int>dist(-4.0f, 4.0f);

	angle.x += dist(eng);
	angle.y += dist(eng);
}

void MashinGun::Update()
{
	velocity = Vector3(0, 0, -1);
	velocity *= Matrix4::RotateX(angle.x);
	velocity *= Matrix4::RotateY(angle.y);
	position += velocity * speed;

	alive++;
	if (alive >= 80)
	{
		death = true;
	}
}

void MashinGun::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g
	Model->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, 0), Vector3(0.2f, 0.2f, 0.2f));
}

void MashinGun::ImGuiDebug()
{
}

void MashinGun::OnCollison(BaseCollider * col)
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
