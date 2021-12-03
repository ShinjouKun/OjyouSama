#include "SummonEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/NormalBullet.h"
#include "../../Utility/Timer/Timer.h"
#include "AttackArea.h"

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
	mCreateObject = false;

	mAttackStep = AttackStep::FALL_DOWN;

	//mAttackArea = std::make_shared<AttackArea>(position, Vector3().zero, mObjManager, mModelRender, number);
	//mAttackArea->SetActive(false);


	mGetupTimer = std::make_shared<Timer>();
	mGetupTimer->setTime(1.0f);

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3(0.0f, 1.0f, 0.0f), 1.5f));


	mModelNum = to_string(number);
	mModelNumName01 = mModelName01 + mModelNum;
	mModelRender->AddModel(mModelNumName01, "Resouse/wood.obj", "Resouse/Big-treeA.png");
}

void SummonEnemy::Update()
{
	mPreviousPosition = position - velocity;

	//プレイヤーの位置を監視
	mPlayerPosition = mObjManager->GetPlayer().GetPosition();

	///*一度だけオブジェクトを生成*/
	//CreateObject();

	if (HP <= 0)
	{
		death = true;
	}

	//範囲内にプレイヤーがいるなら
	if (WithinDistance(mPlayerPosition,SEARCH_LENGTH))
	{
		mWithinPlayerFlag = true;

		//突撃範囲内にプレイヤーがいるなら
		if (WithinDistance(mPlayerPosition, FIND_LRNGTH))
		{
			speed = 0.3f;

			if (WithinDistance(mPlayerPosition, ATTACK_LENGTH))
			{
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

	if (mWithinPlayerFlag && !mAttackFlag)
	{

		Vector3 distance = mPlayerPosition - position;
		distance = distance.normal();
		//二点間の角度を求める
		float radian = atan2(distance.x, distance.z);
		//回転を反映
		angle.y = Math::toDegrees(radian) + 180.0f;
		tt = -Math::toDegrees(radian) - 180.0f;

		velocity = distance * speed;
		position += velocity;

#pragma region アニメーション

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

#pragma endregion
	}
	else
	{
		angle.z = 0.0f;
	}



	if (mAttackFlag)
	{
		/*
		その場に倒れこむ->この時当たり判定表示
		->x軸を90度回転すればいいかな？
		しばらく倒れたまま->約一秒
		その場で起き上がる
		起き上がり終わったら攻撃終了
		*/

		//Vector3 areaPos = RotateY(tt) * ATTACK_LENGTH;
		//mAttackArea->SetActive(true, position + areaPos, Vector3(0.0f, tt, 0.0f));

		switch (mAttackStep)
		{
		case SummonEnemy::FALL_DOWN:

			angle.x += t;
			if (angle.x > 90.0f)
			{
				mAttackStep = AttackStep::WAIT;
			}

			break;
		case SummonEnemy::WAIT:

			mGetupTimer->update();

			if (mGetupTimer->isTime())
			{
				mGetupTimer->setTime(1.0f);
				mAttackStep = AttackStep::GET_UP;
			}

			break;
		case SummonEnemy::GET_UP:

			angle.x -= t;
			if (angle.x <= 0.0f)
			{
				mAttackFlag = false;
				mAttackStep = AttackStep::FALL_DOWN;
			}

			break;
		default:
			break;
		}
	}



	//ImGui::SliderInt("**********************", &HP, 0, 100);
}

void SummonEnemy::Rend()
{
	//表示状態かどうか
    //if(!GetActive()) return;

	DirectXManager::GetInstance()->SetData3D();//モデル用をセット
	mModelRender->Draw(mModelNumName01, position, Vector3(angle.x,tt,angle.z), Vector3(0.5f, 0.5f, 0.5f));
}

void SummonEnemy::ImGuiDebug()
{
}

void SummonEnemy::OnCollison(BaseCollider * col)
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

void SummonEnemy::CreateObject()
{
	if (mCreateObject) return;

	mObjManager->Add(mAttackArea.get());
	mCreateObject = true;
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
