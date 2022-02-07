#include "CEnemy.h"
#include "../../Weapons/ElfBullet.h"
#include"../../Scene/BaseScene.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"
#include "MemberEnemy.h"

CEnemy::CEnemy(
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

CEnemy::~CEnemy()
{
}

void CEnemy::EnemyInit()
{
	HP = 30;//もともと10
	attackTime = 60;
	mEnemyNumber = 0;
	damage = 0;
	speed = 0.35f;
	mRadius = 1.0f;
	mSwingRange = 45.0f;
	mAttackLength = 50.0f;
	mFireAngle = angle.y;

	death = false;
	breadcrumbMode = true;
	DESTRUCT_MODE = false;
	TURNAROUND_MODE = false;
	RECEIVEREPORT_MODE = false;
	mOnlyOnceTrigger = false;
	mSearchCommand = false;
	mSearchResult = false;
	mRotDirection = false;
	mDeathAnimation = false;
	mDeadFlag = false;

	mMemberList.resize(MEMBER_COUNT);
	mMemberPositionList.resize(MEMBER_COUNT);

	scale = Vector3(1.0f, 1.0f, 1.0f);
	mSearchPosition = Vector3().zero;
	SetCollidder(Vector3().zero, mRadius);

	objType = ObjectType::ENEMY;
	mMoveState = MoveState::NOT_FIND;
	mDeathStep = DeathAnimationStep::RISE_SKY;
	searchStep = SearchCommandStep::SEARCH_ORDER;

	//センサーの初期化----------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 360.0f;									   //θの角度
	fanInfo.length = 50.0f;										   //長さ
	fanInfo.rotate = mFanRotateOrigin;							   //回転角
	//--------------------------------

	//サウンド初期化
	mAttackSE = std::make_shared<Sound>("SE/hirai.mp3", true);

	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);

	mDeathSE = std::make_shared<Sound>("SE/Elf_Damage01.mp3", true);


	//タイマー初期化
	mSearchTimer = std::make_shared<Timer>();
	mSearchTimer->setTime(SEARCH_INTERVAL);
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
	mRightLeg =  "CaptainLegR";
	mRLegNumber = mRightLeg + mMyNumber;
	mRend->AddModel(mRLegNumber,  "Resouse/EnemyModel/Elf_C/leg_R_C.obj", "Resouse/EnemyModel/Elf_C/leg_LR2.png");
	mRend->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//左足情報登録
	mLeftLeg = "CaptainLegL";
	mLLegNumber = mLeftLeg + mMyNumber;
	mRend->AddModel(mLLegNumber, "Resouse/EnemyModel/Elf_C/leg_L_C.obj", "Resouse/EnemyModel/Elf_C/leg_LR2.png");
	mRend->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//頭情報登録
	mHead = "CaptainHead";
	mHeadNumber = mHead + mMyNumber;
	mRend->AddModel(mHeadNumber, "Resouse/EnemyModel/Elf_C/elf_head3.obj", "Resouse/EnemyModel/Elf_C/face_color3.png");

	//体情報登録
	mBody = "CaptainBody";
	mBodyNumber = mBody + mMyNumber;
	mRend->AddModel(mBodyNumber, "Resouse/EnemyModel/Elf_C/elf_body3.obj", "Resouse/EnemyModel/Elf_C/hand_bow_color3.png");

#pragma endregion
}

void CEnemy::EnemyUpdate()
{
	/*隊員を生成*/
	OnlyOnceAction();

	Invincible(1);//無敵時間

	/*生存状態を監視*/
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

	///*移動指令*/
	//MoveOrder();

	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		mAnyDeathFlag = false;

		//生存状態を監視
		bool deadFlag = mMemberList[i]->GetDeadFlag();

		if (deadFlag)
		{
			mMemberList.erase(mMemberList.begin() + i);

			mAnyDeathFlag = true;
		}

		if (mAnyDeathFlag) break;

		/*移動管理*/
		MemberMove(i);
	}

	///*索敵指令*/
	//SearchOrder();

	//索敵範囲内にプレイヤーがいるか
	if (InsideDistance(mPlayerPosition, MEMBER_DISTANCE))
	{
		for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
		{
			mMemberList[i]->ReceiveAttackCommand(mPlayerPosition, true);
		}

		mSearchCommand = false;
		mSearchResult = false;
	}
	else//隊長の索敵範囲内にプレイヤーがいないとき
	{
		//索敵指令が出ていない時
		if (!mSearchCommand)
		{
			//2秒に1回索敵指令を出す
			mSearchTimer->update();

			if (mSearchTimer->isTime())
			{
				//索敵指令を出す
				for (const auto& list : mMemberList)
				{
					list->ReceiveSearchCommand(mPlayerPosition, true);
				}

				mSearchTimer->setTime(SEARCH_INTERVAL);
				mSearchCommand = true;
			}
		}

		//索敵指令が出ている
		if (mSearchCommand)
		{
			for (const auto& list : mMemberList)
			{
				//索敵結果を受け取る
				mSearchResult = list->SendSearchResult();
				//対象を発見していたら
				if (mSearchResult)
				{
					//対象の位置を一度だけ取得
					mSearchPosition = list->SendSearchPosition();
					break;
				}
			}

			//全員の報告を受け取る or 誰か1人が対象を発見していたら報告終了
			mSearchCommand = false;
		}

		//対象を発見していたら
		if (mSearchResult)
		{
			//近くなったら元に戻す
			if (InsideDistance(mSearchPosition, 1.0f))
			{
				mSearchResult = false;
			}
			else
			{
				//対象の位置まで移動
				MovePoint(mSearchPosition);
			}
		}
	}
}

void CEnemy::EnemyRend()
{
	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);//右脚
	mRend->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);//左脚
	mRend->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//頭と手
	mRend->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//体と弓	
}

void CEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//SE発射
		mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
		mDamageSE->setPos(position);
		mDamageSE->play();

		//パーティクル発射
		mDamageParticle->setPos(Vector3(position.x, position.y + 5.0f, position.z));
		mDamageParticle->Play();

		//ダメージを受ける
		HP -= col->GetColObject()->GetDamage();
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
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

void CEnemy::EnemyImGuiDebug()
{
}

void CEnemy::OnlyOnceAction()
{
	//すでに実行していたら 処理しない
	if (mOnlyOnceTrigger) return;

	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		Vector3 pos = position + AngleToVectorY(360.0f / mMemberList.size() * i) * MEMBER_DISTANCE;
		auto enemys = new MemberEnemy(pos, angle, mManager, mRend, mPart, mEnemyNumber++);
		mMemberList[i] = enemys;
		mManager->Add(enemys);
	}

	//処理が終わったらフラグを建てる
	mOnlyOnceTrigger = true;
}

void CEnemy::MemberMove(int number)
{
	//離れた位置にポジションを指定
	mMemberPositionList[number] = position + AngleToVectorY(360.0f / mMemberList.size() * number) * MEMBER_DISTANCE;

	//隊員に位置情報を渡す。
	mMemberList[number]->ReceivePosition(mMemberPositionList[number]);
}

void CEnemy::MoveOrder()
{
	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		mAnyDeathFlag = false;

		//生存状態を監視
		bool deadFlag = mMemberList[i]->GetDeadFlag();

		if (deadFlag)
		{
			mMemberList.erase(mMemberList.begin() + i);

			mAnyDeathFlag = true;
		}

		if (mAnyDeathFlag) break;

		/*移動管理*/
		MemberMove(i);
	}
}

void CEnemy::SearchOrder()
{
	//索敵範囲内にプレイヤーがいるか
	if (InsideDistance(mPlayerPosition, MEMBER_DISTANCE))
	{
		for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
		{
			mMemberList[i]->ReceiveAttackCommand(mPlayerPosition, true);
		}

		mSearchCommand = false;
		mSearchResult = false;
	}
	else//隊長の索敵範囲内にプレイヤーがいないとき
	{
		//索敵指令が出ていない時
		if (!mSearchCommand)
		{
			//2秒に1回索敵指令を出す
			mSearchTimer->update();

			if (mSearchTimer->isTime())
			{
				//索敵指令を出す
				for (const auto& list : mMemberList)
				{
					list->ReceiveSearchCommand(mPlayerPosition, true);
				}

				mSearchTimer->setTime(SEARCH_INTERVAL);
				mSearchCommand = true;
			}
		}

		//索敵指令が出ている
		if (mSearchCommand)
		{
			for (const auto& list : mMemberList)
			{
				//索敵結果を受け取る
				mSearchResult = list->SendSearchResult();
				//対象を発見していたら
				if (mSearchResult)
				{
					//対象の位置を一度だけ取得
					mSearchPosition = list->SendSearchPosition();
					break;
				}
			}

			//全員の報告を受け取る or 誰か1人が対象を発見していたら報告終了
			mSearchCommand = false;
		}

		//対象を発見していたら
		if (mSearchResult)
		{
			//近くなったら元に戻す
			if (InsideDistance(mSearchPosition, 1.0f))
			{
				mSearchResult = false;
			}
			else
			{
				//対象の位置まで移動
				MovePoint(mSearchPosition);
			}
		}
	}
}

void CEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		mDeathAnimation = true;
		for (auto& list : mMemberList)
		{
			list->ReceiveCaptainLost(true);
		}
	}

	if (mDeadFlag)
	{
		death = true;
	}

	/*死亡アニメーションを開始*/
	DeathAnimation();
}

void CEnemy::Move()
{
	//攻撃中は移動しない
	if (mAttackFlag) return;

	/*移動 & 追跡*/
	TrackingObject();

	/*移動のアニメーション*/
	MoveAnimation();
}

void CEnemy::MoveAnimation()
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

void CEnemy::Attack()
{
	if (!mAttackFlag) return;

	attackCount++;

	if (attackCount > attackTime)
	{
		attackCount = 0;
		Vector3 firePos = AngleToVectorY(fanInfo.rotate);

		Vector3 pos = Vector3(position.x, position.y + 1.5f, position.z);

		//弾を発射！！
		mManager->Add(new ElfBullet(pos + firePos, Vector3(0, -angle.y, 0), mManager, mRend, mPart, objType, bulletNumber));
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

void CEnemy::DeathAnimation()
{
	//仮死状態でない　なら処理しない
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case CEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case CEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void CEnemy::DeathAnimeStep_RiseSky()
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

		//SE発射
		mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
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

void CEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}
