#include "Castle.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"


Castle::Castle(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> modelRender, shared_ptr<TexRenderer>s,shared_ptr<ParticleManager> effect)
	:Model(modelRender),Sprite(s),Particle(effect)
{
	position = pos;
	angle = ang;
	objM = obj;
}

Castle::~Castle()
{
}

void Castle::Init()
{
	Model->AddModel("DefCanp", "Resouse/castle.obj", "Resouse/castle.png");
	Sprite->AddTexture("Messege1", "Resouse/messege1.png");
	Sprite->AddTexture("Messege2", "Resouse/messege2.png");
	Sprite->AddTexture("Messege3", "Resouse/messege3.png");

	HP = 50;//‚Æ‚è‚ ‚¦‚¸
	MaxHp = HP;
	death = false;
	HitFlag = false;
	HitCount = 0;
	objType = ObjectType::DEFENCEPOINT;
	SetCollidder(Vector3(0, 0, 0), 10.0f);
	MessFlag1 = false;
	MessFlag2 = false;
	MessFlag3 = false;
	MessCount1 = 0;
	MessCount2 = 0;
	MessCount3 = 0;
}

void Castle::Update()
{
	if (HitFlag)
	{
		HitCount++;
		if (HitCount >= 300)
		{
			HitCount = 0;
			HitFlag = false;
		}
		return;
	}
	if (MessFlag1)
	{
		MessCount1 += 1;
	}
	if (MessFlag2)
	{
		MessCount2 += 1;
	}	
	if (MessFlag3)
	{
		MessCount3 += 1;
	}
	if (HP <= MaxHp * 0.75f)
	{
		MessFlag1 = true;
	}
	if (HP <= MaxHp / 2)
	{
		MessFlag1 = false;
		MessFlag2 = true;
	
	}
	if (HP <= MaxHp * 0.25f)
	{
		MessFlag1 = false;
		MessFlag2 = false;
		MessFlag3 = true;
	}
}

void Castle::Rend()
{
	DirectXManager::GetInstance()->SetData3D();
	Model->Draw("DefCanp", position, angle, Vector3(10, 10, 10));

	DirectXManager::GetInstance()->SetData2D();
	Sprite->SetSize("Messege1", Vector2(1280, 180));
	Sprite->SetSize("Messege2", Vector2(1280, 180));
	Sprite->SetSize("Messege3", Vector2(1280, 180));
	if (MessFlag1 && MessCount1 <= 180 )
	{
		Sprite->Draw("Messege1", Vector3(0, 0, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
	if (MessFlag2 && MessCount2 <= 180)
	{
		Sprite->Draw("Messege2", Vector3(0, 0, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
	if (MessFlag3 && MessCount3 <= 180)
	{
		Sprite->Draw("Messege3", Vector3(0, 0, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
	}
}

void Castle::ImGuiDebug()
{
}

void Castle::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::ENEMYBULLET && !HitFlag )
	{
		HitFlag = true;	
		HP -= col->GetColObject()->GetDamage();
	}
}
