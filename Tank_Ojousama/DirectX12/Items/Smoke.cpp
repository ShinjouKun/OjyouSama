#include "Smoke.h"
#include "../Collision/SpherCollider.h"
#include "../Sound/Sound.h"
#include"../Scene/BaseScene.h"
#include"../ParticleSystem/ParticleType/SmokeParticle.h"

Smoke::Smoke(const Vector3 & pos, const Vector3 & ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p, shared_ptr<TexRenderer>s, ItemState itemStates, int num, int addHp)
	:ItemModel(m)
{
	position = pos;
	angle = ang;
	objM = obj;
	objType = ObjectType::ITEM;
	itemName = ItemNames::smock;
	itemState = itemStates;
	smokePoint = addHp;
	num = number;
}

Smoke::~Smoke()
{
}

void Smoke::Init()
{
	death = false;
	isGet = false;
	active = false;
	SetCollidder(Vector3(0, 0, 0), 1.0f);
	alive = 0;
	name = "Smoke";
	num = to_string(number);
	numName = name + num;
	ItemModel->AddModel(numName, "Resouse/smokeModel.obj", "Resouse/smokeModel.png");
	ItemModel->SetAncPoint(numName, Vector3( 0, 2.0f, 0));
	getSE = std::make_shared<Sound>("SE/getItem.mp3", false);
	useSE = std::make_shared<Sound>("SE/smoke.mp3", false);
	mSmokeParticle = std::make_shared<SmokeParticle>(Vector3::zero, true);
	mSmokeParticle->Stop();
}

void Smoke::Update()
{
	angle.y++;
	alive++;
	if (angle.z >= 360 && !isGet)
	{
		angle.z = 0;
	}


	if (ItemHolder::GetInstance()->GetUseSmoke())
	{
		active = true;
		pPos=objM->GetPlayer().GetPosition();
		ItemHolder::GetInstance()->SetUseSmoke(false);
	}

	Smoker();
	getSE->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
	useSE->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);

	FallDown();
}

void Smoke::Rend()
{
	if (!isGet)
	{
		DirectXManager::GetInstance()->SetData3D();//モデル用をセット
		ItemModel->Draw(numName, Vector3(position.x, position.y, position.z), Vector3(angle.x, angle.y, angle.z), Vector3(1.5f, 1.5f, 1.5f));
	}
}

void Smoke::ImGuiDebug()
{
}

void Smoke::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::PLAYER&&!isGet)
	{
		ItemHolder::GetInstance()->AddItem(itemName);
		isGet = true;
		getSE->play();
	}
}

void Smoke::Smoker()
{
	if (!active)
	{
		return;
	}

	mSmokeParticle->setPos(pPos);
	objType = ObjectType::SMOKE;
	useSE->playLoop();
	smokePoint--;

	if (smokePoint <= 0)
	{
		mSmokeParticle->Stop();
		useSE->stop();
		death = true;
	}
	else
	{
		mSmokeParticle->Play();
	}
	
}

void Smoke::FallDown()
{
	if (position.y >= 0.0f)
	{
		position += Vector3(0.0f, -1.0f, 0.0f) * 0.3f;
	}
}
