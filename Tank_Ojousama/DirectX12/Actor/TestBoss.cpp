#include "TestBoss.h"
#include "../Collision/SpherCollider.h"
#include "../Collision/AABBCollider.h"
#include "../Weapons/NormalBullet.h"
#include "../Weapons/LaunchBullet.h"
#include "../Utility/Timer/Timer.h"


#include "SummonEnemy.h"


TestBoss::TestBoss(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<ParticleManager> effectManager,
	int num
)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	mModelRender = modelRender;
	mEffectManager = effectManager;
	number = num;
}

TestBoss::~TestBoss()
{
}

void TestBoss::Init()
{
	HP = MAX_HP;
	speed = MOVE_SPEED;
	objType = ObjectType::ENEMY;

	damage = 5;
	mBulletCount = 0;
	mEnemyNumber = 0;
	mCircleCount = 0;

	death = false;
	mShotNormalBullet = false;
	mSummonEnemy = false;
	mDrawSummonPoint = false;

	mSummonFlag = true;

	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(AIMING_TIME);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(RELOAD_TIME);
	mSummonTime = std::make_shared<Timer>();
	mSummonTime->setTime(SUMMON_TIME);

	//四角の当たり判定(使い方を教わる)
	//SetCollidder(new AABBCollider(Vector3().zero, Vector3(10.0f, 10.0f, 10.0f)));
	SetCollidder(Vector3().zero, 2.0f);

	mModelNum = to_string(number);
	mModelNumName01 = mModelName01 + mModelNum;
	mModelRender->AddModel(mModelNumName01, "Resouse/wood.obj", "Resouse/Big-treeA.png");


	//配列の初期化
	mSummonPoint.resize(SUMMON_COUNT);
	testNum.resize(SUMMON_COUNT);
	testNumName.resize(SUMMON_COUNT);
	for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
	{
		mSummonPoint[i] = position + AngleToVectorY(360.0f / end * i) * 30.0f;

		testNum[i] = to_string(i);
		testNumName[i] = mCircleName + testNum[i];
		mModelRender->AddModel(testNumName[i], "Resouse/maru.obj", "Resouse/marui.png");
	}

	////落下地点用オブジェクト
	//mCircleNum
	//numCircle = mCircleName + num;
	//mModelRender->AddModel(numCircle, "Resouse/maru.obj", "Resouse/marui.png");
}

void TestBoss::Update()
{
	//この敵は、プレイヤーとの距離で描画を分ける必要がある
	//RapidFire();//連続射撃

	Summon();


	ImGui::Checkbox("-------------------", &mDrawSummonPoint);
}

void TestBoss::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(mModelNumName01, position, angle, Vector3(5.0f, 5.0f, 5.0f));

	//表示状態の時だけ、警告を表示
	if (mDrawSummonPoint)
	{
		for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
		{
			mModelRender->Draw(testNumName[i], mSummonPoint[i], angle, Vector3(2,2,2));
		}
	}
}

void TestBoss::ImGuiDebug()
{
}

void TestBoss::OnCollison(BaseCollider * col)
{
}

void TestBoss::RapidFire()
{
	//狙い中
	if (!mShotNormalBullet)
	{
		mAimingTime->update();

		if (mAimingTime->isTime())
		{
			mShotNormalBullet = true;
			mAimingTime->setTime(AIMING_TIME);
		}
	}

	//発射
	if (mShotNormalBullet)
	{
		mReloadTime->update();

		if (mReloadTime->isTime())
		{
			mReloadTime->setTime(RELOAD_TIME);

			Vector3 test = position + FIRE_POSITION;

			Vector3 mTargetPosition = mObjManager->GetPlayer().GetPosition() - test;
			mTargetPosition = mTargetPosition.normal();
			float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
			mFireAngle = -Math::toDegrees(radian) - 180.0f;

			Vector3 firePosition = AngleToVectorY(mFireAngle) + test;

			float t = Math::atan2(mTargetPosition.y, mTargetPosition.z);
			float tt = -Math::toDegrees(t);

			mObjManager->Add(new NormalBullet(test, Vector3(tt, mFireAngle, 0.0f), mObjManager, mModelRender, mEffectManager, objType, mBulletCount++));

			//指定数連続で射撃する
			if (mBulletCount >= RAPIDFIRE_COUNT)
			{
				mBulletCount = 0;
				mShotNormalBullet = false;
			}
		}
	}
}

void TestBoss::Summon()
{
	if (!mSummonFlag) return;

	//召喚が呼ばれた時1回しか処理しない

	//力を溜めている状態
	if (!mSummonEnemy)
	{
		mSummonTime->update();

		//警告を表示する
		mDrawSummonPoint = true;

		if (mSummonTime->isTime())
		{
			mSummonTime->setTime(SUMMON_TIME);
			mSummonEnemy = true;
		}
	}

	//召喚開始！
	if (mSummonEnemy)
	{
		//描画終了
		mDrawSummonPoint = false;

		//最初に用意した位置に、敵を生成
		for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
		{
			mObjManager->Add(new SummonEnemy(mSummonPoint[i], angle, mObjManager, mModelRender, mEffectManager, mEnemyNumber++));
		}

		//召喚が終わったらフラグを戻す
		mSummonEnemy = false;
		mSummonFlag = false;
	}
}

Vector3 TestBoss::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//角度をラジアン角に戻す
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}
