#include "SniperEnemy.h"
#include "../../Weapons/NormalBullet.h"
#include "../../Weapons/ElfBullet.h"
#include "../../Collision/SpherCollider.h"
#include "../../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../../ConstInfomation/Enemy/SniperEnemyConstInfo.h"
#include "../../Sound/Sound.h"
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
#pragma region 変数の初期化
	//SetActive(false);
	HP = ECI::MAX_HP * SECI::MAX_HP;
	warningTime = ECI::WARNING_TIME * SECI::WARNING_TIME * 60;
	attackTime = ECI::ATTACK_TIME * SECI::ATTACK_TIME * 60;

	speed = ECI::MOVE_SPEED * SECI::MOVE_SPEED;
	mRadius = ECI::RADIUS * SECI::RADIUS;
	mSwingRange = ECI::SWING_RANGE * SECI::SWING_RANGE;
	mFireAngle = SECI::FAN_RANGE;
	//turretAngle = 0.0f;
	mAttackLength = ECI::ATTACK_LENGTH * SECI::ATTACK_LENGTH;

	death = false;
	mTrackingPlayer = false;
	mTrackingBreadcrumb = false;
	RECEIVEREPORT_MODE = true;
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	DESTRUCT_MODE = ECI::DESTRUCT_MODE;
	//turnaroundMode = ECI::TURNAROUND_MODE;
	TURNAROUND_MODE = true;

	scale = SECI::SCALE;

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3().zero, mRadius));

	//センサーの初期化
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 200.0f;									   //θの角度
	fanInfo.length = 60.0f;										   //長さ
	fanInfo.rotate = mFanRotateOrigin;							   //回転角

	//最初は索敵状態
	mMoveState = MoveState::NOT_FIND;

	//マップのクリア
	breadMap.clear();

	SetActive(false);

	mLegRotate = 0.0f;

#pragma endregion

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

	mSE = std::make_shared<Sound>("hirai.mp3", false);
	mSound = std::make_shared<Sound>("down.mp3", false);
	//mSound->play();
	mSound->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
}

void SniperEnemy::EnemyUpdate()
{
	if (HP <= 0)
	{
		mSound->play();
	}
	Invincible(2);//無敵時間

	/*共通の要素*/
	ChangeState(); //状態変更
	SearchObject();//パンくずやプレイヤーを探す

	//if (mMoveState == MoveState::WARNING)
	//{
	//	if (mRotDirection)
	//	{
	//		mLegRotate += LEG_SPEED;
	//		if (mLegRotate > LEG_RANGE)
	//		{
	//			mRotDirection = false;
	//		}
	//	}
	//	else
	//	{
	//		mLegRotate -= LEG_SPEED;
	//		if (mLegRotate < -LEG_RANGE)
	//		{
	//			mRotDirection = true;
	//		}
	//	}
	//}
	//else
	//{
	//	mLegRotate = 0.0f;
	//}


	//if (mMoveState == MoveState::SEARCH)
	//{
	//	//首振り索敵型
	//	SwingDirection(mSwingRange);
	//}
	//else if (mMoveState == MoveState::WARNING)
	//{
	//	//追跡機能
	//	TrackingObject();
	//}
	//else if (mMoveState == MoveState::ATTACK)
	//{
	//	attackCount++;

	//	if (attackCount > attackTime)
	//	{
	//		attackCount = 0;
	//		Vector3 firePos = AngleToVectorY(fanInfo.rotate);

	//		//弾を発射！！
	//		mManager->Add(new ElfBullet(position + firePos, Vector3(0, -angle.y, 0), mManager, mRend, effectManager, objType, bulletNumber));
	//		bulletNumber++;
	//		mMoveState = MoveState::WARNING;
	//	}
	//}

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

	if (mAttackFlag)
	{
		attackCount++;

		if (attackCount > attackTime)
		{
			attackCount = 0;
			Vector3 firePos = AngleToVectorY(fanInfo.rotate);

			//弾を発射！！
			mManager->Add(new ElfBullet(position + firePos, Vector3(0, -angle.y, 0), mManager, mRend, mPart, objType, bulletNumber));
			bulletNumber++;
			mAttackFlag = false;
			mMoveState = MoveState::NOT_FIND;
			mSE->play();
		}
	}
	else
	{
		TrackingObject();//移動関連
	}
}

void SniperEnemy::EnemyRend()
{
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

	//if (col->GetColObject()->GetType() == ObjectType::BREADCRUMB)
	//{
	//	mTrackingBreadcrumb = false;
	//}

	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//ダメージを受ける
		HP -= col->GetColObject()->GetDamage();

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

void SniperEnemy::Search()
{

}

void SniperEnemy::Warning()
{

}

void SniperEnemy::Attack()
{

}
