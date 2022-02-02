#include "ElfBullet.h"
#include"../Collision/BaseCollider.h"

ElfBullet::ElfBullet(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	std::shared_ptr<ModelRenderer> modelRender,
	shared_ptr<ParticleManager> particleManager, 
	ObjectType objectType,
	int num
)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	mModelRender = modelRender;
	mParticleManager = particleManager;
	setType = objectType;
	number = num;
}

ElfBullet::~ElfBullet()
{
}

void ElfBullet::Init()
{
	SetBulletType();

	death = false;

	damage = 10;
	alive = 0;
	speed = 1.2f;
	//コライダーの情報をセット
	SetCollidder(Vector3().zero, 0.5f);

	//mObjManager->SetReloadTime(20);

#pragma region モデルの読み込み

	name = "ElfBullet";
	num = to_string(number);
	numName = name + num;
	mModelRender->AddModel(numName, "Resouse/EnemyModel/Arrow/arrow.obj", "Resouse/EnemyModel/Arrow/arrow.png");
	mModelRender->SetAncPoint(numName, Vector3(0, -2.0f, 0));
	mParticleEmitter = make_shared<ParticleEmitterBox>(mParticleManager);
	mParticleEmitter->LoadAndSet("Bom", "Resouse/Bom.jpg");

#pragma endregion
}

void ElfBullet::Update()
{
	velocity = Vector3(0, 0, -1);
	velocity *= Matrix4::RotateX(angle.x);
	velocity *= Matrix4::RotateY(angle.y);
	position += velocity * speed;

	rot += mRotValue;

	alive++;
	if (alive >= ALIVE_COUNT)
	{
		death = true;
	}
}

void ElfBullet::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(numName, Vector3(position.x, position.y + 2.0f, position.z), Vector3(angle.x, angle.y, rot), Vector3(1, 1, 1));
}

void ElfBullet::ImGuiDebug()
{
}

void ElfBullet::OnCollison(BaseCollider * col)
{
	//if (objType == BULLET &&
	//	(col->GetColObject()->GetType() == ObjectType::ENEMY || 
	//	 col->GetColObject()->GetType() == ObjectType::BOSS || 
	//	 col->GetColObject()->GetType() == ObjectType::ENEMYBULLET || 
	//	 col->GetColObject()->GetType() == ObjectType::BLOCK)
	//	)
	//{
	//	mParticleEmitter->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
	//	death = true;
	//}

	//if (objType == ENEMYBULLET && 
	//	(col->GetColObject()->GetType() == ObjectType::PLAYER ||
	//	 col->GetColObject()->GetType() == ObjectType::BULLET ||
	//	 col->GetColObject()->GetType() == ObjectType::BLOCK || 
	//	 col->GetColObject()->GetType() == ObjectType::ITEM)
	//	)
	//{
	//	mParticleEmitter->EmitterUpdate("Bom", Vector3(position.x, position.y, position.z), angle);
	//	death = true;
	//}

	if (objType == ENEMYBULLET &&
		(col->GetColObject()->GetType() == ObjectType::PLAYER)
		)
	{
		//mParticleEmitter->EmitterUpdate(EXPLOSION, position, angle);
		death = true;
	}

	if (objType == ENEMYBULLET &&
		(col->GetColObject()->GetType() == ObjectType::BULLET ||
			col->GetColObject()->GetType() == ObjectType::BLOCK ||
			col->GetColObject()->GetType() == ObjectType::ITEM)
		)
	{

		//mParticleEmitter->EmitterUpdate(PARTICLE_NAME, position, angle);
		death = true;
	}
}
