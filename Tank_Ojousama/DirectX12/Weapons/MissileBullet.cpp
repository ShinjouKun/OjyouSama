#include "MissileBullet.h"
#include"Weapon.h"
#include "../Collision/SpherCollider.h"
#include"../Collision/BaseCollider.h"
MissileBullet::MissileBullet(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p, ObjectType t, int num)
	:Model(m), Particle(p)
{
	position = pos;
	objM = obj;
	angle = ang;
	setType = t;
	number = num;
}

MissileBullet::~MissileBullet()
{
}

void MissileBullet::Init()
{
	SetBulletType();
	damage = 10;
	objM->SetReloadTime(60);
	name = "NormalBullet";
	num = to_string(number);
	numName = name + num;
	Model->AddModel(numName, "Resouse/Bullet.obj", "Resouse/Bullet.png");
	ParticleBox = make_shared<ParticleEmitterBox>(Particle);
	ParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");
	alive = 0;
	death = false;
	speed = 0.5f;
	attackStart = 0;
	//コライダーの情報をセット
	SetCollidder(Vector3(position.x, position.y, position.z), 0.5f);
	LookOn();
	
}

void MissileBullet::Update()
{
	
	attackStart++;
	velocity = Vector3(0, 1.5f, 0);
	if (attackStart >= 10)
	{
		velocity = Vector3(0, 0, -1);
		velocity *= Matrix4::RotateX(angle.x);
		velocity *= Matrix4::RotateY(angle.y);
		
		if (targetSet)
		{
			alive++;
		
			if (alive >= 300)
			{
				death = true;
			}
			if (!obj)
			{
				targetSet = false;
				death = true;
				return;
			}
			if(obj->GetDeath())
			{
				targetSet = false;
			}
			else
			{
				velocity = Look();
			}

		}
		position += velocity * speed;
	}
	position += velocity * speed;
}

void MissileBullet::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	Model->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, 0), Vector3(1, 1, 1));
}

void MissileBullet::ImGuiDebug()
{
}

void MissileBullet::OnCollison(BaseCollider * col)
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

void MissileBullet::LookOn()
{
	vector<BaseObject*>targes = objM->getUseList();
	size_t size = targes.size();
	Vector3 targetPos;
	Vector3 v;
	float len = 500.0f;
	for (int i = 0; i < size; i++)
	{
		if (targes[i]->GetType() == ObjectType::ENEMY || targes[i]->GetType() == ObjectType::BOSS
			&&objType == ObjectType::BULLET)
		{
			targetPos = targes[i]->GetPosition();
			float length = Vector3((position - targetPos)).Length();
			if (length < abs(len))
			{
				len = length;
				obj = targes[i];
				targetSet = true;//敵発見
			}
		}

	}

}

Vector3 MissileBullet::Look()
{
		Vector3 v = Vector3(0, 0, 0);
		v = position - obj->GetPosition();
		v = -v.normal();
		return v;
}
