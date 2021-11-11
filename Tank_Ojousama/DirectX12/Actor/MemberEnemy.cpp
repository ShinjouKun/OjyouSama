#include "MemberEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../Weapons/NormalBullet.h"
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
	mObjManager->Add(new NormalBullet(position + firePosition, Vector3(0.0f, barrelAngle, 0.0f), mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++));
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
	speed = 0.3f;
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
	SetCollidder(new SphereCollider(position, radius));

	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);
	mRandomMoveTimer = std::make_shared<Timer>();
	mRandomMoveTimer->setTime(2.0f);

	mAttackStep = AttackStep::NONE;
	mDissolutionStep = Dissolution::NON;



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
	if (HP <= 0)
	{
		mDeadFlag = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	//ImGui::Checkbox("Dissolution", &mCaptainLost);

	////隊長を失った時
	//if (mCaptainLost)
	//{
	//	mDissolutionStep = Dissolution::DICIDE_DIRECTION;
	//}

	//switch (mDissolutionStep)
	//{
	//case MemberEnemy::NON:
	//	break;
	//case MemberEnemy::DICIDE_DIRECTION:
	//	Random::initialize();

	//	direction = Random::randomRange(0, 3);

	//	switch (direction)
	//	{
	//	case 0://右
	//		mRandomDirection = Vector3(0.1f, 0.0f, 0.0f);
	//		mDissolutionStep = Dissolution::MOVE_DIRECTION;
	//		break;
	//	case 1://左
	//		mRandomDirection = Vector3(-0.1f, 0.0f, 0.0f);
	//		mDissolutionStep = Dissolution::MOVE_DIRECTION;
	//		break;
	//	case 2://前
	//		mRandomDirection = Vector3(0.0f, 0.0f, 0.1f);
	//		mDissolutionStep = Dissolution::MOVE_DIRECTION;
	//		break;
	//	case 3://後
	//		mRandomDirection = Vector3(0.0f, 0.0f, -0.1f);
	//		mDissolutionStep = Dissolution::MOVE_DIRECTION;
	//		break;
	//	default:
	//		break;
	//	}

	//	break;
	//case MemberEnemy::MOVE_DIRECTION:

	//	MoveTarget(position + mRandomDirection, 0.0f);
	//	mRandomMoveTimer->update();

	//	if (mAimingTime->isTime())
	//	{
	//		mRandomMoveTimer->setTime(2.0f);
	//		mDissolutionStep = Dissolution::DICIDE_DIRECTION;
	//	}

	//	break;
	//default:
	//	break;
	//}


	//移動
	MoveTarget(mFixedPosition, 1.0f);

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
	}
}
