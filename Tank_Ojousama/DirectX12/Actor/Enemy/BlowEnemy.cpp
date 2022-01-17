#include "BlowEnemy.h"
#include "../../ConstInfomation/Enemy/EnemyConstInfo.h"
#include "../../ConstInfomation/Enemy/BlowEnemyConstInfo.h"
#include"../../Scene/BaseScene.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"

namespace ECI = EnemyConstInfo;
namespace BECI = BlowEnemyConstInfo;

BlowEnemy::BlowEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
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

	//testActive = false;
}

BlowEnemy::~BlowEnemy()
{
	if (attackArea != nullptr)
	{
		attackArea->SetDeath(true);
	}
	
}

void BlowEnemy::EnemyInit()
{
#pragma region �ϐ��̏�����

	HP = 30;//���Ƃ���20
	warningTime = ECI::WARNING_TIME * BECI::WARNING_TIME * 60;
	attackTime = 30;//���Ƃ���60

	speed = 0.3f;
	mRadius = ECI::RADIUS * BECI::RADIUS;
	mSwingRange = ECI::SWING_RANGE * BECI::SWING_RANGE;
	mFireAngle = BECI::FAN_RANGE;
	mAttackLength = ECI::ATTACK_LENGTH * BECI::ATTACK_LENGTH;

	damage = 5;
	breadcrumbMode = ECI::BRRADCRUMB_MODE;
	DESTRUCT_MODE = ECI::DESTRUCT_MODE;
	death = false;
	TURNAROUND_MODE = true;
	RECEIVEREPORT_MODE = true;
	mRotDirection = false;
	onTrigger = false;
	mDeathAnimation = false;
	mDeadFlag = false;
	mDeathAnimation = false;
	mDeadFlag = false;

	scale = BECI::SCALE;
	mLegRotate = 0.0f;
	SetCollidder(Vector3().zero, mRadius);

	objType = ObjectType::ENEMY;
	mMoveState = MoveState::NOT_FIND;
	mDeathStep = DeathAnimationStep::RISE_SKY;

	//�Z���T�[�̏�����----------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//�ʒu
	fanInfo.fanRange = 200.0f;									   //�Ƃ̊p�x
	fanInfo.length = 50.0f;										   //����
	fanInfo.rotate = mFanRotateOrigin;							   //��]�p
	//--------------------------------

	////����|�C���g�̏�����------------------------------------
	//pointCount = sizeof(patrolPoint) / sizeof(patrolPoint[0]);
	//patrolPoint[0] = Vector3(position.x, position.y, position.z);
	//patrolPoint[1] = Vector3(position.x + 10, position.y, position.z);
	//patrolPoint[2] = Vector3(position.x + 10, position.y, position.z + 10);
	//patrolPoint[3] = Vector3(position.x, position.y, position.z + 10);
	////----------------------------------------------------------------
	/// 
	////�}�b�v�̃N���A
	//breadMap.clear();

	//�����蔻��I�u�W�F�N�g����
	attackArea = new AttackArea(position, angle, mManager, mRend, mOriginNumber);
	attackArea->SetActive(false);

	//�T�E���h������
	mAttackSE = std::make_shared<Sound>("SE/punti.mp3", true);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Elf_Damage02.mp3", true);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//�^�C�}�[������
	mRiseTime = std::make_shared<Timer>();
	mRiseTime->setTime(0.5f);
	mDeathTime = std::make_shared<Timer>();
	mDeathTime->setTime(0.5f);

	//�_���[�W�p�p�[�e�B�N��
	mDamageParticle = std::make_shared<Hit>(Vector3::zero, true);
	mDamageParticle->Stop();

	////���S�p�G�t�F�N�g
	//mDeathParticle = std::make_shared<Explosion>(Vector3::zero, true);
	//mDeathParticle->Stop();

#pragma endregion

#pragma region ���f���̓ǂݍ���

	//�ԍ���int����string�ɕϊ�
	mMyNumber = to_string(mOriginNumber);

	//�E�����o�^
	mRightLeg =  BECI::TANK_RLEG_NAME;
	mRLegNumber = mRightLeg + mMyNumber;
	mRend->AddModel(mRLegNumber, "Resouse/EnemyModel/SpearElf/yari_elf_R.obj", "Resouse/EnemyModel/SpearElf/leg_LR2.png");
	mRend->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�������o�^
	mLeftLeg = BECI::TANK_LREG_NAME;
	mLLegNumber = mLeftLeg + mMyNumber;
	mRend->AddModel(mLLegNumber, "Resouse/EnemyModel/SpearElf/yari_elf_L.obj", "Resouse/EnemyModel/SpearElf/leg_LR2.png");
	mRend->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//����o�^
	mHead = BECI::TANK_HEAD_NAME;
	mHeadNumber = mHead + mMyNumber;
	mRend->AddModel(mHeadNumber, "Resouse/EnemyModel/SpearElf/yari_elf_head.obj", "Resouse/EnemyModel/SpearElf/face_color3.png");

	//�̏��o�^
	mBody = BECI::TANK_BODY_NAME;
	mBodyNumber = mBody + mMyNumber;
	mRend->AddModel(mBodyNumber, "Resouse/EnemyModel/SpearElf/yari_elf_body.obj", "Resouse/EnemyModel/SpearElf/face_color.png");

	//����o�^
	mHand = "BlowHnad";
	mHandNumber = mHand + mMyNumber;
	mRend->AddModel(mHandNumber, "Resouse/EnemyModel/SpearElf/yari_te.obj", "Resouse/EnemyModel/SpearElf/yari_elf.png");

#pragma endregion
}

void BlowEnemy::EnemyUpdate()
{
	//if (!testActive) return;


	/*�����蔻��I�u�W�F�N�g�𐶐�*/
	CreateOneObject();

	Invincible(1);//���G����

	/*���S��ԊĎ�*/
	CheckAlive();

	//������ԂȂ珈�����Ȃ�
	if (mDeathAnimation) return;

	ChangeState(); //��ԕύX
	SearchObject();//�p��������v���C���[��T��

	/*�ړ�*/
	Move();

	/*�U��*/
	Attack();
}

void BlowEnemy::EnemyRend()
{

	//if (!testActive) return;

	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	//���f���̕`��
	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);
	mRend->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);
	mRend->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);
	mRend->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);
	mRend->Draw(mHandNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);
}

void BlowEnemy::EnemyOnCollision(BaseCollider * col)
{
	//if (!testActive) return;

	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
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

	/*if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		SetActive(true);
	}*/

	if (col->GetColObject()->GetType() == ObjectType::ENEMY)
	{
		//�����̔ԍ��������菬����������
		if (col->GetColObject()->GetID() > GetID())
		{
			position = mPreviousPosition;
		}
	}

	if (mAdvanceFlag && col->GetColObject()->GetType() == ObjectType::BORDER_LINE)
	{
		mHitBorderLine = true;
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}

	if (col->GetColObject()->GetType() == ObjectType::DEFENCEPOINT)
	{
		mHitSmokeFlag = true;
	}
}

void BlowEnemy::EnemyImGuiDebug()
{
#ifndef DEBUG

	//ImGui::SliderInt("HP", &HP, 0, 10);
	//ImGui::SliderInt("HP", &destructCount, 0, 1000);
	/*ImGui::Checkbox("Previous", &previousTri);*/

	//float ePos[3] = { fanInfo.position.x,fanInfo.position.y,fanInfo.position.z };
	//float ePos[3] = { angle.x,angle.y,angle.z };
	//ImGui::SliderFloat3("EnemyPosition", ePos, -500.0f, 500.0f);
	//ImGui::SliderFloat("HP", &barrelAngle, -1000.0f, 1000.0f);

#endif // DEBUG
}

void BlowEnemy::CheckAlive()
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

void BlowEnemy::Move()
{
	//�U�����͈ړ����Ȃ�
	if (mAttackFlag) return;

	/*�ړ� & �ǐ�*/
	TrackingObject();

	/*�ړ��̃A�j���[�V����*/
	MoveAnimation();
}

void BlowEnemy::MoveAnimation()
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

void BlowEnemy::Attack()
{
	if (mAttackFlag)
	{
		Vector3 areaPos = AngleToVectorY(fanInfo.rotate) * mAttackLength;
		attackArea->SetActive(true, position + areaPos, -angle);
		mAttackSE->setPos(position);
		mAttackSE->play();

		attackCount++;

		//���b���Ƃɓ����蔻��������̑O���ɕ\������
		if (attackCount > attackTime)
		{
			attackCount = 0;
			attackArea->SetActive(false);
			//�U�����I�������x����Ԃɖ߂�
			mAttackFlag = false;
			mMoveState = MoveState::NOT_FIND;
		}
	}
}

void BlowEnemy::DeathAnimation()
{
	//������ԂłȂ��@�Ȃ珈�����Ȃ�
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case BlowEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case BlowEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void BlowEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//SE����
	mDeathSE->setPos(position);
	mDeathSE->play();

	//���ԂɂȂ��Ă��Ȃ����
	if (!mRiseTime->isTime())
	{
		//��]
		mFireAngle += 50.0f;
		//�㏸
		position.y += 0.8f;
	}
	else
	{
		//���ԂɂȂ�����(1�t���[�������Ă΂��)

		//SE����
		mDamageSE->setPos(position);
		mDamageSE->play();

		//�p�[�e�B�N������
		mDamageParticle->setPos(position);
		mDamageParticle->Play();

		////�p�[�e�B�N������
		//mDeathParticle->setPos(position);
		//mDeathParticle->Play();

		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void BlowEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}

void BlowEnemy::CreateOneObject()
{
	//�����蔻��I�u�W�F�N�g����x��������
	if (!onTrigger)
	{
		mManager->Add(attackArea);
		attackArea->SetActive(false);
		onTrigger = true;
	}
}
