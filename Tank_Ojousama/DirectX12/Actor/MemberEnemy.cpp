#include "MemberEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../Weapons/NormalBullet.h"
#include "../Weapons/LaunchBullet.h"
#include "../Utility/Timer/Timer.h"
#include "../Utility/Random.h"

MemberEnemy::MemberEnemy(
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
	mDeadFlag = false;
}

MemberEnemy::~MemberEnemy()
{
}

void MemberEnemy::MoveTarget(const Vector3 & targetPosition, const float distance)
{
	//移動&停止
	Vector3 dist = targetPosition - position;
	float length = dist.Length();
	dist = dist.normal();

	//目標位置との距離が、指定距離以下だったら 処理しない
	if (length < distance) return;

	//二点間の角度を求める
	float radian = atan2(dist.x, dist.z);
	//回転を反映
	angle.y = Math::toDegrees(radian) + 180.0f;
	barrelAngle = -Math::toDegrees(radian) - 180.0f;

	velocity = dist * speed;
	position += velocity;
}

bool MemberEnemy::WithinDistance(const Vector3& targetPosition, const float distance)
{
	//移動&停止
	Vector3 dist = targetPosition - position;
	float length = dist.Length();
	dist = dist.normal();

	//目標位置との距離が、指定距離以下だったら 処理しない
	if (length < distance)
	{
		return false;
	}

	return true;
}

Vector3 MemberEnemy::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//角度をラジアン角に戻す
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}

void MemberEnemy::AttackStep_NONE()
{
	//攻撃指令を受け取った
	if (mAttackCommand)
	{
		mAttackStep = AttackStep::AIMING;
	}
}

void MemberEnemy::AttackStep_AIMING()
{
	mAimingTime->update();
	//砲塔は、プレイヤーの方を向いている
	Vector3 dist = mAttackTarget - position;
	dist = dist.normal();
	float radian = atan2(dist.x, dist.z);
	barrelAngle = -Math::toDegrees(radian) - 180.0f;

	if (mAimingTime->isTime())
	{
		mAimingTime->setTime(1.0f);
		mAttackStep = AttackStep::FIRE;
	}
}

void MemberEnemy::AttackStep_FIRE()
{
	//弾発射
	Vector3 firePosition = AngleToVectorY(barrelAngle);
	//mObjManager->Add(new NormalBullet(position + firePosition, Vector3(0.0f, barrelAngle, 0.0f), mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++));
	mObjManager->Add(new LaunchBullet(position + firePosition, mAttackTarget, mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++));
	mAttackStep = AttackStep::RELOAD;
}

void MemberEnemy::AttackStep_RELOAD()
{
	//リロード
	mReloadTime->update();
	if (mReloadTime->isTime())
	{
		mReloadTime->setTime(1.0f);
		mAttackCommand = false;
		mAttackStep = AttackStep::NONE;
	}
}

bool MemberEnemy::GetDeadFlag() const
{
	return mDeadFlag;
}

void MemberEnemy::ReceivePosition(const Vector3& targetPosition)
{
	mFixedPosition = targetPosition;
}

void MemberEnemy::ReceiveAttackCommand(const Vector3 & attackTarget, const bool attackCommand)
{
	mAttackTarget = attackTarget;
	mAttackCommand = attackCommand;
}

void MemberEnemy::ReceiveSearchCommand(const Vector3 & searchTarget, const bool searchCommand)
{
	mSearchTarget = searchTarget;
	mSearchCommand = searchCommand;
}

void MemberEnemy::ReceiveCaptainLost(const bool captainLost)
{
	mCaptainLost = captainLost;
}

bool MemberEnemy::SendSearchResult() const
{
	return mSearchResult;
}

Vector3 MemberEnemy::SendSearchPosition() const
{
	return mSearchPosition;
}

void MemberEnemy::Init()
{
	HP = 10;
	speed = 0.2f;
	radius = 1.0f;
	barrelAngle = angle.y;
	turretAngle = 0.0f;
	mBulletNumber = 0;

	scale = Vector3().one;
	mFixedPosition = Vector3().zero;
	mAttackTarget = Vector3().zero;
	mSearchTarget = Vector3().zero;

	death = false;
	mDeadFlag = false;
	mAttackCommand = false;
	mSearchCommand = false;
	mSearchResult = false;
	mCaptainLost = false;

	objType = ObjectType::ENEMY;
	//SetCollidder(new SphereCollider(position, radius));
	SetCollidder(new SphereCollider(Vector3().zero, radius));

	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);
	mRandomMoveTimer = std::make_shared<Timer>();
	mRandomMoveTimer->setTime(0.5f);

	mAttackStep = AttackStep::NONE;

	mRandomDirection = Vector3(0.0f, 0.0f, 0.1f);

	testStep = 0;

#pragma region モデルの読み込み
	//戦車の砲身(上下に移動する部分)Barrel
	tankBarrel = "MemberBarrel";
	num = to_string(number);
	numBarrel = tankBarrel + num;
	mModelRender->AddModel(numBarrel, "Resouse/BoxTankATKAR.obj", "Resouse/BoxTankATKAR.png");
	mModelRender->SetAncPoint(numBarrel, Vector3(-2.0f, -2.0f, -2.0f));//中心点の変更

	//戦車の砲塔(上の部分)Turret
	tankTurret = "MemberTurret";
	num = to_string(number);
	numTurret = tankTurret + num;
	mModelRender->AddModel(numTurret, "Resouse/BoxTankATKBR.obj", "Resouse/BoxTankATKBR.png");
	mModelRender->SetAncPoint(numTurret, Vector3(-2.0f, -2.0f, -2.0f));

	//戦車の車体(下の部分)Body
	tankBody = "MemberBody";
	num = to_string(number);
	numBody = tankBody + num;
	mModelRender->AddModel(numBody, "Resouse/BoxTankBTMR.obj", "Resouse/BoxTankBTMR.png");
	mModelRender->SetAncPoint(numBody, Vector3(-2.0f, -2.0f, -2.0f));
#pragma endregion
}

void MemberEnemy::Update()
{
	mPreviousPosition = position - velocity;

	if (HP <= 0)
	{
		mDeadFlag = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	//隊長を失った時
	if (mCaptainLost)
	{
		if (testStep == 0)
		{
			int direction = 0;
			Random::initialize();

			direction = Random::randomRange(0, 3);

			if (direction == 0)
			{
				mRandomDirection = Vector3(0.1f, 0.0f, 0.0f);
				testStep = 1;
			}
			else if (direction == 1)
			{
				mRandomDirection = Vector3(-0.1f, 0.0f, 0.0f);
				testStep = 1;
			}
			else if (direction == 2)
			{
				mRandomDirection = Vector3(0.0f, 0.0f, 0.1f);
				testStep = 1;
			}
			else if (direction == 3)
			{
				mRandomDirection = Vector3(0.0f, 0.0f, -0.1f);
				testStep = 1;
			}
		}
		else if (testStep == 1)
		{
			MoveTarget(position + mRandomDirection, -1.0f);
			mRandomMoveTimer->update();

			if (mRandomMoveTimer->isTime())
			{
				mRandomMoveTimer->setTime(0.5f);
				testStep = 0;
			}
		}
	}
	else
	{
		//移動
		MoveTarget(mFixedPosition, 1.0f);
	}




	//攻撃指令
	switch (mAttackStep)
	{
	case MemberEnemy::NONE:
		AttackStep_NONE();
		break;
	case MemberEnemy::AIMING:
		AttackStep_AIMING();
		break;
	case MemberEnemy::FIRE:
		AttackStep_FIRE();
		break;
	case MemberEnemy::RELOAD:
		AttackStep_RELOAD();
		break;
	default:
		break;
	}

	//索敵指令を受け取った
	if (mSearchCommand)
	{
		//距離計算
		Vector3 distance = mSearchTarget - position;
		float length = distance.Length();

		//距離測定
		if (length < SEARCH_RANGE)
		{
			//隊長に位置とフラグを伝える。
			mSearchResult = true;
			mSearchPosition = mSearchTarget;
		}
		else
		{
			//発見していない
			mSearchResult = false;
		}

		//索敵を終了する
		mSearchCommand = false;
	}
}

void MemberEnemy::Rend()
{
	//表示状態かどうか
	//if(!GetActive()) return;

	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	mModelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	mModelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	mModelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);
}

void MemberEnemy::ImGuiDebug()
{
}

void MemberEnemy::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
		mSearchResult = true;
		mSearchPosition = mSearchTarget;
	}

	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		//カメラに当たっているとき、描画を行う。
		SetActive(true);
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}

	if (col->GetColObject()->GetType() == ObjectType::ENEMY)
	{
		//自分の番号が相手より小さかったら
		if (col->GetColObject()->GetID() > GetID())
		{
			position = mPreviousPosition;
		}
	}
}
