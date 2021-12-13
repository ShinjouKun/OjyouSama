#include "StoneWeapon.h"
#include"Weapon.h"
#include "../Collision/SpherCollider.h"
#include"../Collision/BaseCollider.h"
StoneWeapon::StoneWeapon(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p, ObjectType t, int num)
	:Model(m), Particle(p)
{
	position = pos;
	objM = obj;
	angle = ang;
	setType = t;
	number = num;
}

StoneWeapon::~StoneWeapon()
{
}

void StoneWeapon::Init()
{
	SetBulletType();
	attckStart = 0;
	z = 0;
	damage = 10;
	name = "Stone";
	num = to_string(number);
	numName = name + num;
	Model->AddModel(numName, "Resouse/throw.obj", "Resouse/throw_ss.png");
	/*ParticleBox = make_shared<ParticleEmitterBox>(Particle);
	ParticleBox->LoadAndSet("Bom", "Resouse/Bom.jpg");*/
	alive = 0;
	death = false;
	speed = 1.5f;
	//コライダーの情報をセット
	SetCollidder(Vector3(0,0,0), 2.5f);
}

void StoneWeapon::Update()
{
	
	if (attckStart >= 10)
	{
		velocity = Vector3(0, 0, -0.5f);
		velocity *= Matrix4::RotateX(angle.x);
		velocity *= Matrix4::RotateY(angle.y);
		

		alive++;
		if (alive >= 150)
		{
			death = true;
		}
	}
	else
	{
		attckStart++;
		velocity = Vector3(0, 0.13f, 0);
	}
	position += velocity * speed;
	
}

void StoneWeapon::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	z += 2.5f;
	Model->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, z), Vector3(1, 1, 1));
}

void StoneWeapon::ImGuiDebug()
{
}

void StoneWeapon::OnCollison(BaseCollider * col)
{
	if (objType == ENEMYBULLET && (col->GetColObject()->GetType() == ObjectType::PLAYER || col->GetColObject()->GetType() == ObjectType::BULLET || col->GetColObject()->GetType() == ObjectType::BLOCK || col->GetColObject()->GetType() == ObjectType::ITEM))
	{
		//ParticleBox->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
		death = true;
	}
}
