#include "Bullet.h"
//#include"Matrix4.h"
#include "../Math/Matrix4.h"

Bullet::Bullet(Vector3 pos, Vector3 ang, ObjectManager * obj, std::shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p,ObjectType t,int num)
	:BulletModel(m),BulletParticle(p)
{
	position = pos;
	objM = obj;
	angle = ang;
	setType = t;
	number = num;
}

Bullet::~Bullet()
{
	
}

void Bullet::Init()
{
	SetBulletType();
	name = "Bullet";
	num = to_string(number);
	numName = name + num;
	BulletModel->AddModel(numName,"Resouse/Bullet.obj","Resouse/Bullet.png");
	BulletParticleBox = make_shared<ParticleEmitterBox>(BulletParticle);
	BulletParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");
	alive = 0;
	death = false;
	speed = 1.5f;
	
}

void Bullet::Update()
{
	velocity = Vector3(0,0,-1);
	velocity *= Matrix4::RotateX(angle.x);
	velocity *= Matrix4::RotateY(angle.y);
	position += velocity * speed;
	
	alive++;
	if (alive >= 150)
	{
		death = true;
	}
}

void Bullet::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	BulletModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, 0), Vector3(1, 1, 1));
}

void Bullet::SetBulletType()
{
	switch (setType)
	{
	case PLAYER:
		objType = ObjectType::BULLET;//プレイヤーの弾	
		break;
	case ENEMY:
	case BOSS:
		objType = ObjectType::ENEMYBULLET;//敵の弾
		break;
	default:
		break;
	}
}

void Bullet::OnCollison(const CollisonInfo & info)
{
	if (objType == BULLET && (info.object->GetType() == ObjectType::ENEMY || info.object->GetType() == ObjectType::BOSS || info.object->GetType() == ObjectType::ENEMYBULLET || info.object->GetType() == ObjectType::BLOCK))
	{
		BulletParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}

	if (objType == ENEMYBULLET && (info.object->GetType() == ObjectType::PLAYER || info.object->GetType() == ObjectType::BULLET || info.object->GetType() == ObjectType::BLOCK))
	{
		BulletParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}
}

void Bullet::ImGuiDebug()
{
}
