#include "Castle.h"
#include"../Collision/SpherCollider.h"
#include "../Collision/BaseCollider.h"
#include"../Utility/Sequence/Sequence.h"
#include"../Device/Window.h"
#include "../Sound/Sound.h"
#include "../ParticleSystem/ParticleType/Explosion.h"
#include "../Scene/BaseScene.h"

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
	Sprite->AddTexture("HpAicon", "Resouse/castle_aicon.png");
	MaxHp = 150;
	HP = MaxHp;//‚Æ‚è‚ ‚¦‚¸
	
	death = false;
	HitFlag = false;
	HitCount = 0;
	objType = ObjectType::DEFENCEPOINT;
	SetCollidder(Vector3(0, 4.0f, 0.0f), 5.0f);
	MessFlag1 = false;
	MessFlag2 = false;
	MessFlag3 = false;
	MessCount1 = 0;
	MessCount2 = 0;
	MessCount3 = 0;

	mExplosion1 = std::make_shared<Explosion>(Vector3::zero, true);
	mExplosion1->setPos(position);
	mExplosion1->Stop();
	mExplosion2 = std::make_shared<Explosion>(Vector3::zero, true);
	mExplosion2->setPos(Vector3(position.x - 25.f, position.y, position.z - 20.f));
	mExplosion2->Stop();
	mExplosion3 = std::make_shared<Explosion>(Vector3::zero, true);
	mExplosion3->setPos(Vector3(position.x + 25.f, position.y, position.z - 20.f));
	mExplosion3->Stop();
	mExplosion4 = std::make_shared<Explosion>(Vector3::zero, true);
	mExplosion4->setPos(Vector3(position.x - 25.f, position.y, position.z + 20.f));
	mExplosion4->Stop();
	mExplosion5 = std::make_shared<Explosion>(Vector3::zero, true);
	mExplosion5->setPos(Vector3(position.x + 25.f, position.y, position.z + 20.f));
	mExplosion5->Stop();

	mSound = std::make_shared<Sound>("SE/Castle_Damage.wav", false);
	mRange = 1.f;
}

void Castle::Update()
{
	if (HP <= 0)
	{
		mExplosion1->Play();
		mExplosion2->Play();
		mExplosion3->Play();
		mExplosion4->Play();
		mExplosion5->Play();
		position.y -= 1.f;
		position.x += mRange;
		position.z += mRange;
		mRange *= -1.f;
		return;
	}
	
	if (HitFlag)
	{
		HitCount++;
		if (HitCount >= 90)
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
	Sequence::instance().set(HP, Vector2(Window::Window_Width / 2 - 32, 0), Vector2(64, 64));
	Sprite->Draw("HpAicon", Vector3(Window::Window_Width / 2 - 96, 0, 0), 0.0f, Vector2(0, 0), Vector4(1, 1, 1, 1));
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
	if (col->GetColObject()->GetType() == ObjectType::ENEMYBULLET &&!HitFlag )
	{
		HitFlag = true;	
		HP -= col->GetColObject()->GetDamage();
		mSound->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
		mSound->play();
	}
}

void Castle::translate(const Vector3 & pos)
{
	position += pos;
}
