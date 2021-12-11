#include "MortarEnemy.h"
#include "../../Weapons/LaunchBullet.h"
#include "../../Utility/Random.h"
#include"../../Scene/BaseScene.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"

MortarEnemy::MortarEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	int num
)
{
	position = pos;
	angle = ang;
	number = num;
}

MortarEnemy::~MortarEnemy()
{
}

void MortarEnemy::AttackStep_AIMING()
{
	mAimingTime->update();

	mTargetPosition = mManager->GetPlayer().GetPosition();

	Vector3 distance = mTargetPosition - position;
	distance = distance.normal();

	//二点間の角度を求める
	float radian = atan2(distance.x, distance.z);
	//回転を反映
	barrelAngle = -Math::toDegrees(radian) - 180.0f;

	if (mAimingTime->isTime())
	{
		mAimingTime->setTime(1.0f);
		mAttackStep = AttackStep::FIRE;
	}
}

void MortarEnemy::AttackStep_FIRE()
{
	if (!mFireFlag)
	{
		Vector3 firePosition = AngleToVectorY(barrelAngle);
		mManager->Add(new LaunchBullet(position + firePosition, mTargetPosition, mManager, mRend, mPart, objType, mBulletNumber++));
		mFireFlag = true;
		mAttackSE->play();
	}

	//腕を振り上げる
	mHandAngle -= 10.0f;

	//振り上げ終わったら
	if (mFireFlag && mHandAngle <= -90.0f)
	{
		mAttackStep = AttackStep::RELOAD;
	}
}

void MortarEnemy::AttackStep_RELOAD()
{
	//リロード
	mReloadTime->update();

	//腕を元に戻す
	mHandAngle += 3.0f;

	if (mHandAngle >= 0.0f)
	{
		mHandAngle = 0.0f;
	}

	//リロード完了 && 腕が元に戻りきっていたら
	if (mReloadTime->isTime() && mHandAngle >= 0.0f)
	{
		//ここの値がランダムじゃないと、全部の攻撃位置が重なっちゃう

		Random::initialize();

		float time = Random::randomRange(0.5f, 1.5f);

		mReloadTime->setTime(time);
		mFireFlag = false;
		mAttackStep = AttackStep::AIMING;
	}
}

void MortarEnemy::CheckAlive()
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

void MortarEnemy::Attack()
{


}

void MortarEnemy::DeathAnimation()
{
	//仮死状態でない　なら処理しない
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case MortarEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case MortarEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void MortarEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//時間になっていなければ
	if (!mRiseTime->isTime())
	{
		//回転
		barrelAngle += 50.0f;
		//上昇
		position.y += 0.2f;
	}
	else
	{
		//時間になったら(1フレームだけ呼ばれる)
		//ここでSEを鳴らしたり、爆発させたりする
		//エフェクト発射
		mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, position, angle);
		//SE発射
		mDamageSE->play();

		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void MortarEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}

void MortarEnemy::EnemyInit()
{
	HP = 10;
	damage = 5;

	mRadius = 1.5f;
	barrelAngle = angle.y;
	mHandAngle = 0.0f;

	death = false;
	mFireFlag = false;
	mDeathAnimation = false;
	mDeadFlag = false;

	objType = ObjectType::ENEMY;
	mAttackStep = AttackStep::AIMING;
	mDeathStep = DeathAnimationStep::RISE_SKY;

	mScale = Vector3(2.0f, 2.0f, 2.0f);
	SetCollidder(Vector3().zero, mRadius);

	//タイマー初期化
	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);
	mRiseTime = std::make_shared<Timer>();
	mRiseTime->setTime(1.0f);
	mDeathTime = std::make_shared<Timer>();
	mDeathTime->setTime(1.0f);

	//サウンド初期化
	mAttackSE = std::make_shared<Sound>("SE/Golem_Attack.mp3", false);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Golem_Damage.mp3", false);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//パーティクル初期化
	EXPLOSION_EFFECT = "Explosion";
	mParticleEmitter = make_shared<ParticleEmitterBox>(mPart);
	mParticleEmitter->LoadAndSet(EXPLOSION_EFFECT, "Resouse/Bom.jpg");

	mExplosion = std::make_shared<Explosion>(Vector3::zero, true);
	mExplosion->Stop();

#pragma region モデルの読み込み

	num = to_string(number);

	//右手
	mHandRight = "MortarHandRight";
	mHandRNum = mHandRight + num;
	mRend->AddModel(mHandRNum, "Resouse/gorem_hands_R.obj", "Resouse/gorem.png");

	//左手
	mHandLeft = "MortarHandLeft";
	mHandLNum = mHandLeft + num;
	mRend->AddModel(mHandLNum, "Resouse/gorem_hands_L.obj", "Resouse/gorem.png");

	mRend->SetAncPoint(mHandRNum, Vector3(0.0f, -3.0f, 0.0f));
	mRend->SetAncPoint(mHandLNum, Vector3(0.0f, -3.0f, 0.0f));

	//体
	mBody = "MortarBody";
	mBodyNum = mBody + num;
	mRend->AddModel(mBodyNum, "Resouse/gorem_body.obj", "Resouse/gorem.png");

#pragma endregion
}

void MortarEnemy::EnemyUpdate()
{
	/*生存状態を監視*/
	CheckAlive();

	switch (mAttackStep)
	{

	case MortarEnemy::AIMING:
		AttackStep_AIMING();
		break;
	case MortarEnemy::FIRE:
		AttackStep_FIRE();
		break;
	case MortarEnemy::RELOAD:
		AttackStep_RELOAD();
		break;
	default:
		break;
	}

	//仮死状態なら処理しない
	if (mDeathAnimation) return;

	/*攻撃*/
	void Attack();
}

void MortarEnemy::EnemyRend()
{
	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	//モデルの描画
	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mHandRNum, Vector3(position.x, position.y + 7.0f, position.z), Vector3(mHandAngle, barrelAngle, 0), mScale);
	mRend->Draw(mHandLNum, Vector3(position.x, position.y + 7.0f, position.z), Vector3(mHandAngle, barrelAngle, 0), mScale);
	mRend->Draw(mBodyNum, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), mScale);
}

void MortarEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
		mExplosion->setPos(position);
		mExplosion->Play();
		mDamageSE->play();
	}

	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		//カメラに当たっているとき、描画を行う。
		SetActive(true);
	}
}

void MortarEnemy::EnemyImGuiDebug()
{
}