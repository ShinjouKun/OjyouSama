#include "SniperEnemy.h"
#include "../../Weapons/ElfBullet.h"
#include "../../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../../ConstInfomation/Enemy/SniperEnemyConstInfo.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"
#include"../../Scene/BaseScene.h"

namespace ECI = EnemyConstInfo;
namespace SECI = SniperEnemyConstInfo;

SniperEnemy::SniperEnemy(
	const Vector3& pos,
	const Vector3& ang,
	int num,
	bool advanceFlag,
	const Vector3& advanceDirection
)
{
	position = pos;
	angle = ang;
	mOriginNumber = num;
	mAdvanceFlag = advanceFlag;
	mAdvanceDirection = advanceDirection;
}

SniperEnemy::~SniperEnemy()
{
}

void SniperEnemy::EnemyInit()
{
	HP = 30;//もともとは10
	warningTime = ECI::WARNING_TIME * SECI::WARNING_TIME * 60;
	attackTime = 1 * 60;//もともと 1 * 60だったのを半分にした
	damage = 0;
	speed = 0.3f;
	mRadius = ECI::RADIUS * SECI::RADIUS;
	mSwingRange = ECI::SWING_RANGE * SECI::SWING_RANGE;
	mFireAngle = SECI::FAN_RANGE;
	mAttackLength = 60.0f;//もともとは30

	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	DESTRUCT_MODE = ECI::DESTRUCT_MODE;
	death = false;
	mTrackingPlayer = false;
	mTrackingBreadcrumb = false;
	RECEIVEREPORT_MODE = true;
	TURNAROUND_MODE = true;
	mDeathAnimation = false;
	mDeadFlag = false;

	mLegRotate = 0.0f;

	SetActive(false);
	scale = SECI::SCALE;

	mMoveState = MoveState::NOT_FIND;
	objType = ObjectType::ENEMY;
	mDeathStep = DeathAnimationStep::RISE_SKY;
	SetCollidder(Vector3(0.0f,2.0f,0.0f), mRadius);

	//センサーの初期化-------------------------------------------------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 200.0f;									   //θの角度
	fanInfo.length = 80.0f;										   //長さ
	fanInfo.rotate = mFanRotateOrigin;							   //回転角
	//-----------------------------------------------------------------------

	////マップのクリア
	//breadMap.clear();

	//サウンドの初期化
	mAttackSE = std::make_shared<Sound>("SE/hirai.mp3", true);

	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);

	mDeathSE = std::make_shared<Sound>("SE/Elf_Damage01.mp3", true);


	//タイマーの初期化
	mRiseTime = std::make_shared<Timer>();
	mRiseTime->setTime(0.5f);
	mDeathTime = std::make_shared<Timer>();
	mDeathTime->setTime(0.5f);

	//ダメージ用パーティクル
	mDamageParticle = std::make_shared<Hit>(Vector3::zero, true);
	mDamageParticle->Stop();

	////死亡用エフェクト
	//mDeathParticle = std::make_shared<Explosion>(Vector3::zero, true);
	//mDeathParticle->Stop();

#pragma region モデルの読み込み

	//番号をintからstringに変換
	mMyNumber = to_string(mOriginNumber);

	//右足情報登録
	mRightLeg = SECI::TANK_RLEG_NAME;
	mRLegNumber = mRightLeg + mMyNumber;
	mRend->AddModel(mRLegNumber, "Resouse/EnemyModel/Elf_A/leg_R.obj", "Resouse/EnemyModel/Elf_A/leg_LR.png");
	mRend->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//左足情報登録
	mLeftLeg = SECI::TANK_LREG_NAME;
	mLLegNumber = mLeftLeg + mMyNumber;
	mRend->AddModel(mLLegNumber, "Resouse/EnemyModel/Elf_A/leg_L.obj", "Resouse/EnemyModel/Elf_A/leg_LR.png");
	mRend->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//頭情報登録
	mHead = SECI::TANK_HEAD_NAME;
	mHeadNumber = mHead + mMyNumber;
	mRend->AddModel(mHeadNumber, "Resouse/EnemyModel/Elf_A/elf_head.obj", "Resouse/EnemyModel/Elf_A/face_color.png");

	//体情報登録
	mBody = SECI::TANK_BODY_NAME;
	mBodyNumber = mBody + mMyNumber;
	mRend->AddModel(mBodyNumber, "Resouse/EnemyModel/Elf_A/elf_body.obj", "Resouse/EnemyModel/Elf_A/hand_bow_color.png");

#pragma endregion
}

void SniperEnemy::EnemyUpdate()
{
	Invincible(1);//無敵時間

	/*死亡状態監視*/
	CheckAlive();

	//仮死状態なら処理しない
	if (mDeathAnimation) return;

	/*共通の要素*/
	ChangeState(); //状態変更
	SearchObject();//パンくずやプレイヤーを探す

	/*移動*/
	Move();

	/*攻撃*/
	Attack();
}

void SniperEnemy::EnemyRend()
{
	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);//右脚
	mRend->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);//左脚
	mRend->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//頭と手
	mRend->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//体と弓	
}

void SniperEnemy::EnemyOnCollision(BaseCollider * col)
{
	/*if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}*/

	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		int test = col->GetColObject()->GetDamage();

		//ダメージを受ける
		HP -= col->GetColObject()->GetDamage();

		//SE発射
		mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
		mDamageSE->setPos(position);
		mDamageSE->play();

		//パーティクル発射
		mDamageParticle->setPos(Vector3(position.x, position.y + 5.0f, position.z));
		mDamageParticle->Play();

		/*報告*/
		InitSearch();
	}

	if (mAdvanceFlag && col->GetColObject()->GetType() == ObjectType::BORDER_LINE)
	{
		mHitBorderLine = true;
	}

	if (col->GetColObject()->GetType() == ObjectType::DEFENCEPOINT)
	{
		mHitSmokeFlag = true;
	}
}

void SniperEnemy::EnemyImGuiDebug()
{

}

void SniperEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		//アニメーション(仮死状態)にする
		mDeathAnimation = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	/*死亡アニメーションを開始*/
	DeathAnimation();
}

void SniperEnemy::Move()
{
	//攻撃中は移動しない
	if (mAttackFlag) return;

	/*移動 & 追跡*/
	TrackingObject();

	/*移動のアニメーション*/
	MoveAnimation();
}

void SniperEnemy::MoveAnimation()
{
	if (moveFlag)
	{
		if (mRotDirection)
		{
			mLegRotate += LEG_SPEED;
			if (mLegRotate > LEG_RANGE)
			{
				mRotDirection = false;
			}
		}
		else
		{
			mLegRotate -= LEG_SPEED;
			if (mLegRotate < -LEG_RANGE)
			{
				mRotDirection = true;
			}
		}
	}
	else
	{
		mLegRotate = 0.0f;
	}
}

void SniperEnemy::Attack()
{
	if (!mAttackFlag) return;
	attackCount++;

	if (attackCount > attackTime)
	{
		attackCount = 0;
		Vector3 firePos = AngleToVectorY(fanInfo.rotate);

		Vector3 pos = Vector3(position.x, position.y + 1.5f, position.z);

		//弾を発射！！
		mManager->Add(new ElfBullet(pos + firePos, Vector3(0.0, -angle.y, 0.0f), mManager, mRend, mPart, objType, bulletNumber));
		
		bulletNumber++;
		mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
		mAttackSE->setPos(position);
		mAttackSE->play();
		mAttackFlag = false;
		mMoveState = MoveState::NOT_FIND;
	}
	else
	{
		////プレイヤーを向く。
		//FacingPlayer();
	}

}

void SniperEnemy::DeathAnimation()
{
	//仮死状態でない　なら処理しない
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case SniperEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case SniperEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void SniperEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//SE発射
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE->setPos(position);
	mDeathSE->play();

	//時間になっていなければ
	if (!mRiseTime->isTime())
	{
		//回転
		mFireAngle += 50.0f;
		//上昇
		position.y += 0.8f;
	}
	else
	{
		//時間になったら(1フレームだけ呼ばれる)

		////パーティクル発射
		//mDeathParticle->setPos(position);
		//mDeathParticle->Play();

		//パーティクル発射
		mDamageParticle->setPos(position);
		mDamageParticle->Play();

		//SE発射
		mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
		mDamageSE->setPos(position);
		mDamageSE->play();

		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void SniperEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}
