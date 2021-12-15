#include "SniperEnemy.h"
#include "../../Weapons/ElfBullet.h"
#include "../../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../../ConstInfomation/Enemy/SniperEnemyConstInfo.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"
#include"../../Scene/BaseScene.h"

namespace ECI = EnemyConstInfo;
namespace SECI = SniperEnemyConstInfo;

SniperEnemy::SniperEnemy(
	const Vector3& pos,
	const Vector3& ang,
	int num,
	bool advanceFlag,
	const Vector3& advanceDirection
)
{
	position = pos;
	angle = ang;
	mOriginNumber = num;
	mAdvanceFlag = advanceFlag;
	mAdvanceDirection = advanceDirection;
}

SniperEnemy::~SniperEnemy()
{
}

void SniperEnemy::EnemyInit()
{
	HP = ECI::MAX_HP * SECI::MAX_HP;
	warningTime = ECI::WARNING_TIME * SECI::WARNING_TIME * 60;
	attackTime = ECI::ATTACK_TIME * SECI::ATTACK_TIME * 60;

	speed = ECI::MOVE_SPEED * SECI::MOVE_SPEED;
	mRadius = ECI::RADIUS * SECI::RADIUS;
	mSwingRange = ECI::SWING_RANGE * SECI::SWING_RANGE;
	mFireAngle = SECI::FAN_RANGE;
	mAttackLength = ECI::ATTACK_LENGTH * SECI::ATTACK_LENGTH;

	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	DESTRUCT_MODE = ECI::DESTRUCT_MODE;
	death = false;
	mTrackingPlayer = false;
	mTrackingBreadcrumb = false;
	RECEIVEREPORT_MODE = true;
	TURNAROUND_MODE = true;
	mDeathAnimation = false;
	mDeadFlag = false;

	mLegRotate = 0.0f;

	SetActive(false);
	scale = SECI::SCALE;

	mMoveState = MoveState::NOT_FIND;
	objType = ObjectType::ENEMY;
	mDeathStep = DeathAnimationStep::RISE_SKY;
	SetCollidder(Vector3().zero, mRadius);

	//�Z���T�[�̏�����-------------------------------------------------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//�ʒu
	fanInfo.fanRange = 200.0f;									   //�Ƃ̊p�x
	fanInfo.length = 60.0f;										   //����
	fanInfo.rotate = mFanRotateOrigin;							   //��]�p
	//-----------------------------------------------------------------------

	////�}�b�v�̃N���A
	//breadMap.clear();

	//�T�E���h�̏�����
	mAttackSE = std::make_shared<Sound>("SE/hirai.mp3", true);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Elf_Damage01.mp3", true);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//�^�C�}�[�̏�����
	mRiseTime = std::make_shared<Timer>();
	mRiseTime->setTime(1.0f);
	mDeathTime = std::make_shared<Timer>();
	mDeathTime->setTime(1.0f);

	//�_���[�W�p�p�[�e�B�N��
	mDamageParticle = std::make_shared<Hit>(Vector3::zero, true);
	mDamageParticle->Stop();

	//���S�p�G�t�F�N�g
	mDeathParticle = std::make_shared<Explosion>(Vector3::zero, true);
	mDeathParticle->Stop();

#pragma region ���f���̓ǂݍ���

	//�ԍ���int����string�ɕϊ�
	mMyNumber = to_string(mOriginNumber);

	//�E�����o�^
	mRightLeg = SECI::TANK_RLEG_NAME;
	mRLegNumber = mRightLeg + mMyNumber;
	mRend->AddModel(mRLegNumber, "Resouse/EnemyModel/Elf_A/leg_R.obj", "Resouse/EnemyModel/Elf_A/leg_LR.png");
	mRend->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�������o�^
	mLeftLeg = SECI::TANK_LREG_NAME;
	mLLegNumber = mLeftLeg + mMyNumber;
	mRend->AddModel(mLLegNumber, "Resouse/EnemyModel/Elf_A/leg_L.obj", "Resouse/EnemyModel/Elf_A/leg_LR.png");
	mRend->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�����o�^
	mHead = SECI::TANK_HEAD_NAME;
	mHeadNumber = mHead + mMyNumber;
	mRend->AddModel(mHeadNumber, "Resouse/EnemyModel/Elf_A/elf_head.obj", "Resouse/EnemyModel/Elf_A/face_color.png");

	//�̏��o�^
	mBody = SECI::TANK_BODY_NAME;
	mBodyNumber = mBody + mMyNumber;
	mRend->AddModel(mBodyNumber, "Resouse/EnemyModel/Elf_A/elf_body.obj", "Resouse/EnemyModel/Elf_A/hand_bow_color.png");

#pragma endregion
}

void SniperEnemy::EnemyUpdate()
{
	Invincible(1);//���G����

	/*���S��ԊĎ�*/
	CheckAlive();

	//������ԂȂ珈�����Ȃ�
	if (mDeathAnimation) return;

	/*���ʂ̗v�f*/
	ChangeState(); //��ԕύX
	SearchObject();//�p��������v���C���[��T��

	/*�ړ�*/
	Move();

	/*�U��*/
	Attack();
}

void SniperEnemy::EnemyRend()
{
	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);//�E�r
	mRend->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);//���r
	mRend->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//���Ǝ�
	mRend->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//�̂Ƌ|	
}

void SniperEnemy::EnemyOnCollision(BaseCollider * col)
{
	/*if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}*/

	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		int test = col->GetColObject()->GetDamage();


		//�_���[�W���󂯂�
		HP -= col->GetColObject()->GetDamage();

		//SE����
		mDamageSE->setPos(position);
		mDamageSE->play();

		//�p�[�e�B�N������
		mDamageParticle->setPos(Vector3(position.x, position.y + 5.0f, position.z));
		mDamageParticle->Play();

		/*��*/
		InitSearch();
	}

	if (mAdvanceFlag && col->GetColObject()->GetType() == ObjectType::BORDER_LINE)
	{
		mHitBorderLine = true;
	}

	if (col->GetColObject()->GetType() == ObjectType::DEFENCEPOINT)
	{
		mHitSmokeFlag = true;
	}
}

void SniperEnemy::EnemyImGuiDebug()
{

}

void SniperEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		//�A�j���[�V����(�������)�ɂ���
		mDeathAnimation = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	/*���S�A�j���[�V�������J�n*/
	DeathAnimation();
}

void SniperEnemy::Move()
{
	//�U�����͈ړ����Ȃ�
	if (mAttackFlag) return;

	/*�ړ� & �ǐ�*/
	TrackingObject();

	/*�ړ��̃A�j���[�V����*/
	MoveAnimation();
}

void SniperEnemy::MoveAnimation()
{
	if (moveFlag)
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

void SniperEnemy::Attack()
{
	if (!mAttackFlag) return;
	attackCount++;

	if (attackCount > attackTime)
	{
		attackCount = 0;
		Vector3 firePos = AngleToVectorY(fanInfo.rotate);

		//�e�𔭎ˁI�I
		mManager->Add(new ElfBullet(position + firePos, Vector3(0.0, -angle.y, 0.0f), mManager, mRend, mPart, objType, bulletNumber));
		bulletNumber++;
		mAttackSE->setPos(position);
		mAttackSE->play();
		mAttackFlag = false;
		mMoveState = MoveState::NOT_FIND;
	}

}

void SniperEnemy::DeathAnimation()
{
	//������ԂłȂ��@�Ȃ珈�����Ȃ�
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case SniperEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case SniperEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void SniperEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//���ԂɂȂ��Ă��Ȃ����
	if (!mRiseTime->isTime())
	{
		//��]
		mFireAngle += 50.0f;
		//�㏸
		position.y += 0.5f;
	}
	else
	{
		//���ԂɂȂ�����(1�t���[�������Ă΂��)

		//�p�[�e�B�N������
		mDeathParticle->setPos(position);
		mDeathParticle->Play();
		//SE����
		mDeathSE->setPos(position);
		mDeathSE->play();

		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void SniperEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}
