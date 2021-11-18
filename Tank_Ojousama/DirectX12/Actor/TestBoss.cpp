#include "TestBoss.h"
#include "../Collision/SpherCollider.h"
#include "../Collision/AABBCollider.h"
#include "../Weapons/NormalBullet.h"
#include "../Weapons/LaunchBullet.h"
#include "../Utility/Timer/Timer.h"
#include "../Utility/Random.h"
#include "SummonEnemy.h"
#include "TreeRoot.h"

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
	//delete mTreeRoot;
}

void TestBoss::Init()
{
	HP = MAX_HP;
	speed = MOVE_SPEED;
	objType = ObjectType::BLOCK;

	damage = 5;
	mBulletCount = 0;
	mEnemyNumber = 0;
	mCircleCount = 0;
	mActionCount = 0;

	mRootSpeed = 1.5f;
	mSummonRotate = 0.0f;
	mHandAngle = 0.0f;

	death = false;
	mShotBullet = false;
	mSummonEnemy = false;
	mDrawSummonPoint = false;
	mCreateObject = false;
	mFinishHandReturn = false;
	mFinishRootReturn = false;

	mActionFlag = false;
	mDeathAnimationFlag = false;

	//各オブジェクトの初期位置をセット
	mOffsetRightHand = Vector3(position.x /*- 10.0f*/, position.y + 15.0f, position.z);
	mOffsetLeftHand = Vector3(position.x  /*+ 10.0f*/, position.y + 15.0f, position.z);
	mOffsetRootPos = Vector3(position.x, position.y - 20.0f, position.z);
	mRightHandPos = mOffsetRightHand;
	mLeftHandPos = mOffsetLeftHand;
	mRootPosition = mOffsetRootPos;

	//根っこオブジェクト生成
	mTreeRoot = new TreeRoot(mRootPosition, angle, mObjManager, mModelRender, number);

	//各種タイマー初期化
	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(AIMING_TIME);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(RELOAD_TIME);
	mSummonTime = std::make_shared<Timer>();
	mSummonTime->setTime(SUMMON_TIME);
	mRootWaitTime = std::make_shared<Timer>();
	mRootWaitTime->setTime(0.5f);
	mEffectInterval = std::make_shared<Timer>();
	mEffectInterval->setTime(0.5f);
	mExplosionTime = std::make_shared<Timer>();
	mExplosionTime->setTime(3.0f);

	//根っこ攻撃状態を初期化
	mRootStep = RootAttackStep::PIERCE_HAND;
	//死亡アニメーション状態の初期化
	mDeathStep = DeathAnimationStep::EXPLOSION;

	//四角の当たり判定(使い方を教わる)
	SetCollidder(Vector3(-10.0f, -10.0f, -10.0f), Vector3(10.0f, 3.0f, 10.0f));
	//SetCollidder(new AABBCollider(position, Vector3(10.0f, 10.0f, 10.0f)));
	//SetCollidder(Vector3().zero, 2.0f);


#pragma region モデル読み込み + α

	mStringNum = to_string(number);

	//胴体
	mModelNumName01 = mModelName01 + mStringNum;
	mModelRender->AddModel(mModelNumName01, "Resouse/EnemyModel/TreeBoss/wood_body.obj", "Resouse/EnemyModel/TreeBoss/wood_body.png");

	//右手
	mHandRightNum = mHandRight + mStringNum;
	mModelRender->AddModel(mHandRightNum, "Resouse/EnemyModel/TreeBoss/wood_hands_R.obj", "Resouse/EnemyModel/TreeBoss/wood_hand.png");
	mModelRender->SetAncPoint(mHandRightNum, Vector3(0.0f, -3.0f, 0.0f));

	//左手
	mHandLeftNum = mHandLeft + mStringNum;
	mModelRender->AddModel(mHandLeftNum, "Resouse/EnemyModel/TreeBoss/wood_hands_L.obj", "Resouse/EnemyModel/TreeBoss/wood_hand.png");
	mModelRender->SetAncPoint(mHandLeftNum, Vector3(0.0f, -3.0f, 0.0f));

	//根っこ攻撃用警告
	mRootCircleNum = mRootCircle + mStringNum;
	mModelRender->AddModel(mRootCircleNum, "Resouse/maru.obj", "Resouse/marui.png");

	//パーティクル
	mParticleEmitter = make_shared<ParticleEmitterBox>(mEffectManager);
	mParticleEmitter->LoadAndSet(PARTICLE_EFFECT, "Resouse/effect.png");

	//配列の初期化
	mSummonPoint.resize(SUMMON_COUNT);
	testNum.resize(SUMMON_COUNT);
	testNumName.resize(SUMMON_COUNT);
	for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
	{
		mSummonPoint[i] = position + AngleToVectorY(360.0f / end * i) * 30.0f;

		//召喚する数分、召喚場所標示用オブジェクトを生成する。
		testNum[i] = to_string(i);
		testNumName[i] = mSummonCircle + testNum[i];
		mModelRender->AddModel(testNumName[i], "Resouse/maru.obj", "Resouse/marui.png");
	}

#pragma endregion

}

void TestBoss::Update()
{
	if (HP <= 0)
	{
		mDeathAnimationFlag = true;
	}

	//プレイヤーの位置を取得！
	mPlayerPosition = mObjManager->GetPlayer().GetPosition();

	/*一度だけオブジェクトを生成*/
	CreateObject();

	/*死亡アニメーションと死亡処理*/
	DeathAnimation();

	/*攻撃*/
	ChangeAttackState();
}

void TestBoss::Rend()
{
	//爆発状態でない(仮死状態だったら)なら描画しない
	if (mDeathStep != DeathAnimationStep::EXPLOSION) return;

	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(mModelNumName01, position, Vector3(angle.x, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));
	mModelRender->Draw(mHandRightNum, mRightHandPos, Vector3(mHandAngle, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));
	mModelRender->Draw(mHandLeftNum, mLeftHandPos, Vector3(mHandAngle, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));

	//根っこがプレイヤーを追跡している時のみ表示
	if (mRootStep == RootAttackStep::CHASE_PLAYER ||
		mRootStep == RootAttackStep::WAIT)
	{
		mModelRender->Draw(mRootCircleNum, Vector3(mRootPosition.x, position.y - 3, mRootPosition.z), angle, Vector3(5, 5, 5));
	}

	//表示状態の時だけ、警告を表示
	if (mDrawSummonPoint)
	{
		for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
		{
			mModelRender->Draw(testNumName[i], mSummonPoint[i], Vector3(0.0f, mSummonRotate += 10.0f, 0.0f), Vector3(2, 2, 2));
		}
	}
}

void TestBoss::ImGuiDebug()
{
}

void TestBoss::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
	}

}

void TestBoss::ChangeAttackState()
{
	if (mDeathAnimationFlag) return;

	Vector3 dist = mPlayerPosition - position;
	float length = dist.Length();

	//索敵範囲内にいれば
	if (length < SEARCH_LENGTH)
	{
		if (!mActionFlag)
		{
			Random::initialize();
			/*0～10で行動を決める*/
			mActionCount = Random::randomRange(0, 10);
			mActionFlag = true;
		}

		//0～5の時
		if (mActionCount >= 0 && mActionCount <= 5)
		{
			RapidFire();//連続射撃
		}
		//6～9
		else if (mActionCount >= 6 && mActionCount <= 9)
		{
			RootAttack();//根っこ攻撃
		}
		else if (mActionCount >= 10 && mActionCount <= 11)
		{
			Summon();//召喚攻撃
		}
	}
}

void TestBoss::RapidFire()
{
	if (!mActionFlag) return;

	//狙い中
	if (!mShotBullet)
	{
		mAimingTime->update();

		//できれば、狙い中もプレイヤーの方をみていてほしい
		Vector3 mTargetPosition = mPlayerPosition - position;
		mTargetPosition = mTargetPosition.normal();
		float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
		mFireAngle = -Math::toDegrees(radian) - 180.0f;
		angle.y = mFireAngle;

		if (mAimingTime->isTime())
		{
			mShotBullet = true;
			mAimingTime->setTime(AIMING_TIME);
		}
	}

	//発射
	if (mShotBullet)
	{
		mReloadTime->update();

		if (mReloadTime->isTime())
		{
			mReloadTime->setTime(RELOAD_TIME);

			Vector3 test = position + FIRE_POSITION;

			Vector3 mTargetPosition = mPlayerPosition - test;
			mTargetPosition = mTargetPosition.normal();
			float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
			mFireAngle = -Math::toDegrees(radian) - 180.0f;
			angle.y = mFireAngle;

			//mObjManager->Add(new NormalBullet(test, Vector3(tt, mFireAngle, 0.0f), mObjManager, mModelRender, mEffectManager, objType, mBulletCount++));

			mObjManager->Add(new LaunchBullet(position, mPlayerPosition, mObjManager, mModelRender, mEffectManager, objType, mBulletCount++,true));
			//mPlayerPosition

			//指定数連続で射撃する
			if (mBulletCount >= RAPIDFIRE_COUNT)
			{
				mBulletCount = 0;
				mShotBullet = false;

				//行動終了とする
				mActionFlag = false;
			}
		}
	}
}

void TestBoss::Summon()
{
	if (!mActionFlag) return;

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

		//行動終了とする
		mActionFlag = false;
	}
}

void TestBoss::DeathAnimation()
{
	//仮死状態 でないなら処理しない
	if(!mDeathAnimationFlag) return;

	switch (mDeathStep)
	{
	case TestBoss::EXPLOSION:

		mExplosionTime->update();

		//回転
		angle.y += 20.0f;

		if (!mExplosionTime->isTime())
		{
			mEffectInterval->update();

			if (mEffectInterval->isTime())
			{
				//パーティクルを描画
				Random::initialize();
				//描画位置を毎回変える
				float x = Random::randomRange(-10.0f, 10.0f);
				float y = Random::randomRange(0.0f, 30.0f);
				float z = Random::randomRange(-10.0f, 10.0f);
				mParticleEmitter->EmitterUpdate(PARTICLE_EFFECT, Vector3(x, y, z), angle);

				/*できればSEも欲しいねうるさくない程度に*/

				mEffectInterval->setTime(0.1f);
			}
		}
		else
		{
			mExplosionTime->setTime(2.0f);//爆発が終わって消えて、死亡するまでの時間

			/*ここででっかい爆発が起きてほしい*/

			mDeathStep = DeathAnimationStep::DEATH_COUNT;
		}

		break;
	case TestBoss::DEATH_COUNT:

		mExplosionTime->update();

		if (mExplosionTime->isTime())
		{
			death = true;
		}

		break;
	default:
		break;
	}
}

void TestBoss::RootAttack()
{
	if (!mActionFlag) return;

	switch (mRootStep)
	{
	case TestBoss::PIERCE_HAND:
		RootAtack_PierceHand();
		break;
	case TestBoss::CHASE_PLAYER:
		RootAtack_ChasePlayer();
		break;
	case TestBoss::WAIT:
		RootAtack_Wait();
		break;
	case TestBoss::GOUP_ROOT:
		RootAtack_GoupRoot();
		break;
	case TestBoss::GODOWN_ROOT:
		RootAtack_GodownRoot();
		break;
	default:
		break;
	}
}

void TestBoss::RootAtack_PierceHand()
{
	mHandAngle = 90.0f;

	//地面に手をぶっさす
	mRightHandPos.y--;
	mLeftHandPos.y--;

	if (mRightHandPos.y <= 8.0f && mLeftHandPos.y <= 8.0f)
	{
		mRootStep = RootAttackStep::CHASE_PLAYER;
	}
}

void TestBoss::RootAtack_ChasePlayer()
{
	//警告がプレイヤーを追いかける

	//Yの移動を省いて計算する
	Vector3 holdTarget = Vector3(mPlayerPosition.x, 0.0f, mPlayerPosition.z);
	Vector3 holdPosition = Vector3(mRootPosition.x, 0.0f, mRootPosition.z);
	Vector3 dist = holdTarget - holdPosition;
	float length = dist.Length();
	dist = dist.normal();

	//索敵範囲内にいれば
	if (length < SEARCH_LENGTH)
	{
		velocity = dist * mRootSpeed;
		mRootPosition += velocity;

		mTreeRoot->SetPosition(mRootPosition);

		//根っこの当たる範囲内にいれば
		if (length < 1.0f)
		{

			mRootStep = RootAttackStep::WAIT;
		}
	}
	else
	{
		//いなかったら、その場に根っこを出す
		mRootStep = RootAttackStep::GOUP_ROOT;
	}
}

void TestBoss::RootAtack_Wait()
{
	mRootWaitTime->update();

	//一定時間待つ
	if (mRootWaitTime->isTime())
	{
		mRootWaitTime->setTime(1.0f);
		mRootStep = RootAttackStep::GOUP_ROOT;
	}
}

void TestBoss::RootAtack_GoupRoot()
{
	if (mRootPosition.y >= 0.0f)
	{
		mRootWaitTime->update();

		//一定時間待つ
		if (mRootWaitTime->isTime())
		{
			mRootWaitTime->setTime(0.5f);
			mRootStep = RootAttackStep::GODOWN_ROOT;
		}
	}
	else
	{
		//根っこを上に上げる。
		mRootPosition.y += 2.0f;

		mTreeRoot->SetPosition(mRootPosition);
	}
}

void TestBoss::RootAtack_GodownRoot()
{
	//右手と左手を元に戻す
	if (mRightHandPos.y >= mOffsetRightHand.y && mLeftHandPos.y >= mOffsetLeftHand.y)
	{
		mRightHandPos = mOffsetRightHand;
		mLeftHandPos = mOffsetLeftHand;

		mFinishHandReturn = true;
	}
	else
	{
		//手を元の位置に戻す
		mRightHandPos.y++;
		mLeftHandPos.y++;
	}

	//根っこを元に戻す
	if (mRootPosition.y <= mOffsetRootPos.y)
	{
		mRootPosition = mOffsetRootPos;
		mFinishRootReturn = true;
	}
	else
	{
		//根っこを下におろす
		mRootPosition.y -= 0.5f;
		mTreeRoot->SetPosition(mRootPosition);
	}

	//両腕と根っこが元の位置に戻ったら
	if (mFinishHandReturn && mFinishRootReturn)
	{
		mHandAngle = 0.0f;
		mFinishHandReturn = mFinishRootReturn = false;
		//行動終了とする
		mActionFlag = false;
		mRootStep = RootAttackStep::PIERCE_HAND;
	}
}

void TestBoss::CreateObject()
{
	if (mCreateObject) return;

	mObjManager->Add(mTreeRoot);

	mCreateObject = true;
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
