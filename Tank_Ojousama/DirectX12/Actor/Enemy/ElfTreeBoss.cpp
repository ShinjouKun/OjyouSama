#include "ElfTreeBoss.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/LaunchBullet.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Utility/Random.h"
#include "../../Sound/Sound.h"
#include "../../Scene/BaseScene.h"
#include "SummonEnemy.h"
#include "TreeRoot.h"

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

bool ElfTreeBoss::GetDeadFlag()
{
	return mDeadFlag;
}

void ElfTreeBoss::ChangeAttackState()
{
	//死亡アニメーション中は処理しない
	if (mDeathAnimationFlag) return;

	mPlayerPosition = mManager->GetPlayer().GetPosition();

	Vector3 dist = mPlayerPosition - position;
	float length = dist.Length();

	//索敵範囲内にいれば
	if (length < SEARCH_LENGTH)
	{

		if (!mActionFlag)
		{
			Random::initialize();
			/*0〜10で行動を決める*/
			mActionCount = Random::randomRange(0, 13);
			mActionFlag = true;
		}

		//0〜5の時
		if (mActionCount >= 0 && mActionCount <= 5)
		{
			RapidFire();//連続射撃
		}
		//6〜9
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

			mManager->Add(new LaunchBullet(test, mPlayerPosition, mManager, mRend, mPart, objType, mBulletCount++,true));

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

void ElfTreeBoss::DeathAnimation()
{
	//仮死状態 でないなら処理しない
	if(!mDeathAnimationFlag) return;

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
			mSmallExplosion->play();
			mOneShotSound = true;
		}


		if (mEffectInterval->isTime())
		{
			//パーティクルを描画
			Random::initialize();
			//描画位置を毎回変える
			float x = Random::randomRange(position.x - 10.0f, position.x + 10.0f);
			float y = Random::randomRange(0.0f, position.y + 40.0f);
			float z = Random::randomRange(position.z - 10.0f, position.z + 10.0f);
			//mParticleEmitter->EmitterUpdate(PARTICLE_EFFECT, Vector3(x, y, z), angle);
			mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, Vector3(x, y, z), angle);

			///*できればSEも欲しいねうるさくない程度に*/
			//mSmallExplosion->play();

			mEffectInterval->setTime(0.5f);
		}
	}

	if(mExplosionTime->isTime())
	{

		mExplosionTime->setTime(2.0f);//爆発が終わって消えて、死亡するまでの時間

		/*ここででっかい爆発が起きてほしい*/
		mBigExplosion->play();
		mDeathSE->play();

		for (int i = 0; i < 10; i++)
		{
			//パーティクルを描画
			Random::initialize();
			float x = Random::randomRange(position.x - 10.0f, position.x + 10.0f);
			float y = Random::randomRange(0.0f, position.y + 40.0f);
			float z = Random::randomRange(position.z - 10.0f, position.z + 10.0f);
			mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, Vector3(x, y, z), angle);
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

		mTreeRoot->SetPosition(mRootPosition);
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

	//各オブジェクトの初期位置をセット
	mOffsetRightHand = Vector3(position.x, position.y + 15.0f, position.z);
	mOffsetLeftHand = Vector3(position.x, position.y + 15.0f, position.z);
	mOffsetRootPos = Vector3(position.x, position.y - 20.0f, position.z);
	mRightHandPos = mOffsetRightHand;
	mLeftHandPos = mOffsetLeftHand;
	mRootPosition = mOffsetRootPos;

	//根っこオブジェクト生成
	mTreeRoot = new TreeRoot(mRootPosition, angle, mManager, mRend, number);
	//召喚した敵リストの初期化
	mSummonList.clear();

	//サウンドの設定
	mSmallExplosion = std::make_shared<Sound>("SE/Long.mp3", false);
	mSmallExplosion->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mBigExplosion = std::make_shared<Sound>("SE/Big_Explosion.mp3", false);
	mBigExplosion->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", false);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Boss_Death.wav", false);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mNoDeathSE = std::make_shared<Sound>("SE/Boss_NoDamage.wav", false);
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

	//根っこ攻撃状態を初期化
	mRootStep = RootAttackStep::PIERCE_HAND;
	//死亡アニメーション状態の初期化
	mDeathStep = DeathAnimationStep::EXPLOSION;

	//四角の当たり判定(使い方を教わる)
	//SetCollidder(Vector3(-10.0f, -10.0f, -10.0f), Vector3(10.0f, 3.0f, 10.0f));
	//SetCollidder(new AABBCollider(position, Vector3(10.0f, 10.0f, 10.0f)));

	/*当たり判定を増やせないので、顔の部分のみ当たるようにした*/
	SetCollidder(Vector3(0.0f, 15.0f, 0.0f), mRadius);

	///*memo : 四角形の当たり判定が使えないので、円の当たり判定を増やしました*/
	//for (int i = 0; i < 10; i++)
	//{
	//	SetCollidder(Vector3(0.0f, mRadius * i, 0.0f), mRadius);
	//}


#pragma region モデル読み込み + α

	mStringNum = to_string(number);

	//胴体
	mModelNumName01 = mModelName01 + mStringNum;
	mRend->AddModel(mModelNumName01, "Resouse/EnemyModel/TreeBoss/wood_body.obj", "Resouse/EnemyModel/TreeBoss/wood_body.png");

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

	//パーティクル1
	mParticleEmitter = make_shared<ParticleEmitterBox>(mPart);
	mParticleEmitter->LoadAndSet(PARTICLE_EFFECT, "Resouse/effect.png");
	mParticleEmitter->LoadAndSet(EXPLOSION_EFFECT, "Resouse/Bom.jpg");

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
	mRend->Draw(mModelNumName01, position, Vector3(angle.x, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));
	mRend->Draw(mHandRightNum, mRightHandPos, Vector3(mHandAngle, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));
	mRend->Draw(mHandLeftNum, mLeftHandPos, Vector3(mHandAngle, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));

	//根っこがプレイヤーを追跡している時のみ表示
	if (mRootStep == RootAttackStep::CHASE_PLAYER ||
		mRootStep == RootAttackStep::WAIT)
	{
		mRend->Draw(mRootCircleNum, Vector3(mRootPosition.x, position.y - 3, mRootPosition.z), angle, Vector3(5, 5, 5));
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
}

void ElfTreeBoss::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//雑魚敵が召喚されている時、ダメージを受けない
		if (mSummonAlive)
		{
			//SE発射
			mNoDeathSE->play();
		}
		else
		{
			//SE発射
			mDamageSE->play();
			HP -= col->GetColObject()->GetDamage();
		}
	}
}

void ElfTreeBoss::EnemyImGuiDebug()
{
}