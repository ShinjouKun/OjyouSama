#include "TaihouEnemy.h"
#include"Bullet.h"

TaihouEnemy::TaihouEnemy(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager> p,int num)
	:taihouModel(m),taihouParticle(p)
{
	position = pos;
	angle = ang;
	objM = obj;
	number = num;
}

TaihouEnemy::~TaihouEnemy()
{
}

void TaihouEnemy::Shot()
{
	objM->Add(new Bullet(Vector3(position.x, position.y+3.0f, position.z), Vector3(angle.x, angle.y, angle.z), objM, taihouModel, taihouParticle, objType, bulletStock));
	ShotCount = 0;
	ShotFlag = false;
}

void TaihouEnemy::Move()
{
	Count++;

	if (Count <= 160)
	{
		angle.y += 0.3f;
	}
	else
	{
		angle.y -= 0.3f;
		if (Count >= 320)
		{
			Count = 0;
		}
	}
}

void TaihouEnemy::Init()
{
	Count = 0;
	HP = 40;
	death = false;
	//position = Vector3(40.0f, 8.0f, -180.0f);
	//SphereSize = 2.5f;
	ShotFlag = false;
	HitFlag = false;
	ShotCount = 0;
	HitCount = 0;
	objType = ObjectType::ENEMY;
	name = "Taihou";
	num = to_string(number);
	numName = name + num;
	taihouModel->AddModel(numName, "Resouse/EnemyHou.obj", "Resouse/EnemyHou.png");
	taihouModel->SetAncPoint(numName, Vector3(-2.0f, -2.0f, -2.0f));
	name2 = "Daiza";
	num = to_string(number);
	numName2 = name2 + num;
	taihouModel->AddModel(numName2, "Resouse/EnemyDaiza.obj", "Resouse/EnemyDaiza.png");
	taihouModel->SetAncPoint(numName2, Vector3(-2.0f, -2.0f, -2.0f));

	/*taihouParticleBox = make_shared<ParticleEmitterBox>(taihouParticle);
	taihouParticleBox->LoadAndSet("HIT", "Resouse/Bom.jpg");*/
	bulletStock = number*10;
}

void TaihouEnemy::Update()
{
	Move();
	//“–‚½‚è
	if (HitFlag)
	{
		HitCount++;
		if (HitCount >= 30)
		{
			HitCount = 0;
			HitFlag = false;
		}
	}
	if (HP == 0)
	{
		death = true;
	}
	//UŒ‚

	if (!ShotFlag)
	{
		ShotCount++;
		if (ShotCount >= 120)
		{
			ShotFlag = true;
			Shot();
		}
	}
	//‹…”ãŒÀ‚ðÝ‚¯
	if (bulletStock >= 30)
	{
		bulletStock = number*10;
	}
}

void TaihouEnemy::Rend()
{
	DirectXManager::GetInstance()->SetData3D();
	taihouModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, 0), Vector3(5,5,5));
	taihouModel->Draw(numName2, Vector3(position.x, position.y, position.z), Vector3(0,0,0), Vector3(5,5,5));
}


void TaihouEnemy::OnCollison(const CollisonInfo & info)
{
	if (!HitFlag&& info.object->GetType() == ObjectType::BULLET)
	{
		HP -= 20;
		HitFlag = true;//–³“GŽžŠÔ‚É“ü‚é
	}
}

void TaihouEnemy::ImGuiDebug()
{
}
