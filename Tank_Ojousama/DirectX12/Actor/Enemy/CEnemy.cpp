#include "CEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/ElfBullet.h"
#include "../../Utility/Timer/Timer.h"
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
	HP = 10;
	attackTime = 60;

	speed = 0.1f;
	mRadius = 1.0f;
	mSwingRange = 45.0f;
	mFireAngle = angle.y;
	//turretAngle = 0.0f;
	mAttackLength = 10.0f;

	death = false;
	breadcrumbMode = true;
	DESTRUCT_MODE = false;
	TURNAROUND_MODE = false;
	RECEIVEREPORT_MODE = false;

	mOnlyOnceTrigger = false;
	mSearchCommand = false;
	mSearchResult = false;

	scale = Vector3(1.0f, 1.0f, 1.0f);
	mSearchPosition = Vector3().zero;

	objType = ObjectType::ENEMY;
	//SetCollidder(new SphereCollider(position, radius));
	SetCollidder(new SphereCollider(Vector3().zero, mRadius));

	//センサーの初期化----------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 180.0f;									   //θの角度
	fanInfo.length = 30.0f;										   //長さ
	fanInfo.rotate = mFanRotateOrigin;							   //回転角
	//--------------------------------

	mMoveState = MoveState::NOT_FIND;
	searchStep = SearchCommandStep::SEARCH_ORDER;

	mMemberList.resize(MEMBER_COUNT);
	mMemberPositionList.resize(MEMBER_COUNT);
	mEnemyNumber = 0;

	mSearchTimer = std::make_shared<Timer>();
	mSearchTimer->setTime(SEARCH_INTERVAL);

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

	/*共通の要素*/
	ChangeState(); //状態変更
	SearchObject();//パンくずやプレイヤーを探す

	if (HP <= 0)
	{
		for (auto& list : mMemberList)
		{
			list->ReceiveCaptainLost(true);
		}
	}

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


	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		//int debugHP = mMemberList[i]->GetHP();
        //ImGui::SliderInt("HP-------------------", &debugHP, 0, 500);

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
		}
	}
	else
	{
		TrackingObject();//移動関連
	}

	//ImGui::Checkbox("SearchCommand", &mSearchCommand);
	//ImGui::Checkbox("SearchResult", &mSearchResult);

	//float time = mSearchTimer->getCuttentTime();
	//ImGui::SliderFloat("SearchTimer", &time, 0, time);

	////プレイヤーの位置を取得
	//float debugPlayerPosition[3] = { mPlayerPosition.x,mPlayerPosition.y,mPlayerPosition.z };
	//ImGui::SliderFloat3("PlayerPosition----------", debugPlayerPosition, -1000.0f, 1000.0f);
}

void CEnemy::EnemyRend()
{
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
}

void CEnemy::EnemyImGuiDebug()
{
}

void CEnemy::Search()
{
	//SwingDirection(swingRange);
}

void CEnemy::Warning()
{
	//TrackingObject();
}

void CEnemy::Attack()
{
	//attackCount++;

	////数秒ごとに当たり判定を自分の前方に表示する
	//if (attackCount > attackTime)
	//{
	//	attackCount = 0;

	//	//攻撃が終わったら警戒状態に戻す
	//	actionState = ActionState::WARNING;
	//}
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
