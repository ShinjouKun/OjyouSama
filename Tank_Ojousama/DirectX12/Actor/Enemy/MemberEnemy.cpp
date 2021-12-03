#include "MemberEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/NormalBullet.h"
#include "../../Weapons/ElfBullet.h"
#include "../../Weapons/LaunchBullet.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Utility/Random.h"

MemberEnemy::MemberEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	std::shared_ptr<ModelRenderer> modelRender,
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
	//à⁄ìÆ&í‚é~
	Vector3 dist = targetPosition - position;
	float length = dist.Length();
	dist = dist.normal();

	//ñ⁄ïWà íuÇ∆ÇÃãóó£Ç™ÅAéwíËãóó£à»â∫ÇæÇ¡ÇΩÇÁ èàóùÇµÇ»Ç¢
	if (length < distance) return;

	//ìÒì_ä‘ÇÃäpìxÇãÅÇﬂÇÈ
	float radian = atan2(dist.x, dist.z);
	//âÒì]ÇîΩâf
	angle.y = Math::toDegrees(radian) + 180.0f;
	mFireAngle = -Math::toDegrees(radian) - 180.0f;

	velocity = dist * speed;
	position += velocity;
}

bool MemberEnemy::WithinDistance(const Vector3& targetPosition, const float distance)
{
	//à⁄ìÆ&í‚é~
	Vector3 dist = targetPosition - position;
	float length = dist.Length();
	dist = dist.normal();

	//ñ⁄ïWà íuÇ∆ÇÃãóó£Ç™ÅAéwíËãóó£à»â∫ÇæÇ¡ÇΩÇÁ èàóùÇµÇ»Ç¢
	if (length < distance)
	{
		return false;
	}

	return true;
}

Vector3 MemberEnemy::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//äpìxÇÉâÉWÉAÉìäpÇ…ñﬂÇ∑
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}

void MemberEnemy::AttackStep_NONE()
{
	//çUåÇéwóﬂÇéÛÇØéÊÇ¡ÇΩ
	if (mAttackCommand)
	{
		mAttackStep = AttackStep::AIMING;
	}
}

void MemberEnemy::AttackStep_AIMING()
{
	mAimingTime->update();
	//ñCìÉÇÕÅAÉvÉåÉCÉÑÅ[ÇÃï˚Çå¸Ç¢ÇƒÇ¢ÇÈ
	Vector3 dist = mAttackTarget - position;
	dist = dist.normal();
	float radian = atan2(dist.x, dist.z);
	mFireAngle = -Math::toDegrees(radian) - 180.0f;

	if (mAimingTime->isTime())
	{
		mAimingTime->setTime(1.0f);
		mAttackStep = AttackStep::FIRE;
	}
}

void MemberEnemy::AttackStep_FIRE()
{
	//íeî≠éÀ
	Vector3 firePosition = AngleToVectorY(mFireAngle);
	mObjManager->Add(new ElfBullet(position + firePosition, Vector3(0.0f, mFireAngle, 0.0f), mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++));
	//mObjManager->Add(new LaunchBullet(position + firePosition, mAttackTarget, mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++));
	mAttackStep = AttackStep::RELOAD;
}

void MemberEnemy::AttackStep_RELOAD()
{
	//ÉäÉçÅ[Éh
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
	damage = 5;
	radius = 1.0f;
	mFireAngle = angle.y;
	//turretAngle = 0.0f;
	mBulletNumber = 0;
	mMoveDirection = 0;

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

	mLegRotate = 0.0f;

#pragma region ÉÇÉfÉãÇÃì«Ç›çûÇ›

	//î‘çÜÇintÇ©ÇÁstringÇ…ïœä∑
	mMyNumber = to_string(number);

	//âEë´èÓïÒìoò^
	mRightLeg = "MemberLegR";
	mRLegNumber = mRightLeg + mMyNumber;
	mModelRender->AddModel(mRLegNumber, "Resouse/EnemyModel/Elf_B/leg_R_B.obj", "Resouse/EnemyModel/Elf_B/leg_LR2.png");
	mModelRender->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//ç∂ë´èÓïÒìoò^
	mLeftLeg = "MemberLegL";
	mLLegNumber = mLeftLeg + mMyNumber;
	mModelRender->AddModel(mLLegNumber, "Resouse/EnemyModel/Elf_B/leg_L_B.obj", "Resouse/EnemyModel/Elf_B/leg_LR2.png");
	mModelRender->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//ì™èÓïÒìoò^
	mHead = "MemberHead";
	mHeadNumber = mHead + mMyNumber;
	mModelRender->AddModel(mHeadNumber, "Resouse/EnemyModel/Elf_B/elf_head2.obj", "Resouse/EnemyModel/Elf_B/face_color2.png");

	//ëÃèÓïÒìoò^
	mBody = "MemberBody";
	mBodyNumber = mBody + mMyNumber;
	mModelRender->AddModel(mBodyNumber, "Resouse/EnemyModel/Elf_B/elf_body2.obj", "Resouse/EnemyModel/Elf_B/hand_bow_color2.png");
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

	//ë‡í∑Çé∏Ç¡ÇΩéû
	if (mCaptainLost)
	{
		if (mMoveDirection == 0)
		{
			int direction = 0;
			Random::initialize();

			direction = Random::randomRange(0, 3);

			if (direction == 0)
			{
				mRandomDirection = Vector3(0.1f, 0.0f, 0.0f);
				mMoveDirection = 1;
			}
			else if (direction == 1)
			{
				mRandomDirection = Vector3(-0.1f, 0.0f, 0.0f);
				mMoveDirection = 1;
			}
			else if (direction == 2)
			{
				mRandomDirection = Vector3(0.0f, 0.0f, 0.1f);
				mMoveDirection = 1;
			}
			else if (direction == 3)
			{
				mRandomDirection = Vector3(0.0f, 0.0f, -0.1f);
				mMoveDirection = 1;
			}
		}
		else if (mMoveDirection == 1)
		{
			MoveTarget(position + mRandomDirection, -1.0f);
			mRandomMoveTimer->update();

			if (mRandomMoveTimer->isTime())
			{
				mRandomMoveTimer->setTime(0.5f);
				mMoveDirection = 0;
			}
		}
	}
	else
	{
		//à⁄ìÆ
		MoveTarget(mFixedPosition, 1.0f);
	}

	if (velocity.x == 0.0f || velocity.z == 0.0f)
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


	//çUåÇéwóﬂ
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

	//çıìGéwóﬂÇéÛÇØéÊÇ¡ÇΩ
	if (mSearchCommand)
	{
		//ãóó£åvéZ
		Vector3 distance = mSearchTarget - position;
		float length = distance.Length();

		//ãóó£ë™íË
		if (length < SEARCH_RANGE)
		{
			//ë‡í∑Ç…à íuÇ∆ÉtÉâÉOÇì`Ç¶ÇÈÅB
			mSearchResult = true;
			mSearchPosition = mSearchTarget;
		}
		else
		{
			//î≠å©ÇµÇƒÇ¢Ç»Ç¢
			mSearchResult = false;
		}

		//çıìGÇèIóπÇ∑ÇÈ
		mSearchCommand = false;
	}
}

void MemberEnemy::Rend()
{
	//ï\é¶èÛë‘Ç©Ç«Ç§Ç©
	//if(!GetActive()) return;

	//ÉÇÉfÉãÇÃï`âÊ
	DirectXManager::GetInstance()->SetData3D();
	//mModelRender->Draw(numBarrel, Vector3(position.x, position.y, position.z), Vector3(0, barrelAngle, 0), scale);
	//mModelRender->Draw(numTurret, Vector3(position.x, position.y, position.z), Vector3(turretAngle, barrelAngle, 0), scale);
	//mModelRender->Draw(numBody, Vector3(position.x, position.y, position.z), Vector3(0, -angle.y, 0), scale);

	mModelRender->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);//âEãr
	mModelRender->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);//ç∂ãr
	mModelRender->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//ì™Ç∆éË
	mModelRender->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//ëÃÇ∆ã|
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
		//ÉJÉÅÉâÇ…ìñÇΩÇ¡ÇƒÇ¢ÇÈÇ∆Ç´ÅAï`âÊÇçsÇ§ÅB
		SetActive(true);
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}

	if (col->GetColObject()->GetType() == ObjectType::ENEMY)
	{
		//é©ï™ÇÃî‘çÜÇ™ëäéËÇÊÇËè¨Ç≥Ç©Ç¡ÇΩÇÁ
		if (col->GetColObject()->GetID() > GetID())
		{
			position = mPreviousPosition;
		}
	}
}
