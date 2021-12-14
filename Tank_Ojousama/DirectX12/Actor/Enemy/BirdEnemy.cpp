#include "BirdEnemy.h"
#include "../../Weapons/LaunchBullet.h"
#include "../../Scene/BaseScene.h"

#include "../../Utility/Random.h"
#include "../../Items/Repair.h"
#include "../../Items/Shield.h"
#include "../../Items/Smoke.h"
#include "../../Render/TexRenderer.h"


BirdEnemy::BirdEnemy(
	const Vector3 & pos, 
	const Vector3 & ang,
	shared_ptr<TexRenderer> texRender,
	int num
)
{
	position = pos;
	angle = ang;
	mTexRender = texRender;
	number = num;
}

BirdEnemy::~BirdEnemy()
{
}

void BirdEnemy::Attack()
{
	switch (mActionStep)
	{
	case BirdEnemy::RISING:
		Action_Rising();//上昇
		break;
	case BirdEnemy::MOVE:
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

void BirdEnemy::Action_Rising()
{
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
	MovePointY(targetPosition);

	if (InsideDistanceY(targetPosition, ATTACK_LENGTH))
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

		mAttackSE->setPos(position);
		mAttackSE->play();
	}

	mLegRotate -= LEG_SPEED;

	if (mLegRotate < -LEG_RANGE)
	{
		mFinishAnimation = true;
		mLegRotate = -LEG_RANGE;
	}

	MovePointY(mPlayerPosition);

	if (InsideDistanceY(mPlayerPosition, 1.0f) && mFireFlag && mFinishAnimation)
	{
		mActionStep = ActionStep::BACK;
		mLegRotate = 0.0f;
	}
}

void BirdEnemy::Action_Back(const Vector3 & targetPosition)
{
	//拠点に帰る
	MovePointY(mOffsetRisePosition);

	if (InsideDistanceY(targetPosition, 0.5f))
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

bool BirdEnemy::InsideDistanceY(const Vector3 & distance, const float length) const
{
	Vector3 dist = Vector3(distance.x, 0.0f, distance.z) - Vector3(position.x, 0.0f, position.z);
	float inside = dist.Length();

	if (inside > length)
	{
		//範囲外
		return false;
	}

	return true;
}

void BirdEnemy::EnemyInit()
{
	HP = 5;
	damage = 5;

	speed = 0.8f;
	mRadius = 2.0f;

	mFinishAnimation = false;
	death = false;
	mFireFlag = false;
	mCreateItem = false;

	objType = ObjectType::ENEMY;
	mActionStep = ActionStep::RISING;

	mScale = Vector3(2.0f, 2.0f, 2.0f);
	mOffsetPosition = position;//帰還位置を保存
	//↓帰還位置の真上を保存
	mOffsetRisePosition = Vector3(mOffsetPosition.x, mOffsetPosition.y + RISE_HEIGHT, mOffsetPosition.z);
	SetCollidder(Vector3(0.0f, 5.0f, 0.0f), mRadius);

	//タイマー初期化
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);

	//サウンド初期化
	mAttackSE = std::make_shared<Sound>("SE/Bird_Attack.mp3", true);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//パーティクル初期化
	EXPLOSION_EFFECT = "Explosion";
	mParticleEmitter = make_shared<ParticleEmitterBox>(mPart);
	mParticleEmitter->LoadAndSet(EXPLOSION_EFFECT, "Resouse/Bom.jpg");

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
	//生存状態の監視
	AliveSurveillance();

	/*死亡状態監視*/
	CheckAlive();

	/*攻撃*/
	Attack();
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
		mDamageSE->setPos(position);
		mDamageSE->play();
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

void BirdEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		//エフェクト発射
		mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, position, angle);

		if (mCreateItem) return;

		Random::initialize();

		int count = Random::randomRange(0, 2);

		if (count == 0)
		{
			mManager->Add(new Repair(Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), mManager, mRend, mTexRender, ItemState::Low, 0, 500000, 20));
			mCreateItem = true;
		}
		else /* if (count == 1)*/
		{
			mManager->Add(new Shield(position, angle, mManager, mRend, mTexRender, ItemState::Low, 0, 20));
			mCreateItem = true;
		}
		//else
		//{
		//	mManager->Add(new Smoke(position, angle, mManager, mRend, mPart, mTexRender, ItemState::Low, 0, 5 * 60));
		//}
	}
}
