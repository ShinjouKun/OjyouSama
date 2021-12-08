#include "Castle.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"


Castle::Castle(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> modelRender, shared_ptr<ParticleManager> effect)
	:Model(modelRender),Particle(effect)
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
	HP = 200;//‚Æ‚è‚ ‚¦‚¸
	death = false;
	HitFlag = false;
	HitCount = 0;
	objType = ObjectType::DEFENCEPOINT;
	SetCollidder(Vector3(0, 0, 0), 10.0f);
}

void Castle::Update()
{
	if (HitFlag)
	{
		HitCount++;
		if (HitCount >= 30)
		{
			HitCount = 0;
			HitFlag = false;
		}
	}
}

void Castle::Rend()
{
	DirectXManager::GetInstance()->SetData3D();
	Model->Draw("DefCanp", position, angle, Vector3(10, 10, 10));
}

void Castle::ImGuiDebug()
{
}

void Castle::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::ENEMYBULLET)
	{
		HP -= col->GetColObject()->GetDamage();
		HitFlag = true;
	}
}
