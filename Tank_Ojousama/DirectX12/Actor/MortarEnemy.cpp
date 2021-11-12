#include "MortarEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../Weapons/LaunchBullet.h"
#include "../Utility/Timer/Timer.h"

MortarEnemy::MortarEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objectManager,
	shared_ptr<ModelRenderer> modelRender, 
	shared_ptr<ParticleManager> effectManager,
	int num
)
{
	position = pos;
	angle = ang;
	mObjManager = objectManager;
	mModelRender = modelRender;
	mEffectManager = effectManager;
	number = num;
}

MortarEnemy::~MortarEnemy()
{
}

void MortarEnemy::Init()
{
	HP = 10;

	mRadius = 1.5f;
	barrelAngle = angle.y;
	turretAngle = 0.0f;

	mScale = Vector3(2.0f, 2.0f, 2.0f);

	death = false;

	objType = ObjectType::ENEMY;

	SetCollidder(new SphereCollider(Vector3().zero, mRadius));

	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);

	mAttackStep = AttackStep::AIMING;

#pragma region ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý
	//íŽÔ‚Ì–Cg(ã‰º‚ÉˆÚ“®‚·‚é•”•ª)Barrel
	tankBarrel = "MemberBarrel";
	num = to_string(number);
	numBarrel = tankBarrel + num;
	mModelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	mModelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//’†S“_‚Ì•ÏX

	//íŽÔ‚Ì–C“ƒ(ã‚Ì•”•ª)Turret
	tankTurret = "MemberTurret";
	num = to_string(number);
	numTurret = tankTurret + num;
	mModelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	mModelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//íŽÔ‚ÌŽÔ‘Ì(‰º‚Ì•”•ª)Body
	tankBody = "MemberBody";
	num = to_string(number);
	numBody = tankBody + num;
	mModelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	mModelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));
#pragma endregion
}

void MortarEnemy::Update()
{

	switch (mAttackStep)
	{
	case MortarEnemy::AIMING:
		AttackStep_AIMING();
		break;
	case MortarEnemy::FIRE:
		AttackStep_FIRE();
		break;
	case MortarEnemy::RELOAD:
		AttackStep_RELOAD();
		break;
	default:
		break;
	}
}

void MortarEnemy::Rend()
{
	//ƒ‚ƒfƒ‹‚Ì•`‰æ
	DirectXManager::GetInstance()->SetData3D();
	mModelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), mScale);
	mModelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), mScale);
	mModelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), mScale);
}

void MortarEnemy::ImGuiDebug()
{
}

void MortarEnemy::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
	}

	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		//ƒJƒƒ‰‚É“–‚½‚Á‚Ä‚¢‚é‚Æ‚«A•`‰æ‚ðs‚¤B
		SetActive(true);
	}
}

void MortarEnemy::AttackStep_AIMING()
{
	mAimingTime->update();

	mTargetPosition = mObjManager->GetPlayer().GetPosition();

	Vector3 distance = mTargetPosition - position;
	distance = distance.normal();

	//“ñ“_ŠÔ‚ÌŠp“x‚ð‹‚ß‚é
	float radian = atan2(distance.x, distance.z);
	//‰ñ“]‚ð”½‰f
	barrelAngle = -Math::toDegrees(radian) - 180.0f;

	if (mAimingTime->isTime())
	{
		mAimingTime->setTime(1.0f);
		mAttackStep = AttackStep::FIRE;
	}
}

void MortarEnemy::AttackStep_FIRE()
{
	Vector3 firePosition = AngleToVectorY(barrelAngle);
	mObjManager->Add(new LaunchBullet(position + firePosition, mTargetPosition, mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++));
	mAttackStep = AttackStep::RELOAD;
}

void MortarEnemy::AttackStep_RELOAD()
{
	//ƒŠƒ[ƒh
	mReloadTime->update();
	if (mReloadTime->isTime())
	{
		mReloadTime->setTime(1.0f);
		mAttackStep = AttackStep::AIMING;
	}
}

Vector3 MortarEnemy::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//Šp“x‚ðƒ‰ƒWƒAƒ“Šp‚É–ß‚·
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}
