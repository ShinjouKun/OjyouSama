#include "CEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../Utility/Timer/Timer.h"
#include "MemberEnemy.h"

CEnemy::CEnemy(
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

CEnemy::~CEnemy()
{
}

void CEnemy::EnemyInit()
{
	HP = 10;
	attackTime = 60;

	speed = 0.1f;
	radius = 1.0f;
	swingRange = 45.0f;
	barrelAngle = angle.y;
	turretAngle = 0.0f;
	attackLength = 10.0f;

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
	SetCollidder(new SphereCollider(Vector3().zero, radius));

	//センサーの初期化----------------
	fanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//位置
	fanInfo.fanRange = 180.0f;									   //θの角度
	fanInfo.length = 30.0f;										   //長さ
	fanInfo.rotate = fanRotateOrigin;							   //回転角
	//--------------------------------

	actionState = ActionState::SEARCH;
	searchStep = SearchCommandStep::SEARCH_ORDER;

	mMemberList.resize(MEMBER_COUNT);
	mMemberPositionList.resize(MEMBER_COUNT);
	mEnemyNumber = 0;

	mSearchTimer = std::make_shared<Timer>();
	mSearchTimer->setTime(SEARCH_INTERVAL);

#pragma region モデルの読み込み

	num = to_string(number);

	////戦車の砲身(上下に移動する部分)Barrel
	//tankBarrel = "CaptainLegR";
	//numBarrel = tankBarrel + num;
	//mModelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	//mModelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//中心点の変更

	tankBarrel =  "CaptainLegR";
	tankBarrel2 = "CaptainLegL";
	numBarrel = tankBarrel + num;
	numBarrel2 = tankBarrel2 + num;
	mModelRender->AddModel(numBarrel,  "Resouse/EnemyModel/Elf_C/leg_R_C.obj", "Resouse/EnemyModel/Elf_C/leg_LR2.png");
	mModelRender->AddModel(numBarrel2, "Resouse/EnemyModel/Elf_C/leg_L_C.obj", "Resouse/EnemyModel/Elf_C/leg_LR2.png");
	mModelRender->SetAncPoint(numBarrel, Vector3(0.0f, -2.0f, 0.0f));
	mModelRender->SetAncPoint(numBarrel2, Vector3(0.0f, -2.0f, 0.0f));

	////戦車の砲塔(上の部分)Turret
	//tankTurret = "Turret";
	//numTurret = tankTurret + num;
	//mModelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	//mModelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));
	tankTurret = "CaptainHead";
	numTurret = tankTurret + num;
	mModelRender->AddModel(numTurret, "Resouse/EnemyModel/Elf_C/elf_head3.obj", "Resouse/EnemyModel/Elf_C/face_color3.png");

	////戦車の車体(下の部分)Body
	//tankBody = "Body";
	//numBody = tankBody + num;
	//mModelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	//mModelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));
	tankBody = "CaptainBody";
	numBody = tankBody + num;
	mModelRender->AddModel(numBody, "Resouse/EnemyModel/Elf_C/elf_body3.obj", "Resouse/EnemyModel/Elf_C/hand_bow_color3.png");


	mCircle = "Circle";
	mCircleNumber = mCircle + num;
	mModelRender->AddModel(mCircleNumber, "Resouse/maru.obj", "Resouse/marui.png");

#pragma endregion
}

void CEnemy::EnemyUpdate()
{
	/*隊員を生成*/
	OnlyOnceAction();

	if (HP <= 0)
	{
		for (auto& list : mMemberList)
		{
			list->ReceiveCaptainLost(true);
		}
	}

	if (actionState == ActionState::WARNING)
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
				Move(mSearchPosition);
			}
		}
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
	//mModelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	//mModelRender->Draw(numBarrel2, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	//mModelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	//mModelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);

	mModelRender->Draw(numBarrel,  Vector3(position.x, position.y + 2.0f, position.z), Vector3( mLegRotate,barrelAngle, 0), scale);
	mModelRender->Draw(numBarrel2, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, barrelAngle, 0), scale);
	mModelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	mModelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
	//mModelRender->Draw(mCircleNumber, Vector3(position.x, position.y + 5.0f, position.z), Vector3(0, 0, 0), Vector3(10, 10, 10));
}

void CEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//ダメージを受ける
		HP -= col->GetColObject()->GetDamage();
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
	TrackingObject();
}

void CEnemy::Attack()
{
	attackCount++;

	//数秒ごとに当たり判定を自分の前方に表示する
	if (attackCount > attackTime)
	{
		attackCount = 0;

		//攻撃が終わったら警戒状態に戻す
		actionState = ActionState::WARNING;
	}
}

void CEnemy::Destruct()
{
}

void CEnemy::OnlyOnceAction()
{
	//すでに実行していたら 処理しない
	if (mOnlyOnceTrigger) return;

	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		Vector3 pos = position + AngleToVectorY(360.0f / mMemberList.size() * i) * MEMBER_DISTANCE;
		auto enemys = new MemberEnemy(pos, angle, mObjManager, mModelRender, mEffectManager, mEnemyNumber++);
		mMemberList[i] = enemys;
		mObjManager->Add(enemys);
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
