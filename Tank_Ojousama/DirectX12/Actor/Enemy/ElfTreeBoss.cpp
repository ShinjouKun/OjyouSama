#include "ElfTreeBoss.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/LaunchBullet.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Utility/Random.h"
#include "../../Sound/Sound.h"
#include "../../Scene/BaseScene.h"
#include "SummonEnemy.h"
#include "TreeRoot.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"

ElfTreeBoss::ElfTreeBoss(
	const Vector3 & pos,
	const Vector3 & ang,
	shared_ptr<ParticleManager> effectManager,
	int num
)
{
	position = pos;
	angle = ang;
	//mEffectManager = effectManager;
	number = num;
}

ElfTreeBoss::~ElfTreeBoss()
{

}

void ElfTreeBoss::EndCameraAnimation(bool value)
{
	mEndAnimation = value;
}

bool ElfTreeBoss::GetDeadFlag()
{
	return mDeadFlag;
}

void ElfTreeBoss::ChangeAttackState()
{
	//return;

	//シーンアニメーション中 | 死亡アニメーション中は処理しない
	if (!mEndAnimation || mDeathAnimationFlag) return;

	mPlayerPosition = mManager->GetPlayer().GetPosition();

	Vector3 dist = mPlayerPosition - position;
	float length = dist.Length();

	//索敵範囲内にいれば
	if (length < SEARCH_LENGTH)
	{
		//怒り状態の時
		if (mAngryFlag)
		{
			if (!mActionFlag)
			{
				Random::initialize();
				/*0～10で行動を決める*/
				mActionCount = Random::randomRange(0, 13);
				mActionFlag = true;
			}

			//0～5の時
			if (mActionCount >= 0 && mActionCount <= 5)
			{
				//RapidFire();//連続射撃
				ManyAppleDrop();
			}
			//6～9
			else if (mActionCount >= 6 && mActionCount <= 9)
			{
				RootAttack();//根っこ攻撃
			}
			else if (mActionCount >= 10 && mActionCount <= 13)
			{
				Summon();//召喚攻撃
			}
		}
		else
		{
			if (!mActionFlag)
			{
				Random::initialize();
				/*0～10で行動を決める*/
				mActionCount = Random::randomRange(0, 13);
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
			else if (mActionCount >= 10 && mActionCount <= 13)
			{
				Summon();//召喚攻撃
			}
		}
	}
}

void ElfTreeBoss::RapidFire()
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

			mManager->Add(new LaunchBullet(test, mPlayerPosition, mManager, mRend, mPart, objType, mBulletCount++, true));

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

void ElfTreeBoss::Summon()
{
	if (!mActionFlag) return;

	//召喚が呼ばれた時1回しか処理しない

	//敵がすでに存在しているとき
	if (mSummonAlive)
	{
		//行動終了とする
		mActionFlag = false;

		return;
	}

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
			//mManager->Add(new SummonEnemy(mSummonPoint[i], angle, mManager, mRend, mPart, mEnemyNumber++));

			auto enemys = new SummonEnemy(mSummonPoint[i], angle, mManager, mRend, mPart, mEnemyNumber++);
			mSummonList.push_back(enemys);
			mManager->Add(enemys);
		}

		//召喚が終わったらフラグを戻す
		mSummonEnemy = false;

		//行動終了とする
		mActionFlag = false;
	}
}

void ElfTreeBoss::ManyAppleDrop()
{
	mAppleDropTime->update();

	if (!mAppleDropTime->isTime())
	{
		mAppleInterval->update();

		if (mAppleInterval->isTime())
		{
			Random::initialize();
			//描画位置を毎回変える
			float x = Random::randomRange(position.x - 80.0f, position.x + 80.0f);
			float y = position.y + 100.0f;
			float z = Random::randomRange(position.z - 80.0f, position.z + 80.0f);

			Vector3 test = Vector3(x, y - 100.0f, z);

			Vector3 mTargetPosition = mPlayerPosition - test;
			//mTargetPosition = mTargetPosition.normal();
			//float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
			//mFireAngle = -Math::toDegrees(radian) - 180.0f;
			//angle.y = mFireAngle;

			mManager->Add(new LaunchBullet(Vector3(x, y, z), test, mManager, mRend, mPart, objType, mBulletCount++, true));

			mAppleInterval->setTime(0.1f);
		}


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

			mManager->Add(new LaunchBullet(test, mPlayerPosition, mManager, mRend, mPart, objType, mBulletCount++, true));
		}
	}

	//時間になったら
	if (mAppleDropTime->isTime())
	{
		//タイマーをリセットする
		mAppleDropTime->setTime(1.0f);

		//行動終了とする
		mActionFlag = false;
	}
}

void ElfTreeBoss::DeathAnimation()
{
	//仮死状態 でないなら処理しない
	if (!mDeathAnimationFlag) return;

	if (mDeathStep == DeathAnimationStep::EXPLOSION)
	{
		DeathAnimation_Explosion();
	}
	else if (mDeathStep == DeathAnimationStep::DEATH_COUNT)
	{
		DeathAnimation_DeathCount();
	}
}

void ElfTreeBoss::DeathAnimation_Explosion()
{
	mExplosionTime->update();

	//回転
	angle.y += 20.0f;

	if (!mExplosionTime->isTime())
	{

		mEffectInterval->update();

		if (!mOneShotSound)
		{
			/*一度だけ鳴らしたいよ*/
			mSmallExplosion->setPos(position);
			mSmallExplosion->play();
			mOneShotSound = true;
		}


		if (mEffectInterval->isTime())
		{
			//パーティクルを描画
			Random::initialize();
			//描画位置を毎回変える
			float x = Random::randomRange(position.x - 20.0f, position.x + 20.0f);
			float y = Random::randomRange(0.0f, position.y + 30.0f);
			float z = Random::randomRange(position.z - 20.0f, position.z + 20.0f);
			//mParticleEmitter->EmitterUpdate(PARTICLE_EFFECT, Vector3(x, y, z), angle);
			//mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, Vector3(x, y, z), angle);

			mDamageParticle = std::make_shared<Hit>(Vector3(x, y, z), false);

			//mDamageParticle->Stop();
			//mDamageParticle->setPos(Vector3(x, y, z));
			//mDamageParticle->Play();

			//mDeathParticle = std::make_shared<Explosion>(Vector3(x, y, z), false);

			///*できればSEも欲しいねうるさくない程度に*/
			//mSmallExplosion->play();

			mEffectInterval->setTime(0.2f);
		}
	}

	if (mExplosionTime->isTime())
	{

		mExplosionTime->setTime(2.0f);//爆発が終わって消えて、死亡するまでの時間

		///*ここででっかい爆発が起きてほしい*/
		//mBigExplosion->setPos(position);
		//mBigExplosion->play();
		mDeathSE->setPos(position);
		mDeathSE->play();

		for (int i = 0; i < 5; i++)
		{
			//パーティクルを描画
			Random::initialize();
			float x = Random::randomRange(position.x - 5.0f, position.x + 5.0f);
			float y = Random::randomRange(0.0f, position.y + 20.0f);
			float z = Random::randomRange(position.z - 5.0f, position.z + 5.0f);
			//mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, Vector3(x, y, z), angle);

			//mDeathParticle = std::make_shared<Hit>(Vector3(x, y, z), false);

			mDeathParticle = std::make_shared<Explosion>(Vector3(x, y, z), false);

			//mDeathParticle->setPos(Vector3(x, y, z));
			//mDeathParticle->Play();
		}

		mDeathStep = DeathAnimationStep::DEATH_COUNT;
	}
}

void ElfTreeBoss::DeathAnimation_DeathCount()
{
	mExplosionTime->update();

	if (mExplosionTime->isTime())
	{

		mDeadFlag = true;
	}
}

void ElfTreeBoss::RootAttack()
{
	if (!mActionFlag) return;

	//回転
	Vector3 mTargetPosition = mPlayerPosition - position;
	mTargetPosition = mTargetPosition.normal();
	float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
	mFireAngle = -Math::toDegrees(radian) - 180.0f;
	angle.y = mFireAngle;

	switch (mRootStep)
	{
	case ElfTreeBoss::PIERCE_HAND:
		RootAtack_PierceHand();
		break;
	case ElfTreeBoss::CHASE_PLAYER:
		RootAtack_ChasePlayer();
		break;
	case ElfTreeBoss::WAIT:
		RootAtack_Wait();
		break;
	case ElfTreeBoss::GOUP_ROOT:
		RootAtack_GoupRoot();
		break;
	case ElfTreeBoss::GODOWN_ROOT:
		RootAtack_GodownRoot();
		break;
	default:
		break;
	}
}

void ElfTreeBoss::RootAtack_PierceHand()
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

void ElfTreeBoss::RootAtack_ChasePlayer()
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

		if (mAngryFlag)
		{
			mTreeRoot2->SetPosition(mRootPosition);
			mTreeRoot3->SetPosition(mRootPosition);
		}

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

void ElfTreeBoss::RootAtack_Wait()
{
	mRootWaitTime->update();

	//一定時間待つ
	if (mRootWaitTime->isTime())
	{
		mRootWaitTime->setTime(1.0f);
		mRootStep = RootAttackStep::GOUP_ROOT;
	}
}

void ElfTreeBoss::RootAtack_GoupRoot()
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

		Vector3 mRightPos = AngleToVectorY(mFireAngle) * 10.0f;


		mTreeRoot->SetPosition(mRootPosition);
		mTreeRoot->SetAngle(angle);

		if (mAngryFlag)
		{
			mTreeRoot2->SetPosition(mRootPosition + mRightPos);
			mTreeRoot3->SetPosition(mRootPosition - mRightPos);
			mTreeRoot2->SetAngle(angle);
			mTreeRoot3->SetAngle(angle);
		}


		//if (mAngryFlag)
		//{
		//	mTreeRoot2->SetPosition(Vector3(mRootPosition.x + 10, mRootPosition.y, mRootPosition.z));
		//	mTreeRoot3->SetPosition(Vector3(mRootPosition.x - 10, mRootPosition.y, mRootPosition.z));
		//}
	}
}

void ElfTreeBoss::RootAtack_GodownRoot()
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

		if (mAngryFlag)
		{
			mTreeRoot2->SetPosition(Vector3(mRootPosition.x + 10, mRootPosition.y, mRootPosition.z));
			mTreeRoot3->SetPosition(Vector3(mRootPosition.x - 10, mRootPosition.y, mRootPosition.z));
		}
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

void ElfTreeBoss::CreateObject()
{
	if (mCreateObject) return;

	mManager->Add(mTreeRoot);
	mManager->Add(mTreeRoot2);
	mManager->Add(mTreeRoot3);

	mCreateObject = true;
}

void ElfTreeBoss::EnemyInit()
{
	HP = MAX_HP;
	speed = MOVE_SPEED;
	objType = ObjectType::ENEMY;
	mRadius = 3.0f;

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
	mDeadFlag = false;
	mOneShotSound = false;
	mSummonAlive = false;
	mEndAnimation = false;

	mAngryFlag = false;
	mAngryAnimFlag[0] = false;
	mAngryAnimFlag[1] = false;
	mAngryAnimFlag[2] = false;

	//各オブジェクトの初期位置をセット
	mOffsetRightHand = Vector3(position.x, position.y + 15.0f, position.z);
	mOffsetLeftHand = Vector3(position.x, position.y + 15.0f, position.z);
	mOffsetRootPos = Vector3(position.x, position.y - 20.0f, position.z);
	mRightHandPos = mOffsetRightHand;
	mLeftHandPos = mOffsetLeftHand;
	mRootPosition = mOffsetRootPos;
	mScale = Vector3(5.0f, 5.0f, 5.0f);
	//mScale = Vector3(10.0f, 10.0f, 10.0f);

	//根っこオブジェクト生成
	mTreeRoot = new TreeRoot(mRootPosition, angle, mManager, mRend, number);
	mTreeRoot2 = new TreeRoot(mRootPosition, angle, mManager, mRend, number + 1);
	mTreeRoot3 = new TreeRoot(mRootPosition, angle, mManager, mRend, number + 2);
	//召喚した敵リストの初期化
	mSummonList.clear();

	//サウンドの設定
	mSmallExplosion = std::make_shared<Sound>("SE/Long.mp3", true);
	mSmallExplosion->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mBigExplosion = std::make_shared<Sound>("SE/Big_Explosion.mp3", true);
	mBigExplosion->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Boss_Death.wav", true);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mNoDeathSE = std::make_shared<Sound>("SE/Boss_NoDamage.wav", true);
	mNoDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

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
	mAppleDropTime = std::make_shared<Timer>();
	mAppleDropTime->setTime(1.0f);
	mAppleInterval = std::make_shared<Timer>();
	mAppleInterval->setTime(0.1f);

	//根っこ攻撃状態を初期化
	mRootStep = RootAttackStep::PIERCE_HAND;
	//死亡アニメーション状態の初期化
	mDeathStep = DeathAnimationStep::EXPLOSION;

	//mDeathParticle = std::make_shared<Explosion>(Vector3::zero, true);
	//mDeathParticle->Stop();
	mDamageParticle = std::make_shared<Hit>(Vector3::zero, true);
	mDamageParticle->Stop();

	//四角の当たり判定(使い方を教わる)
	//SetCollidder(Vector3(-10.0f, -10.0f, -10.0f), Vector3(10.0f, 50.0f, 10.0f));

	/*当たり判定を増やせないので、顔の部分のみ当たるようにした*/
	//SetCollidder(Vector3(0.0f, 15.0f, 0.0f), mRadius);
	SetCollidder(Vector3(0.0f, 14.0f, 0.0f), mRadius);


#pragma region モデル読み込み + α

	mStringNum = to_string(number);

	//胴体
	mModelNumName01 = mModelName01 + mStringNum;
	mRend->AddModel(mModelNumName01, "Resouse/EnemyModel/TreeBoss/wood_body.obj", "Resouse/EnemyModel/TreeBoss/wood_body.png");
	mRend->SetColor(mModelNumName01, Vector4(0, 0, 0, 0));

	//右手
	mHandRightNum = mHandRight + mStringNum;
	mRend->AddModel(mHandRightNum, "Resouse/EnemyModel/TreeBoss/wood_hands_R.obj", "Resouse/EnemyModel/TreeBoss/wood_hand.png");
	mRend->SetAncPoint(mHandRightNum, Vector3(0.0f, -3.0f, 0.0f));

	//左手
	mHandLeftNum = mHandLeft + mStringNum;
	mRend->AddModel(mHandLeftNum, "Resouse/EnemyModel/TreeBoss/wood_hands_L.obj", "Resouse/EnemyModel/TreeBoss/wood_hand.png");
	mRend->SetAncPoint(mHandLeftNum, Vector3(0.0f, -3.0f, 0.0f));

	//根っこ攻撃用警告
	mRootCircleNum = mRootCircle + mStringNum;
	mRend->AddModel(mRootCircleNum, "Resouse/maru.obj", "Resouse/marui.png");
	mRend->SetColor(mRootCircleNum, Vector4(1, 0, 0, 0.2f));

	//バリア
	mBarrierNum = mBarrier + mStringNum;
	mRend->AddModel(mBarrierNum, "Resouse/EnemyModel/BossBarrier/boss_barrier.obj", "Resouse/EnemyModel/BossBarrier/boss_barrier.png");
	//mRend->SetColor(mBarrierNum, Vector4(1, 1, 1, 0.5f));

	//配列の初期化
	mSummonPoint.resize(SUMMON_COUNT);
	mSummonName.resize(SUMMON_COUNT);
	mSummonNum.resize(SUMMON_COUNT);
	for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
	{
		mSummonPoint[i] = position + AngleToVectorY(360.0f / end * i) * 30.0f;

		//召喚する数分、召喚場所標示用オブジェクトを生成する。
		mSummonName[i] = to_string(i);
		mSummonNum[i] = mSummonCircle + mSummonName[i];
		mRend->AddModel(mSummonNum[i], "Resouse/maru.obj", "Resouse/marui.png");
	}

#pragma endregion
}

void ElfTreeBoss::EnemyUpdate()
{
	if (HP <= 0)
	{
		mDeathAnimationFlag = true;
	}

	//体力が半分以下になったら怒り状態
	if (HP <= MAX_HP / 2)
	{
		mAngryFlag = true;
	}

	if (mAngryFlag == true)
	{
		//if (mAngryAnimFlag[0] && mAngryAnimFlag[1] && mAngryAnimFlag[1]) return;

		mScale += Vector3(0.1f, 0.1f, 0.1f);

		if (mScale.x >= 10.0f)
		{
			mScale = Vector3(10.0f, 10.0f, 10.0f);
			mAngryAnimFlag[0] = true;
		}

		mRadius += 1.0f;

		if (mRadius >= 5.0f)
		{

			mRadius = 5.0f;
			mAngryAnimFlag[1] = true;
		}

		mOffsetRightHand = Vector3(position.x, position.y + 25.0f, position.z);
		mOffsetLeftHand =  Vector3(position.x, position.y + 25.0f, position.z);
		mAngryAnimFlag[2] = true;

		//if (mOffsetRightHand.y >= position.y + 25.0f && mOffsetLeftHand.y >= position.y + 25.0f)
		//{

		//	mOffsetRightHand = Vector3(position.x, position.y + 55.0f, position.z);
		//	mOffsetLeftHand =  Vector3(position.x, position.y + 55.0f, position.z);
		//	mAngryAnimFlag[2] = true;
		//}
	}

	if (mAngryAnimFlag[2])
	{

		int test = 0;


		test = 20;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	//召喚リストが空でないとき
	if (!mSummonList.empty())
	{
		//召喚した敵が存在する
		mSummonAlive = true;

		//リストを更新
		for (int i = 0, end = static_cast<int>(mSummonList.size()); i < end; i++)
		{
			//生存状態を確認
			bool summonDead = mSummonList[i]->GetDeathFlag();

			//死んでいたらリストから削除する
			if (summonDead)
			{
				mSummonList.erase(mSummonList.begin() + i);
				break;
			}
		}
	}
	else
	{
		//召喚した敵が存在しない
		mSummonAlive = false;
	}

	//音量の調整
	mSmallExplosion->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
	mBigExplosion->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);

	/*一度だけオブジェクトを生成*/
	CreateObject();

	/*死亡アニメーション*/
	DeathAnimation();

	/*攻撃*/
	ChangeAttackState();
}

void ElfTreeBoss::EnemyRend()
{
	//爆発状態でない(仮死状態だったら)なら描画しない
	if (mDeathStep != DeathAnimationStep::EXPLOSION) return;

	DirectXManager::GetInstance()->SetData3D();//モデル用をセット

	mRend->Draw(mModelNumName01, position, Vector3(angle.x, angle.y, angle.z), mScale);
	mRend->Draw(mHandRightNum, mRightHandPos, Vector3(mHandAngle, angle.y, angle.z), mScale);
	mRend->Draw(mHandLeftNum, mLeftHandPos, Vector3(mHandAngle, angle.y, angle.z), mScale);

	//根っこがプレイヤーを追跡している時のみ表示
	if (mRootStep == RootAttackStep::CHASE_PLAYER ||
		mRootStep == RootAttackStep::WAIT)
	{
		mRend->Draw(mRootCircleNum, Vector3(mRootPosition.x, position.y - 3, mRootPosition.z), angle, mScale);
	}

	//if (mSummonAlive)
	//{
	//	mRend->Draw(mBarrierNum, Vector3(position.x, position.y, position.z), angle, Vector3(5, 5, 5));
	//}

	//表示状態の時だけ、警告を表示
	if (mDrawSummonPoint)
	{
		for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
		{
			mRend->Draw(mSummonNum[i], mSummonPoint[i], Vector3(0.0f, mSummonRotate += 10.0f, 0.0f), Vector3(2, 2, 2));
		}
	}

	mRend->SetColor(mModelNumName01, Vector4(0, 0, 0, 0));
}

void ElfTreeBoss::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//雑魚敵が召喚されている & 怒り状態の時、ダメージを受けない
		if (mSummonAlive && mAngryFlag)
		{
			//SE発射
			mNoDeathSE->setPos(position);
			mNoDeathSE->play();
		}
		else
		{
			//SE発射
			mDamageSE->setPos(position);
			mDamageSE->play();
			//モデルの色変え
			mRend->SetColor(mModelNumName01, Vector4(1, 0, 0, 1));

			mDamageParticle->setPos(position);
			mDamageParticle->Play();

			//ダメージから防御力を引く
			damagePool = col->GetColObject()->GetDamage() - DEFENSE;

			if (damagePool <= 0)
			{
				damagePool = 1;
			}

			HP -= damagePool;
		}
	}
}

void ElfTreeBoss::EnemyImGuiDebug()
{
}