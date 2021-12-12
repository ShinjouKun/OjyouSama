#include "MemberEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/ElfBullet.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Utility/Random.h"
#include "../../Sound/Sound.h"
#include"../../Scene/BaseScene.h"

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
	//�ړ�&��~
	Vector3 dist = targetPosition - position;
	float length = dist.Length();
	dist = dist.normal();

	//�ڕW�ʒu�Ƃ̋������A�w�苗���ȉ��������� �������Ȃ�
	if (length < distance) return;

	//��_�Ԃ̊p�x�����߂�
	float radian = atan2(dist.x, dist.z);
	//��]�𔽉f
	angle.y = Math::toDegrees(radian) + 180.0f;
	mFireAngle = -Math::toDegrees(radian) - 180.0f;

	velocity = dist * speed;
	position += velocity;
}

bool MemberEnemy::WithinDistance(const Vector3& targetPosition, const float distance)
{
	//�ړ�&��~
	Vector3 dist = targetPosition - position;
	float length = dist.Length();
	dist = dist.normal();

	//�ڕW�ʒu�Ƃ̋������A�w�苗���ȉ��������� �������Ȃ�
	if (length < distance)
	{
		return false;
	}

	return true;
}

Vector3 MemberEnemy::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//�p�x�����W�A���p�ɖ߂�
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}

void MemberEnemy::DeathAnimation()
{
	//������ԂłȂ��@�Ȃ珈�����Ȃ�
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case MemberEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case MemberEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void MemberEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//���ԂɂȂ��Ă��Ȃ����
	if (!mRiseTime->isTime())
	{
		//��]
		mFireAngle += 50.0f;
		//�㏸
		position.y += 0.2f;
	}
	else
	{
		//���ԂɂȂ�����(1�t���[�������Ă΂��)
		//������SE��炵����A�����������肷��
		//�G�t�F�N�g����
		mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, position, angle);
		//SE����
		mDeathSE->play();

		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void MemberEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}

void MemberEnemy::AttackStep_NONE()
{
	//�U���w�߂��󂯎����
	if (mAttackCommand)
	{
		mAttackStep = AttackStep::AIMING;
	}
}

void MemberEnemy::AttackStep_AIMING()
{
	mAimingTime->update();
	//�C���́A�v���C���[�̕��������Ă���
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
	//�e����
	Vector3 firePosition = AngleToVectorY(mFireAngle);
	mObjManager->Add(new ElfBullet(position + firePosition, Vector3(0.0f, mFireAngle, 0.0f), mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++));
	mAttackSE->play();
	mAttackStep = AttackStep::RELOAD;
}

void MemberEnemy::AttackStep_RELOAD()
{
	//�����[�h
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

void MemberEnemy::SetMoveRange(const Vector3 & moveRangeMin, const Vector3 & moveRangeMax)
{
	mMoveRangeMin = moveRangeMin;
	mMoveRangeMax = moveRangeMax;
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
	mRadius = 1.0f;
	mFireAngle = angle.y;
	mBulletNumber = 0;
	mMoveDirection = 0;
	mLegRotate = 0.0f;

	scale = Vector3().one;
	mFixedPosition = Vector3().zero;
	mAttackTarget = Vector3().zero;
	mSearchTarget = Vector3().zero;
	mRandomDirection = Vector3(0.0f, 0.0f, 0.1f);

	/*�ނ����*/
	mMoveRangeMax = Vector3(0.0f, 0.0f, 520.0f);

	death = false;
	mDeadFlag = false;
	mAttackCommand = false;
	mSearchCommand = false;
	mSearchResult = false;
	mCaptainLost = false;
	mRotDirection = false;
	mDeathAnimation = false;
	mDeadFlag = false;

	objType = ObjectType::ENEMY;
	mAttackStep = AttackStep::NONE;
	mDeathStep = DeathAnimationStep::RISE_SKY;
	SetCollidder(Vector3().zero, mRadius);

	//�^�C�}�[������
	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);
	mRandomMoveTimer = std::make_shared<Timer>();
	mRandomMoveTimer->setTime(0.5f);
	mRiseTime = std::make_shared<Timer>();
	mRiseTime->setTime(1.0f);
	mDeathTime = std::make_shared<Timer>();
	mDeathTime->setTime(1.0f);

	//�T�E���h������
	mAttackSE = std::make_shared<Sound>("SE/hirai.mp3", false);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", false);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Elf_Damage01.mp3", false);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//�p�[�e�B�N��������
	EXPLOSION_EFFECT = "Explosion";
	mParticleEmitter = make_shared<ParticleEmitterBox>(mEffectManager);
	mParticleEmitter->LoadAndSet(EXPLOSION_EFFECT, "Resouse/Bom.jpg");

#pragma region ���f���̓ǂݍ���

	//�ԍ���int����string�ɕϊ�
	mMyNumber = to_string(number);

	//�E�����o�^
	mRightLeg = "MemberLegR";
	mRLegNumber = mRightLeg + mMyNumber;
	mModelRender->AddModel(mRLegNumber, "Resouse/EnemyModel/Elf_B/leg_R_B.obj", "Resouse/EnemyModel/Elf_B/leg_LR2.png");
	mModelRender->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�������o�^
	mLeftLeg = "MemberLegL";
	mLLegNumber = mLeftLeg + mMyNumber;
	mModelRender->AddModel(mLLegNumber, "Resouse/EnemyModel/Elf_B/leg_L_B.obj", "Resouse/EnemyModel/Elf_B/leg_LR2.png");
	mModelRender->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�����o�^
	mHead = "MemberHead";
	mHeadNumber = mHead + mMyNumber;
	mModelRender->AddModel(mHeadNumber, "Resouse/EnemyModel/Elf_B/elf_head2.obj", "Resouse/EnemyModel/Elf_B/face_color2.png");

	//�̏��o�^
	mBody = "MemberBody";
	mBodyNumber = mBody + mMyNumber;
	mModelRender->AddModel(mBodyNumber, "Resouse/EnemyModel/Elf_B/elf_body2.obj", "Resouse/EnemyModel/Elf_B/hand_bow_color2.png");
#pragma endregion
}

void MemberEnemy::Update()
{
	mPreviousPosition = position - velocity;

	/*������Ԃ��Ď�*/
	CheckAlive();

	//������ԂȂ珈�����Ȃ�
	if (mDeathAnimation) return;

	/*�ړ�*/
	Move();

	/*�U��*/
	Attack();

	/*�����s��*/
	Confusion();
}

void MemberEnemy::Rend()
{
	//�\����Ԃ��ǂ���
	//if(!GetActive()) return;

	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	//���f���̕`��
	DirectXManager::GetInstance()->SetData3D();
	mModelRender->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);//�E�r
	mModelRender->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);//���r
	mModelRender->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//���Ǝ�
	mModelRender->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//�̂Ƌ|
}

void MemberEnemy::ImGuiDebug()
{
}

void MemberEnemy::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		mDamageSE->play();
		HP -= col->GetColObject()->GetDamage();
		mSearchResult = true;
		mSearchPosition = mSearchTarget;
	}

	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		//�J�����ɓ������Ă���Ƃ��A�`����s���B
		SetActive(true);
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}

	if (col->GetColObject()->GetType() == ObjectType::ENEMY)
	{
		//�����̔ԍ��������菬����������
		if (col->GetColObject()->GetID() > GetID())
		{
			position = mPreviousPosition;
		}
	}
}

void MemberEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		mDeathAnimation = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	/*���S�A�j���[�V�������J�n*/
	DeathAnimation();
}

void MemberEnemy::Move()
{
	//����������ł����珈�����Ȃ�
	if (mCaptainLost) return;

	//�ړ�
	MoveTarget(mFixedPosition, 1.0f);

	/*�s������*/
	MoveRange();

	/*���s�A�j���[�V����*/
	MoveAnimation();
}

void MemberEnemy::MoveRange()
{
	if (position.x < mMoveRangeMin.x)
	{
		position.x = mMoveRangeMin.x;
	}
	else if (position.x > mMoveRangeMax.x)
	{
		position.x = mMoveRangeMax.x;
	}

	if (position.z < mMoveRangeMin.z)
	{
		position.z = mMoveRangeMin.z;
	}
	else if (position.z > mMoveRangeMax.z)
	{
		position.z = mMoveRangeMax.z;
	}
}

void MemberEnemy::MoveAnimation()
{
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
}

void MemberEnemy::Attack()
{
	//���G�w�߂��󂯎����
	if (mSearchCommand)
	{
		//�����v�Z
		Vector3 distance = mSearchTarget - position;
		float length = distance.Length();

		//��������
		if (length < SEARCH_RANGE)
		{
			//�����Ɉʒu�ƃt���O��`����B
			mSearchResult = true;
			mSearchPosition = mSearchTarget;
		}
		else
		{
			//�������Ă��Ȃ�
			mSearchResult = false;
		}

		//���G���I������
		mSearchCommand = false;
	}
}

void MemberEnemy::Confusion()
{
	//�����������Ă����珈�����Ȃ�
	if (!mCaptainLost) return;
	//��������������
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
