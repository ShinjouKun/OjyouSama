#include "BlowEnemy.h"
#include "../../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../../ConstInfomation/Enemy/BlowEnemyConstInfo.h"
#include"../../Scene/BaseScene.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"

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

	//testActive = false;
}

BlowEnemy::~BlowEnemy()
{
	if (attackArea != nullptr)
	{
		attackArea->SetDeath(true);
	}
	
}

void BlowEnemy::EnemyInit()
{
#pragma region 変数の初期化

	HP = 30;//もともと20
	warningTime = ECI::WARNING_TIME * BECI::WARNING_TIME * 60;
	attackTime = 30;//もともと60

	speed = 0.3f;
	mRadius = ECI::RADIUS * BECI::RADIUS;
	mSwingRange = ECI::SWING_RANGE * BECI::SWING_RANGE;
	mFireAngle = BECI::FAN_RANGE;
	mAttackLength = ECI::ATTACK_LENGTH * BECI::ATTACK_LENGTH;

	damage = 5;
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	DESTRUCT_MODE = ECI::DESTRUCT_MODE;
	death = false;
	TURNAROUND_MODE = true;
	RECEIVEREPORT_MODE = true;
	mRotDirection = false;
	onTrigger = false;
	mDeathAnimation = false;
	mDeadFlag = false;
	mDeathAnimation = false;
	mDeadFlag = false;

	scale = BECI::SCALE;
	mLegRotate = 0.0f;
	SetCollidder(Vector3().zero, mRadius);

	objType = ObjectType::ENEMY;
	mMoveState = MoveState::NOT_FIND;
	mDeathStep = DeathAnimationStep::RISE_SKY;

	//センサーの初期化----------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 200.0f;									   //θの角度
	fanInfo.length = 50.0f;										   //長さ
	fanInfo.rotate = mFanRotateOrigin;							   //回転角
	//--------------------------------

	////巡回ポイントの初期化------------------------------------
	//pointCount = sizeof(patrolPoint) / sizeof(patrolPoint[0]);
	//patrolPoint[0] = Vector3(position.x, position.y, position.z);
	//patrolPoint[1] = Vector3(position.x + 10, position.y, position.z);
	//patrolPoint[2] = Vector3(position.x + 10, position.y, position.z + 10);
	//patrolPoint[3] = Vector3(position.x, position.y, position.z + 10);
	////----------------------------------------------------------------
	/// 
	////マップのクリア
	//breadMap.clear();

	//当たり判定オブジェクト生成
	attackArea = new AttackArea(position, angle, mManager, mRend, mOriginNumber);
	attackArea->SetActive(false);

	//サウンド初期化
	mAttackSE = std::make_shared<Sound>("SE/punti.mp3", true);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Elf_Damage02.mp3", true);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//タイマー初期化
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

#pragma endregion

#pragma region モデルの読み込み

	//番号をintからstringに変換
	mMyNumber = to_string(mOriginNumber);

	//右足情報登録
	mRightLeg =  BECI::TANK_RLEG_NAME;
	mRLegNumber = mRightLeg + mMyNumber;
	mRend->AddModel(mRLegNumber, "Resouse/EnemyModel/SpearElf/yari_elf_R.obj", "Resouse/EnemyModel/SpearElf/leg_LR2.png");
	mRend->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//左足情報登録
	mLeftLeg = BECI::TANK_LREG_NAME;
	mLLegNumber = mLeftLeg + mMyNumber;
	mRend->AddModel(mLLegNumber, "Resouse/EnemyModel/SpearElf/yari_elf_L.obj", "Resouse/EnemyModel/SpearElf/leg_LR2.png");
	mRend->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//顔情報登録
	mHead = BECI::TANK_HEAD_NAME;
	mHeadNumber = mHead + mMyNumber;
	mRend->AddModel(mHeadNumber, "Resouse/EnemyModel/SpearElf/yari_elf_head.obj", "Resouse/EnemyModel/SpearElf/face_color3.png");

	//体情報登録
	mBody = BECI::TANK_BODY_NAME;
	mBodyNumber = mBody + mMyNumber;
	mRend->AddModel(mBodyNumber, "Resouse/EnemyModel/SpearElf/yari_elf_body.obj", "Resouse/EnemyModel/SpearElf/face_color.png");

	//手情報登録
	mHand = "BlowHnad";
	mHandNumber = mHand + mMyNumber;
	mRend->AddModel(mHandNumber, "Resouse/EnemyModel/SpearElf/yari_te.obj", "Resouse/EnemyModel/SpearElf/yari_elf.png");

#pragma endregion
}

void BlowEnemy::EnemyUpdate()
{
	//if (!testActive) return;


	/*当たり判定オブジェクトを生成*/
	CreateOneObject();

	Invincible(1);//無敵時間

	/*死亡状態監視*/
	CheckAlive();

	//仮死状態なら処理しない
	if (mDeathAnimation) return;

	ChangeState(); //状態変更
	SearchObject();//パンくずやプレイヤーを探す

	/*移動*/
	Move();

	/*攻撃*/
	Attack();
}

void BlowEnemy::EnemyRend()
{

	//if (!testActive) return;

	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);
	mRend->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);
	mRend->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);
	mRend->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);
	mRend->Draw(mHandNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);
}

void BlowEnemy::EnemyOnCollision(BaseCollider * col)
{
	//if (!testActive) return;

	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//ダメージを受ける
		HP -= col->GetColObject()->GetDamage();

		//SE発射
		mDamageSE->setPos(position);
		mDamageSE->play();

		//パーティクル発射
		mDamageParticle->setPos(Vector3(position.x, position.y + 5.0f, position.z));
		mDamageParticle->Play();

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

void BlowEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		mDeathAnimation = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	/*死亡アニメーションを開始*/
	DeathAnimation();
}

void BlowEnemy::Move()
{
	//攻撃中は移動しない
	if (mAttackFlag) return;

	/*移動 & 追跡*/
	TrackingObject();

	/*移動のアニメーション*/
	MoveAnimation();
}

void BlowEnemy::MoveAnimation()
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

void BlowEnemy::Attack()
{
	if (mAttackFlag)
	{
		Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * mAttackLength;
		attackArea->SetActive(true, position + areaPos, -angle);
		mAttackSE->setPos(position);
		mAttackSE->play();

		attackCount++;

		//数秒ごとに当たり判定を自分の前方に表示する
		if (attackCount > attackTime)
		{
			attackCount = 0;
			attackArea->SetActive(false);
			//攻撃が終わったら警戒状態に戻す
			mAttackFlag = false;
			mMoveState = MoveState::NOT_FIND;
		}
	}
}

void BlowEnemy::DeathAnimation()
{
	//仮死状態でない　なら処理しない
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case BlowEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case BlowEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void BlowEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//SE発射
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

		//SE発射
		mDamageSE->setPos(position);
		mDamageSE->play();

		//パーティクル発射
		mDamageParticle->setPos(position);
		mDamageParticle->Play();

		////パーティクル発射
		//mDeathParticle->setPos(position);
		//mDeathParticle->Play();

		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void BlowEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
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
