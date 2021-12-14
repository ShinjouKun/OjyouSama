#include "SummonEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Sound/Sound.h"
#include"../../Scene/BaseScene.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"

SummonEnemy::SummonEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	std::shared_ptr<ModelRenderer> modelRender,
	shared_ptr<ParticleManager> effectManager,
	int num)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	mModelRender = modelRender;
	mEffectManager = effectManager;
	number = num;
}

SummonEnemy::~SummonEnemy()
{

}

bool SummonEnemy::GetDeathFlag() const
{
	return mDeadFlag;
}

void SummonEnemy::Init()
{
	//最初は非表示状態
	SetActive(false);

	HP = 10;
	damage = 5;

	speed = 0.2f;

	death = false;
	mWithinPlayerFlag = false;
	mAttackFlag = false;
	mStep = false;
	mDeathAnimation = false;
	mDeadFlag = false;

	mScale = Vector3(0.5f, 0.5f, 0.5f);
	SetCollidder(Vector3(0.0f, 1.0f, 0.0f), 1.5f);

	//列挙型初期化
	objType = ObjectType::ENEMY;
	mAttackStep = AttackStep::FALL_DOWN;
	mDeathStep = DeathAnimationStep::RISE_SKY;

	//サウンド初期化
	mAttackSE = std::make_shared<Sound>("SE/punti.mp3", true);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Summon_Damage.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//タイマー初期化
	mGetupTimer = std::make_shared<Timer>();
	mGetupTimer->setTime(1.0f);
	mRiseTime = std::make_shared<Timer>();
	mRiseTime->setTime(1.0f);
	mDeathTime = std::make_shared<Timer>();
	mDeathTime->setTime(1.0f);

	//ダメージ用パーティクル
	mDamageParticle = std::make_shared<Hit>(Vector3::zero, true);
	mDamageParticle->Stop();

	//死亡用エフェクト
	mDeathParticle = std::make_shared<Explosion>(Vector3::zero, true);
	mDeathParticle->Stop();

	//モデル読み込み
	mMyNumber = to_string(number);
	mSummon = "SummonEnemy";
	mSummonNum = mSummon + mMyNumber;
	mModelRender->AddModel(mSummonNum, "Resouse/wood.obj", "Resouse/Big-treeA.png");
}

void SummonEnemy::Update()
{
	/*死亡状態監視*/
	CheckAlive();

	//仮死状態なら処理しない
	if (mDeathAnimation) return;

	/*移動*/
	Move();

	/*倒れこみ攻撃*/
	DownAttack();
}

void SummonEnemy::Rend()
{
	//表示状態かどうか
	//if(!GetActive()) return;

	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(mSummonNum, position, Vector3(angle.x, mFireAngle, angle.z), mScale);
}

void SummonEnemy::ImGuiDebug()
{
}

void SummonEnemy::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//SE発射
		mDamageSE->setPos(position);
		mDamageSE->play();

		//パーティクル発射
		mDamageParticle->setPos(Vector3(position.x, position.y + 5.0f, position.z));
		mDamageParticle->Play();

		HP -= col->GetColObject()->GetDamage();
	}

	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		//カメラに当たっているとき、描画を行う。
		SetActive(true);
	}

	if (col->GetColObject()->GetType() == ObjectType::ENEMY)
	{
		//自分の番号が相手より小さかったら
		if (col->GetColObject()->GetID() > GetID())
		{
			position = mPreviousPosition;
		}
	}
	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}
}

void SummonEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		//アニメーション(仮死状態)にする
		mDeathAnimation = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	/*死亡アニメーション開始*/
	DeathAnimation();
}

void SummonEnemy::Move()
{
	//前フレームの位置を取得
	mPreviousPosition = position - velocity;

	//プレイヤーの位置を監視
	mPlayerPosition = mObjManager->GetPlayer().GetPosition();

	//視野範囲内にプレイヤーがいるなら
	if (WithinDistance(mPlayerPosition, SEARCH_LENGTH))
	{
		mWithinPlayerFlag = true;

		//突撃範囲内にプレイヤーがいるなら
		if (WithinDistance(mPlayerPosition, FIND_LRNGTH))
		{
			//突撃するとき、移動速度がすこし上がる
			speed = 0.3f;

			//攻撃範囲内にプレイヤーがいたら
			if (WithinDistance(mPlayerPosition, ATTACK_LENGTH))
			{
				//攻撃中にする
				mAttackFlag = true;
			}
		}
		else
		{
			speed = 0.2f;
		}
	}
	else
	{
		mWithinPlayerFlag = false;
	}

	//範囲内にプレイヤーがいる & 攻撃していない時
	if (mWithinPlayerFlag && !mAttackFlag)
	{
		Vector3 distance = mPlayerPosition - position;
		distance = distance.normal();
		//二点間の角度を求める
		float radian = atan2(distance.x, distance.z);
		//回転を反映
		angle.y = Math::toDegrees(radian) + 180.0f;
		mFireAngle = -Math::toDegrees(radian) - 180.0f;

		//移動する
		velocity = distance * speed;
		position += velocity;

		/*移動アニメーション*/
		MoveAnimation();
	}
	else
	{
		//脚の角度を元に戻す
		angle.z = 0.0f;
	}
}

void SummonEnemy::MoveAnimation()
{
	//右ステップ
	if (mStep)
	{
		angle.z++;
		if (angle.z > 25.0f)
		{
			mStep = false;
		}
	}
	else
	{
		angle.z--;
		if (angle.z < -25.0f)
		{
			mStep = true;
		}
	}
}

void SummonEnemy::DownAttack()
{
	//攻撃中出なければ処理しない
	if (!mAttackFlag) return;

	switch (mAttackStep)
	{
	case SummonEnemy::FALL_DOWN:
		AttackStep_FallDown();
		break;
	case SummonEnemy::WAIT:
		AttackStep_Wait();
		break;
	case SummonEnemy::GET_UP:
		AttackStep_GetUp();
		break;
	default:
		break;
	}
}

void SummonEnemy::AttackStep_FallDown()
{
	angle.x += mDownSpeed;
	if (angle.x > 90.0f)
	{
		mAttackStep = AttackStep::WAIT;
		mAttackSE->setPos(position);
		mAttackSE->play();
	}
}

void SummonEnemy::AttackStep_Wait()
{
	mGetupTimer->update();

	if (mGetupTimer->isTime())
	{
		mGetupTimer->setTime(1.0f);
		mAttackStep = AttackStep::GET_UP;
	}
}

void SummonEnemy::AttackStep_GetUp()
{
	angle.x -= mDownSpeed;
	if (angle.x <= 0.0f)
	{
		mAttackFlag = false;
		mAttackStep = AttackStep::FALL_DOWN;
	}
}

void SummonEnemy::DeathAnimation()
{
	//仮死状態でない　なら処理しない
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case SummonEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case SummonEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void SummonEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//時間になっていなければ
	if (!mRiseTime->isTime())
	{
		//回転
		mFireAngle += 50.0f;
		//上昇
		position.y += 0.5f;
	}
	else
	{
		//時間になったら(1フレームだけ呼ばれる)
		//ここでSEを鳴らしたり、爆発させたりする
		//SE発射
		mDeathSE->setPos(position);
		mDeathSE->play();

		//パーティクル発射
		mDeathParticle->setPos(position);
		mDeathParticle->Play();

		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void SummonEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}

bool SummonEnemy::WithinDistance(const Vector3 & targetPosition, const float distance)
{
	//移動&停止
	Vector3 dist = targetPosition - position;
	float length = dist.Length();
	dist = dist.normal();

	//目標位置との距離が、指定距離以上だったら 処理しない
	if (length > distance)
	{
		return false;
	}

	return true;
}
