#include "CEnemy.h"
#include "../../Weapons/ElfBullet.h"
#include"../../Scene/BaseScene.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"
#include "MemberEnemy.h"

CEnemy::CEnemy(
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
}

CEnemy::~CEnemy()
{
}

void CEnemy::EnemyInit()
{
	HP = 30;//���Ƃ���10
	attackTime = 60;
	mEnemyNumber = 0;
	damage = 0;
	speed = 0.35f;
	mRadius = 1.0f;
	mSwingRange = 45.0f;
	mAttackLength = 50.0f;
	mFireAngle = angle.y;

	death = false;
	breadcrumbMode = true;
	DESTRUCT_MODE = false;
	TURNAROUND_MODE = false;
	RECEIVEREPORT_MODE = false;
	mOnlyOnceTrigger = false;
	mSearchCommand = false;
	mSearchResult = false;
	mRotDirection = false;
	mDeathAnimation = false;
	mDeadFlag = false;

	mMemberList.resize(MEMBER_COUNT);
	mMemberPositionList.resize(MEMBER_COUNT);

	scale = Vector3(1.0f, 1.0f, 1.0f);
	mSearchPosition = Vector3().zero;
	SetCollidder(Vector3().zero, mRadius);

	objType = ObjectType::ENEMY;
	mMoveState = MoveState::NOT_FIND;
	mDeathStep = DeathAnimationStep::RISE_SKY;
	searchStep = SearchCommandStep::SEARCH_ORDER;

	//�Z���T�[�̏�����----------------
	mFanRotateOrigin = -angle.y - 90.0f;
	fanInfo.position = Vector3(position.x, position.y, position.z);//�ʒu
	fanInfo.fanRange = 360.0f;									   //�Ƃ̊p�x
	fanInfo.length = 50.0f;										   //����
	fanInfo.rotate = mFanRotateOrigin;							   //��]�p
	//--------------------------------

	//�T�E���h������
	mAttackSE = std::make_shared<Sound>("SE/hirai.mp3", true);

	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);

	mDeathSE = std::make_shared<Sound>("SE/Elf_Damage01.mp3", true);


	//�^�C�}�[������
	mSearchTimer = std::make_shared<Timer>();
	mSearchTimer->setTime(SEARCH_INTERVAL);
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

#pragma region ���f���̓ǂݍ���

	//�ԍ���int����string�ɕϊ�
	mMyNumber = to_string(mOriginNumber);

	//�E�����o�^
	mRightLeg =  "CaptainLegR";
	mRLegNumber = mRightLeg + mMyNumber;
	mRend->AddModel(mRLegNumber,  "Resouse/EnemyModel/Elf_C/leg_R_C.obj", "Resouse/EnemyModel/Elf_C/leg_LR2.png");
	mRend->SetAncPoint(mRLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�������o�^
	mLeftLeg = "CaptainLegL";
	mLLegNumber = mLeftLeg + mMyNumber;
	mRend->AddModel(mLLegNumber, "Resouse/EnemyModel/Elf_C/leg_L_C.obj", "Resouse/EnemyModel/Elf_C/leg_LR2.png");
	mRend->SetAncPoint(mLLegNumber, Vector3(0.0f, -2.0f, 0.0f));

	//�����o�^
	mHead = "CaptainHead";
	mHeadNumber = mHead + mMyNumber;
	mRend->AddModel(mHeadNumber, "Resouse/EnemyModel/Elf_C/elf_head3.obj", "Resouse/EnemyModel/Elf_C/face_color3.png");

	//�̏��o�^
	mBody = "CaptainBody";
	mBodyNumber = mBody + mMyNumber;
	mRend->AddModel(mBodyNumber, "Resouse/EnemyModel/Elf_C/elf_body3.obj", "Resouse/EnemyModel/Elf_C/hand_bow_color3.png");

#pragma endregion
}

void CEnemy::EnemyUpdate()
{
	/*�����𐶐�*/
	OnlyOnceAction();

	Invincible(1);//���G����

	/*������Ԃ��Ď�*/
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

	///*�ړ��w��*/
	//MoveOrder();

	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		mAnyDeathFlag = false;

		//������Ԃ��Ď�
		bool deadFlag = mMemberList[i]->GetDeadFlag();

		if (deadFlag)
		{
			mMemberList.erase(mMemberList.begin() + i);

			mAnyDeathFlag = true;
		}

		if (mAnyDeathFlag) break;

		/*�ړ��Ǘ�*/
		MemberMove(i);
	}

	///*���G�w��*/
	//SearchOrder();

	//���G�͈͓��Ƀv���C���[�����邩
	if (InsideDistance(mPlayerPosition, MEMBER_DISTANCE))
	{
		for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
		{
			mMemberList[i]->ReceiveAttackCommand(mPlayerPosition, true);
		}

		mSearchCommand = false;
		mSearchResult = false;
	}
	else//�����̍��G�͈͓��Ƀv���C���[�����Ȃ��Ƃ�
	{
		//���G�w�߂��o�Ă��Ȃ���
		if (!mSearchCommand)
		{
			//2�b��1����G�w�߂��o��
			mSearchTimer->update();

			if (mSearchTimer->isTime())
			{
				//���G�w�߂��o��
				for (const auto& list : mMemberList)
				{
					list->ReceiveSearchCommand(mPlayerPosition, true);
				}

				mSearchTimer->setTime(SEARCH_INTERVAL);
				mSearchCommand = true;
			}
		}

		//���G�w�߂��o�Ă���
		if (mSearchCommand)
		{
			for (const auto& list : mMemberList)
			{
				//���G���ʂ��󂯎��
				mSearchResult = list->SendSearchResult();
				//�Ώۂ𔭌����Ă�����
				if (mSearchResult)
				{
					//�Ώۂ̈ʒu����x�����擾
					mSearchPosition = list->SendSearchPosition();
					break;
				}
			}

			//�S���̕񍐂��󂯎�� or �N��1�l���Ώۂ𔭌����Ă�����񍐏I��
			mSearchCommand = false;
		}

		//�Ώۂ𔭌����Ă�����
		if (mSearchResult)
		{
			//�߂��Ȃ����猳�ɖ߂�
			if (InsideDistance(mSearchPosition, 1.0f))
			{
				mSearchResult = false;
			}
			else
			{
				//�Ώۂ̈ʒu�܂ňړ�
				MovePoint(mSearchPosition);
			}
		}
	}
}

void CEnemy::EnemyRend()
{
	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	//���f���̕`��
	DirectXManager::GetInstance()->SetData3D();
	mRend->Draw(mRLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(+mLegRotate, mFireAngle, 0), scale);//�E�r
	mRend->Draw(mLLegNumber, Vector3(position.x, position.y + 2.0f, position.z), Vector3(-mLegRotate, mFireAngle, 0), scale);//���r
	mRend->Draw(mHeadNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//���Ǝ�
	mRend->Draw(mBodyNumber, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), scale);//�̂Ƌ|	
}

void CEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//SE����
		mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
		mDamageSE->setPos(position);
		mDamageSE->play();

		//�p�[�e�B�N������
		mDamageParticle->setPos(Vector3(position.x, position.y + 5.0f, position.z));
		mDamageParticle->Play();

		//�_���[�W���󂯂�
		HP -= col->GetColObject()->GetDamage();
	}

	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
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

void CEnemy::EnemyImGuiDebug()
{
}

void CEnemy::OnlyOnceAction()
{
	//���łɎ��s���Ă����� �������Ȃ�
	if (mOnlyOnceTrigger) return;

	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		Vector3 pos = position + AngleToVectorY(360.0f / mMemberList.size() * i) * MEMBER_DISTANCE;
		auto enemys = new MemberEnemy(pos, angle, mManager, mRend, mPart, mEnemyNumber++);
		mMemberList[i] = enemys;
		mManager->Add(enemys);
	}

	//�������I�������t���O�����Ă�
	mOnlyOnceTrigger = true;
}

void CEnemy::MemberMove(int number)
{
	//���ꂽ�ʒu�Ƀ|�W�V�������w��
	mMemberPositionList[number] = position + AngleToVectorY(360.0f / mMemberList.size() * number) * MEMBER_DISTANCE;

	//�����Ɉʒu����n���B
	mMemberList[number]->ReceivePosition(mMemberPositionList[number]);
}

void CEnemy::MoveOrder()
{
	for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
	{
		mAnyDeathFlag = false;

		//������Ԃ��Ď�
		bool deadFlag = mMemberList[i]->GetDeadFlag();

		if (deadFlag)
		{
			mMemberList.erase(mMemberList.begin() + i);

			mAnyDeathFlag = true;
		}

		if (mAnyDeathFlag) break;

		/*�ړ��Ǘ�*/
		MemberMove(i);
	}
}

void CEnemy::SearchOrder()
{
	//���G�͈͓��Ƀv���C���[�����邩
	if (InsideDistance(mPlayerPosition, MEMBER_DISTANCE))
	{
		for (int i = 0, end = static_cast<int>(mMemberList.size()); i < end; i++)
		{
			mMemberList[i]->ReceiveAttackCommand(mPlayerPosition, true);
		}

		mSearchCommand = false;
		mSearchResult = false;
	}
	else//�����̍��G�͈͓��Ƀv���C���[�����Ȃ��Ƃ�
	{
		//���G�w�߂��o�Ă��Ȃ���
		if (!mSearchCommand)
		{
			//2�b��1����G�w�߂��o��
			mSearchTimer->update();

			if (mSearchTimer->isTime())
			{
				//���G�w�߂��o��
				for (const auto& list : mMemberList)
				{
					list->ReceiveSearchCommand(mPlayerPosition, true);
				}

				mSearchTimer->setTime(SEARCH_INTERVAL);
				mSearchCommand = true;
			}
		}

		//���G�w�߂��o�Ă���
		if (mSearchCommand)
		{
			for (const auto& list : mMemberList)
			{
				//���G���ʂ��󂯎��
				mSearchResult = list->SendSearchResult();
				//�Ώۂ𔭌����Ă�����
				if (mSearchResult)
				{
					//�Ώۂ̈ʒu����x�����擾
					mSearchPosition = list->SendSearchPosition();
					break;
				}
			}

			//�S���̕񍐂��󂯎�� or �N��1�l���Ώۂ𔭌����Ă�����񍐏I��
			mSearchCommand = false;
		}

		//�Ώۂ𔭌����Ă�����
		if (mSearchResult)
		{
			//�߂��Ȃ����猳�ɖ߂�
			if (InsideDistance(mSearchPosition, 1.0f))
			{
				mSearchResult = false;
			}
			else
			{
				//�Ώۂ̈ʒu�܂ňړ�
				MovePoint(mSearchPosition);
			}
		}
	}
}

void CEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		mDeathAnimation = true;
		for (auto& list : mMemberList)
		{
			list->ReceiveCaptainLost(true);
		}
	}

	if (mDeadFlag)
	{
		death = true;
	}

	/*���S�A�j���[�V�������J�n*/
	DeathAnimation();
}

void CEnemy::Move()
{
	//�U�����͈ړ����Ȃ�
	if (mAttackFlag) return;

	/*�ړ� & �ǐ�*/
	TrackingObject();

	/*�ړ��̃A�j���[�V����*/
	MoveAnimation();
}

void CEnemy::MoveAnimation()
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

void CEnemy::Attack()
{
	if (!mAttackFlag) return;

	attackCount++;

	if (attackCount > attackTime)
	{
		attackCount = 0;
		Vector3 firePos = AngleToVectorY(fanInfo.rotate);

		Vector3 pos = Vector3(position.x, position.y + 1.5f, position.z);

		//�e�𔭎ˁI�I
		mManager->Add(new ElfBullet(pos + firePos, Vector3(0, -angle.y, 0), mManager, mRend, mPart, objType, bulletNumber));
		bulletNumber++;
		mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
		mAttackSE->setPos(position);
		mAttackSE->play();
		mAttackFlag = false;
		mMoveState = MoveState::NOT_FIND;
	}
	else
	{
		////�v���C���[�������B
		//FacingPlayer();
	}
}

void CEnemy::DeathAnimation()
{
	//������ԂłȂ��@�Ȃ珈�����Ȃ�
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case CEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case CEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void CEnemy::DeathAnimeStep_RiseSky()
{
	mRiseTime->update();

	//SE����
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
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
		mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
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

void CEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
	}
}
