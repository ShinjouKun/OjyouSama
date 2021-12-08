#include "BlowEnemy.h"
#include "../../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../../ConstInfomation/Enemy/BlowEnemyConstInfo.h"
#include "../../Sound/Sound.h"
#include"../../Scene/BaseScene.h"

namespace ECI = EnemyConstInfo;
namespace BECI = BlowEnemyConstInfo;

BlowEnemy::BlowEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
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

BlowEnemy::~BlowEnemy()
{
	attackArea->SetDeath(true);
}

void BlowEnemy::EnemyInit()
{
#pragma region 変数の初期化

	HP = ECI::MAX_HP * BECI::MAX_HP;
	warningTime = ECI::WARNING_TIME * BECI::WARNING_TIME * 60;
	attackTime = ECI::ATTACK_TIME * BECI::ATTACK_TIME * 60;

	speed = ECI::MOVE_SPEED * BECI::MOVE_SPEED;
	mRadius = ECI::RADIUS * BECI::RADIUS;
	mSwingRange = ECI::SWING_RANGE * BECI::SWING_RANGE;
	mFireAngle = BECI::FAN_RANGE;
	//turretAngle = 0.0f;
	mAttackLength = ECI::ATTACK_LENGTH * BECI::ATTACK_LENGTH;

	death = false;
	damage = 5;
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	DESTRUCT_MODE = ECI::DESTRUCT_MODE;
	//turnaroundMode = ECI::TURNAROUND_MODE;
	TURNAROUND_MODE = true;
	RECEIVEREPORT_MODE = true;

	scale = BECI::SCALE;

	objType = ObjectType::ENEMY;
	SetCollidder(Vector3().zero, mRadius);

	//センサーの初期化----------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 200.0f;									   //θの角度
	fanInfo.length = 30.0f;										   //長さ
	fanInfo.rotate = mFanRotateOrigin;							   //回転角
	//--------------------------------

	//最初は索敵状態
	mMoveState = MoveState::NOT_FIND;

	//マップのクリア
	breadMap.clear();

	attackArea = new AttackArea(position, angle, mManager, mRend, mOriginNumber);
	attackArea->SetActive(false);

	//巡回ポイントの初期化------------------------------------
	pointCount = sizeof(patrolPoint) / sizeof(patrolPoint[0]);
	patrolPoint[0] = Vector3(position.x, position.y, position.z);
	patrolPoint[1] = Vector3(position.x + 10, position.y, position.z);
	patrolPoint[2] = Vector3(position.x + 10, position.y, position.z + 10);
	patrolPoint[3] = Vector3(position.x, position.y, position.z + 10);
	//----------------------------------------------------------------

	onTrigger = false;

#pragma endregion

#pragma region モデルの読み込み

	////戦車の砲身(上下に移動する部分)Barrel
	//tankBarrel = BECI::TANK_BARREL_NAME;
	//num = to_string(number);
	//numBarrel = tankBarrel + num;
	//modelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	//modelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//中心点の変更

	mMyNumber = to_string(mOriginNumber);

	mRightLeg =  BECI::TANK_RLEG_NAME;
	mLeftLeg = BECI::TANK_LREG_NAME;
	mRLegNumber = mRightLeg + mMyNumber;
	mLLegNumber = mLeftLeg + mMyNumber;
	mRend->AddModel(mRLegNumber,  "Resouse/EnemyModel/Elf_B/leg_R_B.obj", "Resouse/EnemyModel/Elf_B/leg_LR2.png");
	mRend->AddModel(mLLegNumber, "Resouse/EnemyModel/Elf_B/leg_L_B.obj", "Resouse/EnemyModel/Elf_B/leg_LR2.png");
	mRend->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));
	mRend->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	////戦車の砲塔(上の部分)Turret
	//tankTurret = BECI::TANK_TURRET_NAME;
	//num = to_string(number);
	//numTurret = tankTurret + num;
	//modelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	//modelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//戦車の砲塔(上の部分)Turret
	mHead = BECI::TANK_HEAD_NAME;
	mHeadNumber = mHead + mMyNumber;
	mRend->AddModel(mHeadNumber, "Resouse/EnemyModel/Elf_B/elf_head2.obj", "Resouse/EnemyModel/Elf_B/face_color2.png");

	////戦車の車体(下の部分)Body
	//tankBody = BECI::TANK_BODY_NAME;
	//num = to_string(number);
	//numBody = tankBody + num;
	//modelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	//modelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));

	//戦車の車体(下の部分)Body
	mBody = BECI::TANK_BODY_NAME;
	mBodyNumber = mBody + mMyNumber;
	mRend->AddModel(mBodyNumber, "Resouse/EnemyModel/Elf_B/elf_body2.obj", "Resouse/EnemyModel/Elf_B/hand_bow_color2.png");

#pragma endregion
	mSE = std::make_shared<Sound>("punti.mp3", false);
	//mSound->play();
	mSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
}

void BlowEnemy::EnemyUpdate()
{
	/*当たり判定オブジェクトを生成*/
	CreateOneObject();

	///*自爆機能*/
	//DestructMode(BECI::MAX_HP / 2, destructMode);


	///*無敵時間 & 振り向き処理*/
	////Invincible(ECI::REPORT_INTERVAL);//無敵時間
	Invincible(2);

	/*共通の要素*/
	ChangeState(); //状態変更
	SearchObject();//パンくずやプレイヤーを探す
	

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
		Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * mAttackLength;
		attackArea->SetActive(true, position + areaPos, -angle);

		attackCount++;

		//数秒ごとに当たり判定を自分の前方に表示する
		if (attackCount > attackTime)
		{
			attackCount = 0;
			attackArea->SetActive(false);
			mSE->play();
			//攻撃が終わったら警戒状態に戻す
			/*mMoveState = MoveState::WARNING;*/
			mAttackFlag = false;
			mMoveState = MoveState::NOT_FIND;
		}
	}
	else
	{
		TrackingObject();//移動関連
	}
}

void BlowEnemy::EnemyRend()
{
	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);
	mRend->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);
	mRend->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);
	mRend->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);
}

void BlowEnemy::EnemyOnCollision(BaseCollider * col)
{
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

	/*if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}*/

	if (col->GetColObject()->GetType() == ObjectType::ENEMY)
	{
		//自分の番号が相手より小さかったら
		if (col->GetColObject()->GetID() > GetID())
		{
			position = mPreviousPosition;
		}
	}

	if (mAdvanceFlag && col->GetColObject()->GetType() == ObjectType::BORDER_LINE)
	{
		mHitBorderLine = true;
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}

	if (col->GetColObject()->GetType() == ObjectType::DEFENCEPOINT)
	{
		mHitSmokeFlag = true;
	}
}

void BlowEnemy::EnemyImGuiDebug()
{
#ifndef DEBUG

	//ImGui::SliderInt("HP", &HP, 0, 10);
	//ImGui::SliderInt("HP", &destructCount, 0, 1000);
	/*ImGui::Checkbox("Previous", &previousTri);*/

	//float ePos[3] = { fanInfo.position.x,fanInfo.position.y,fanInfo.position.z };
	//float ePos[3] = { angle.x,angle.y,angle.z };
	//ImGui::SliderFloat3("EnemyPosition", ePos, -500.0f, 500.0f);
	//ImGui::SliderFloat("HP", &barrelAngle, -1000.0f, 1000.0f);

#endif // DEBUG
}

void BlowEnemy::Search()
{
	//PatrolPoint(patrolPoint,pointCount);
	//SwingDirection(swingRange);
}

void BlowEnemy::Warning()
{
	//ImGui::Text("ActionState == WARNING");

	//attackArea->SetActive(false);

	//TrackingObject();
}

void BlowEnemy::Attack()
{
	//ImGui::Text("ActionState == ATTACK");

	//Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * attackLength;
	//attackArea->SetActive(true, position + areaPos, -angle);

	//attackCount++;

	////数秒ごとに当たり判定を自分の前方に表示する
	//if (attackCount > attackTime)
	//{
	//	attackCount = 0;
	//	attackArea->SetActive(false);

	//	//攻撃が終わったら警戒状態に戻す
	//	actionState = ActionState::WARNING;
	//}
}

void BlowEnemy::CreateOneObject()
{
	//当たり判定オブジェクトを一度だけ生成
	if (!onTrigger)
	{
		mManager->Add(attackArea);
		attackArea->SetActive(false);
		onTrigger = true;
	}
}
