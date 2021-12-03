#include "BirdEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Weapons/LaunchBullet.h"

BirdEnemy::BirdEnemy(
	const Vector3 & pos, 
	const Vector3 & ang,
	int num
)
{
	position = pos;
	angle = ang;
	number = num;
}

BirdEnemy::~BirdEnemy()
{
}

//void BirdEnemy::Init()
//{
//	HP = 100;
//	damage = 5;
//
//	speed = 0.8f;
//	mRadius = 2.0f;
//	mFireAngle = 0.0f;
//
//	mIntervalTime = std::make_shared<Timer>();
//	mIntervalTime->setTime(1.0f);
//	mReloadTime = std::make_shared<Timer>();
//	mReloadTime->setTime(1.0f);
//
//	mScale = Vector3(2.0f, 2.0f, 2.0f);
//	mOffsetPosition = position;//帰還位置を保存
//	//↓帰還位置の真上を保存
//	mOffsetRisePosition = Vector3(mOffsetPosition.x, mOffsetPosition.y + RISE_HEIGHT, mOffsetPosition.z);
//
//	death = false;
//	mFireFlag = false;
//
//	objType = ObjectType::ENEMY;
//
//	SetCollidder(Vector3(0.0f, 5.0f, 0.0f), mRadius);
//
//	mActionStep = ActionStep::RISING;
//
//#pragma region モデルの読み込み
//
//	num = to_string(number);
//
//	//右手
//	mWingRight = "BirdWingRight";
//	mWingRNum = mWingRight + num;
//	mModelRender->AddModel(mWingRNum, "Resouse/gorem_hands_R.obj", "Resouse/gorem.png");
//
//	//左手
//	mWingLeft = "BirdWingLeft";
//	mWingLNum = mWingLeft + num;
//	mModelRender->AddModel(mWingLNum, "Resouse/gorem_hands_L.obj", "Resouse/gorem.png");
//
//	mModelRender->SetAncPoint(mWingRNum, Vector3(0.0f, -3.0f, 0.0f));
//	mModelRender->SetAncPoint(mWingLNum, Vector3(0.0f, -3.0f, 0.0f));
//
//	//体
//	mBody = "BirdBody";
//	mBodyNum = mBody + num;
//	mModelRender->AddModel(mBodyNum, "Resouse/gorem_body.obj", "Resouse/gorem.png");
//#pragma endregion
//}
//
//void BirdEnemy::Update()
//{
//	if (HP <= 0)
//	{
//		death = true;
//	}
//
//	switch (mActionStep)
//	{
//	case BirdEnemy::RISING:
//		Action_Rising();//上昇
//		break;
//	case BirdEnemy::MOVE:
//		//この時だけ、プレイヤーの位置を取得できる
//		mTargetPosition = mObjManager->GetPlayer().GetPosition();
//		Action_Move(mTargetPosition);
//		break;
//	case BirdEnemy::FIRE:
//		Action_Fire();
//		break;
//	case BirdEnemy::BACK:
//		Action_Back(mOffsetRisePosition);//拠点の上空に戻る
//		break;
//	case BirdEnemy::DESCEND:
//		Action_Descend();
//		break;
//	case BirdEnemy::RELOAD:
//		Action_Reload();
//		break;
//	default:
//		break;
//	}
//}
//
//void BirdEnemy::Rend()
//{
//	//if (!GetActive()) return;
//
//	//モデルの描画
//	DirectXManager::GetInstance()->SetData3D();
//	mModelRender->Draw(mWingRNum, Vector3(position.x, position.y + 7.0f, position.z), Vector3(0, mFireAngle, 0), mScale);
//	mModelRender->Draw(mWingLNum, Vector3(position.x, position.y + 7.0f, position.z), Vector3(0, mFireAngle, 0), mScale);
//	mModelRender->Draw(mBodyNum, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), mScale);
//}
//
//void BirdEnemy::ImGuiDebug()
//{
//}
//
//void BirdEnemy::OnCollison(BaseCollider * col)
//{
//	if (col->GetColObject()->GetType() == ObjectType::BULLET)
//	{
//		HP -= col->GetColObject()->GetDamage();
//	}
//
//	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
//	{
//		//カメラに当たっているとき、描画を行う。
//		SetActive(true);
//	}
//}

void BirdEnemy::Attack()
{
}

void BirdEnemy::Move(const Vector3 & targetPosition)
{
	Vector3 distance = targetPosition - position;
	float length = distance.Length();
	distance = distance.normal();

	//弾が、生成位置より下に行かないよ！
	velocity = distance * mRiseSpeed;
	position += velocity;
}

void BirdEnemy::MoveY(const Vector3 & targetPosition)
{
	//Yを省いた位置から、距離を求める
	Vector3 holdTarget = Vector3(targetPosition.x, 0.0f, targetPosition.z);
	Vector3 holdPosition = Vector3(position.x, 0.0f, position.z);
	Vector3 distance = holdTarget - holdPosition;
	float length = distance.Length();
	distance = distance.normal();

	//Yを省いた距離
	Vector3 distHold = Vector3(distance.x, 0.0f, distance.z);

	//弾が、生成位置より下に行かないよ！
	velocity = distHold * speed;
	position += velocity;
}

void BirdEnemy::Action_Rising()
{
	////真上に上昇
	//Vector3 distance = mOffsetRisePosition - position;
	//float length = distance.Length();
	//distance = distance.normal();

	////弾が、生成位置より下に行かないよ！
	//velocity = distance * mRiseSpeed;
	//position += velocity;

	MovePoint(mOffsetRisePosition);

	if (InsideDistance(mOffsetRisePosition, 0.5f))
	{
		//上に移動完了とする。
		mActionStep = ActionStep::MOVE;
	}
}

void BirdEnemy::Action_Move(const Vector3 & targetPosition)
{
	//プレイヤーの元へ移動
	//Move(targetPosition);
	MovePointY(targetPosition);

	if (InsideDistance(targetPosition, ATTACK_LENGTH))
	{

		//攻撃を開始する
		mActionStep = ActionStep::FIRE;
	}
}

void BirdEnemy::Action_Fire()
{
	//プレイヤーに向けて弾を発射
	if (!mFireFlag)
	{
		Vector3 firePosition = AngleToVectorY(mFireAngle);
		mManager->Add(new LaunchBullet(position + firePosition, mTargetPosition, mManager, mRend, mPart, objType, mBulletNumber++,true));
		mFireFlag = true;
	}

	mLegRotate -= LEG_SPEED;

	if (mLegRotate < -LEG_RANGE)
	{
		mFinishAnimation = true;
		mLegRotate = -LEG_RANGE;
	}

	mIntervalTime->update();

	if (mIntervalTime->isTime() && mFireFlag && mFinishAnimation)
	{
		mIntervalTime->setTime(1.0f);
		mActionStep = ActionStep::BACK;
		mLegRotate = 0.0f;
	}
}

void BirdEnemy::Action_Back(const Vector3 & targetPosition)
{
	//拠点に帰る
	//Move(targetPosition);
	MovePointY(mOffsetRisePosition);

	if (InsideDistance(targetPosition, 0.5f))
	{
		//拠点上空に到着したとする
		mActionStep = ActionStep::DESCEND;
	}
}

void BirdEnemy::Action_Descend()
{
	//下に降りる
	MovePoint(mOffsetPosition);

	if (InsideDistance(mOffsetPosition, 0.5f))
	{
		mActionStep = ActionStep::RELOAD;
	}
}

void BirdEnemy::Action_Reload()
{
	mReloadTime->update();

	if (mReloadTime->isTime())
	{
		mReloadTime->setTime(1.0f);
		mActionStep = ActionStep::RISING;
		mFireFlag = false;
		mFinishAnimation = false;
		mLegRotate = 0.0f;
	}
}

bool BirdEnemy::InsideDistance(const Vector3 & distance, const float length) const
{
	Vector3 dist = distance - position;
	float inside = dist.Length();

	if (inside > length)
	{
		//範囲外
		return false;
	}

	return true;
}

Vector3 BirdEnemy::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//角度をラジアン角に戻す
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}

void BirdEnemy::EnemyInit()
{
	HP = 100;
	damage = 5;

	speed = 0.8f;
	mRadius = 2.0f;

	mIntervalTime = std::make_shared<Timer>();
	mIntervalTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);

	mScale = Vector3(2.0f, 2.0f, 2.0f);
	mOffsetPosition = position;//帰還位置を保存
	//↓帰還位置の真上を保存
	mOffsetRisePosition = Vector3(mOffsetPosition.x, mOffsetPosition.y + RISE_HEIGHT, mOffsetPosition.z);

	death = false;
	mFireFlag = false;

	objType = ObjectType::ENEMY;

	SetCollidder(Vector3(0.0f, 5.0f, 0.0f), mRadius);

	mActionStep = ActionStep::RISING;

#pragma region モデルの読み込み

	num = to_string(number);

	//弾
	mBullet = "BirdWingRight";
	mBulletNum = mBullet + num;
	mRend->AddModel(mBulletNum, "Resouse/EnemyModel/AppleBullet/apple.obj", "Resouse/EnemyModel/AppleBullet/apple.png");

	//脚
	mLeg = "BirdWingLeft";
	mLegNum = mLeg + num;
	mRend->AddModel(mLegNum, "Resouse/EnemyModel/Eagle/eagle_leg.obj", "Resouse/EnemyModel/Eagle/eagle.png");
	mRend->SetAncPoint(mLegNum, Vector3(0.0f, -3.5f, 0.0f));

	//体
	mBody = "BirdBody";
	mBodyNum = mBody + num;
	mRend->AddModel(mBodyNum, "Resouse/EnemyModel/Eagle/eagle_body.obj", "Resouse/EnemyModel/Eagle/eagle.png");
#pragma endregion
}

void BirdEnemy::EnemyUpdate()
{
	if (HP <= 0)
	{
		death = true;
	}


	//ImGui::SliderFloat("-------------------------", &mLegRotate, -500, 500);

	switch (mActionStep)
	{
	case BirdEnemy::RISING:
		Action_Rising();//上昇
		break;
	case BirdEnemy::MOVE:
		//この時だけ、プレイヤーの位置を取得できる
		mTargetPosition = mManager->GetPlayer().GetPosition();
		Action_Move(mTargetPosition);
		break;
	case BirdEnemy::FIRE:
		Action_Fire();
		break;
	case BirdEnemy::BACK:
		Action_Back(mOffsetRisePosition);//拠点の上空に戻る
		break;
	case BirdEnemy::DESCEND:
		Action_Descend();
		break;
	case BirdEnemy::RELOAD:
		Action_Reload();
		break;
	default:
		break;
	}
}

void BirdEnemy::EnemyRend()
{
	//if (!GetActive()) return;

    //モデルの描画
	DirectXManager::GetInstance()->SetData3D();

	if (!mFireFlag)
	{
		mRend->Draw(mBulletNum, Vector3(position.x, position.y + 2.0f, position.z), Vector3(0, mFireAngle, 0), mScale);
	}
	mRend->Draw(mLegNum, Vector3(position.x, position.y + 7.5f, position.z), Vector3(mLegRotate, mFireAngle, 0), mScale);
	mRend->Draw(mBodyNum, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), mScale);
}

void BirdEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
	}

	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		//カメラに当たっているとき、描画を行う。
		SetActive(true);
	}
}

void BirdEnemy::EnemyImGuiDebug()
{
}

void BirdEnemy::Search()
{
}

void BirdEnemy::Warning()
{
}
