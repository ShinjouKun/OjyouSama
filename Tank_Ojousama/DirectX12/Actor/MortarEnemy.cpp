#include "MortarEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../Weapons/LaunchBullet.h"
#include "../Utility/Timer/Timer.h"
#include "../Utility/Random.h"

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
	damage = 5;

	mRadius = 1.5f;
	barrelAngle = angle.y;
	mHandAngle = 0.0f;

	mScale = Vector3(2.0f, 2.0f, 2.0f);

	death = false;
	mFireFlag = false;

	objType = ObjectType::ENEMY;

	SetCollidder(new SphereCollider(Vector3().zero, mRadius));

	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);

	mAttackStep = AttackStep::AIMING;

#pragma region モデルの読み込み

	num = to_string(number);

	//右手
	mHandRight = "MemberHandRight";
	mHandRNum = mHandRight + num;
	mModelRender->AddModel(mHandRNum, "Resouse/gorem_hands_R.obj", "Resouse/gorem.png");

	//左手
	mHandLeft = "MemberHandLeft";
	mHandLNum = mHandLeft + num;
	mModelRender->AddModel(mHandLNum, "Resouse/gorem_hands_L.obj", "Resouse/gorem.png");

	mModelRender->SetAncPoint(mHandRNum, Vector3(0.0f, -3.0f, 0.0f));
	mModelRender->SetAncPoint(mHandLNum, Vector3(0.0f, -3.0f, 0.0f));

	//体
	mBody = "MemberBody";
	mBodyNum = mBody + num;
	mModelRender->AddModel(mBodyNum, "Resouse/gorem_body.obj", "Resouse/gorem.png");
#pragma endregion
}

void MortarEnemy::Update()
{
	if (HP <= 0)
	{
		death = true;
	}

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
	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	mModelRender->Draw(mHandRNum, Vector3(position.x, position.y + 7.0f, position.z), Vector3(mHandAngle, barrelAngle, 0), mScale);
	mModelRender->Draw(mHandLNum, Vector3(position.x, position.y + 7.0f, position.z), Vector3(mHandAngle, barrelAngle, 0), mScale);
	mModelRender->Draw(mBodyNum, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), mScale);
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
		//カメラに当たっているとき、描画を行う。
		SetActive(true);
	}
}

void MortarEnemy::AttackStep_AIMING()
{
	mAimingTime->update();

	mTargetPosition = mObjManager->GetPlayer().GetPosition();

	Vector3 distance = mTargetPosition - position;
	distance = distance.normal();

	//二点間の角度を求める
	float radian = atan2(distance.x, distance.z);
	//回転を反映
	barrelAngle = -Math::toDegrees(radian) - 180.0f;

	if (mAimingTime->isTime())
	{
		mAimingTime->setTime(1.0f);
		mAttackStep = AttackStep::FIRE;
	}
}

void MortarEnemy::AttackStep_FIRE()
{
	if (!mFireFlag)
	{
		Vector3 firePosition = AngleToVectorY(barrelAngle);
		mObjManager->Add(new LaunchBullet(position + firePosition, mTargetPosition, mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++));
		mFireFlag = true;
	}

	//腕を振り上げる
	mHandAngle -= 10.0f;

	//振り上げ終わったら
	if (mFireFlag && mHandAngle <= -90.0f)
	{
		mAttackStep = AttackStep::RELOAD;
	}
}

void MortarEnemy::AttackStep_RELOAD()
{
	//リロード
	mReloadTime->update();

	//腕を元に戻す
	mHandAngle += 3.0f;

	if (mHandAngle >= 0.0f)
	{
		mHandAngle = 0.0f;
	}

	//リロード完了 && 腕が元に戻りきっていたら
	if (mReloadTime->isTime() && mHandAngle >= 0.0f)
	{
		//ここの値がランダムじゃないと、全部の攻撃位置が重なっちゃう

		Random::initialize();

		float time = Random::randomRange(0.5f, 1.5f);

		mReloadTime->setTime(time);
		mFireFlag = false;
		mAttackStep = AttackStep::AIMING;
	}
}

Vector3 MortarEnemy::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//角度をラジアン角に戻す
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}
