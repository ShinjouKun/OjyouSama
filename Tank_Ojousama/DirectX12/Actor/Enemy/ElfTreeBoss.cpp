#include "ElfTreeBoss.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/LaunchBullet.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Utility/Random.h"
#include "../../Sound/Sound.h"
#include "../../Scene/BaseScene.h"
#include "SummonEnemy.h"
#include "TreeRoot.h"

ElfTreeBoss::ElfTreeBoss(
	const Vector3 & pos,
	const Vector3 & ang,
	shared_ptr<ParticleManager> effectManager,
	int num
)
{
	position = pos;
	angle = ang;
	mEffectManager = effectManager;
	number = num;
}

ElfTreeBoss::~ElfTreeBoss()
{
	
}

bool ElfTreeBoss::GetDeadFlag()
{
	return mDeadFlag;
}

void ElfTreeBoss::ChangeAttackState()
{
	if (mDeathAnimationFlag) return;

	Vector3 dist = mPlayerPosition - position;
	float length = dist.Length();

	//���G�͈͓��ɂ����
	if (length < SEARCH_LENGTH)
	{
		if (!mActionFlag)
		{
			Random::initialize();
			/*0�`10�ōs�������߂�*/
			mActionCount = Random::randomRange(0, 11);
			mActionFlag = true;
		}

		//0�`5�̎�
		if (mActionCount >= 0 && mActionCount <= 5)
		{
			RapidFire();//�A���ˌ�
		}
		//6�`9
		else if (mActionCount >= 6 && mActionCount <= 9)
		{
			RootAttack();//�������U��
		}
		else if (mActionCount >= 10 && mActionCount <= 11)
		{
			Summon();//�����U��
		}
	}
}

void ElfTreeBoss::RapidFire()
{
	if (!mActionFlag) return;

	//�_����
	if (!mShotBullet)
	{
		mAimingTime->update();

		//�ł���΁A�_�������v���C���[�̕����݂Ă��Ăق���
		Vector3 mTargetPosition = mPlayerPosition - position;
		mTargetPosition = mTargetPosition.normal();
		float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
		mFireAngle = -Math::toDegrees(radian) - 180.0f;
		angle.y = mFireAngle;

		if (mAimingTime->isTime())
		{
			mShotBullet = true;
			mAimingTime->setTime(AIMING_TIME);
		}
	}

	//����
	if (mShotBullet)
	{
		mReloadTime->update();

		if (mReloadTime->isTime())
		{
			mReloadTime->setTime(RELOAD_TIME);

			Vector3 test = position + FIRE_POSITION;

			Vector3 mTargetPosition = mPlayerPosition - test;
			mTargetPosition = mTargetPosition.normal();
			float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
			mFireAngle = -Math::toDegrees(radian) - 180.0f;
			angle.y = mFireAngle;

			mManager->Add(new LaunchBullet(position, mPlayerPosition, mManager, mRend, mEffectManager, objType, mBulletCount++,true));

			//�w�萔�A���Ŏˌ�����
			if (mBulletCount >= RAPIDFIRE_COUNT)
			{
				mBulletCount = 0;
				mShotBullet = false;

				//�s���I���Ƃ���
				mActionFlag = false;
			}
		}
	}
}

void ElfTreeBoss::Summon()
{
	if (!mActionFlag) return;

	//�������Ă΂ꂽ��1�񂵂��������Ȃ�

	//�͂𗭂߂Ă�����
	if (!mSummonEnemy)
	{
		mSummonTime->update();

		//�x����\������
		mDrawSummonPoint = true;

		if (mSummonTime->isTime())
		{
			mSummonTime->setTime(SUMMON_TIME);
			mSummonEnemy = true;
		}
	}

	//�����J�n�I
	if (mSummonEnemy)
	{
		//�`��I��
		mDrawSummonPoint = false;

		//�ŏ��ɗp�ӂ����ʒu�ɁA�G�𐶐�
		for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
		{
			mManager->Add(new SummonEnemy(mSummonPoint[i], angle, mManager, mRend, mEffectManager, mEnemyNumber++));
		}

		//�������I�������t���O��߂�
		mSummonEnemy = false;

		//�s���I���Ƃ���
		mActionFlag = false;
	}
}

void ElfTreeBoss::DeathAnimation()
{
	//������� �łȂ��Ȃ珈�����Ȃ�
	if(!mDeathAnimationFlag) return;

	switch (mDeathStep)
	{
	case ElfTreeBoss::EXPLOSION:

		mExplosionTime->update();

		//��]
		angle.y += 20.0f;

		if (!mExplosionTime->isTime())
		{
			mEffectInterval->update();

			if (mEffectInterval->isTime())
			{
				//�p�[�e�B�N����`��
				Random::initialize();
				//�`��ʒu�𖈉�ς���
				float x = Random::randomRange(position.x - 10.0f, position.x +10.0f);
				float y = Random::randomRange(0.0f, position.y + 40.0f);
				float z = Random::randomRange(position.z - 10.0f, position.z + 10.0f);
				//mParticleEmitter->EmitterUpdate(PARTICLE_EFFECT, Vector3(x, y, z), angle);
				mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, Vector3(x, y, z), angle);

				/*�ł����SE���~�����˂��邳���Ȃ����x��*/
				mSmallExplosion->play();

				mEffectInterval->setTime(0.1f);
			}
		}
		else
		{
			mExplosionTime->setTime(2.0f);//�������I����ď����āA���S����܂ł̎���

			/*�����łł������������N���Ăق���*/
			mBigExplosion->play();

			for (int i = 0; i < 10; i++)
			{
				//�p�[�e�B�N����`��
				Random::initialize();
				float x = Random::randomRange(position.x -10.0f, position.x+ 10.0f);
				float y = Random::randomRange(0.0f, position.y + 40.0f);
				float z = Random::randomRange(position.z -10.0f, position.z + 10.0f);
				mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, Vector3(x, y, z), angle);
			}

			mDeathStep = DeathAnimationStep::DEATH_COUNT;
		}

		break;
	case ElfTreeBoss::DEATH_COUNT:

		mExplosionTime->update();

		if (mExplosionTime->isTime())
		{
			mDeadFlag = true;
		}

		break;
	default:
		break;
	}
}

void ElfTreeBoss::RootAttack()
{
	if (!mActionFlag) return;

	switch (mRootStep)
	{
	case ElfTreeBoss::PIERCE_HAND:
		RootAtack_PierceHand();
		break;
	case ElfTreeBoss::CHASE_PLAYER:
		RootAtack_ChasePlayer();
		break;
	case ElfTreeBoss::WAIT:
		RootAtack_Wait();
		break;
	case ElfTreeBoss::GOUP_ROOT:
		RootAtack_GoupRoot();
		break;
	case ElfTreeBoss::GODOWN_ROOT:
		RootAtack_GodownRoot();
		break;
	default:
		break;
	}
}

void ElfTreeBoss::RootAtack_PierceHand()
{
	mHandAngle = 90.0f;

	//�n�ʂɎ���Ԃ�����
	mRightHandPos.y--;
	mLeftHandPos.y--;

	if (mRightHandPos.y <= 8.0f && mLeftHandPos.y <= 8.0f)
	{
		mRootStep = RootAttackStep::CHASE_PLAYER;
	}
}

void ElfTreeBoss::RootAtack_ChasePlayer()
{
	//�x�����v���C���[��ǂ�������

	//Y�̈ړ����Ȃ��Čv�Z����
	Vector3 holdTarget = Vector3(mPlayerPosition.x, 0.0f, mPlayerPosition.z);
	Vector3 holdPosition = Vector3(mRootPosition.x, 0.0f, mRootPosition.z);
	Vector3 dist = holdTarget - holdPosition;
	float length = dist.Length();
	dist = dist.normal();

	//���G�͈͓��ɂ����
	if (length < SEARCH_LENGTH)
	{
		velocity = dist * mRootSpeed;
		mRootPosition += velocity;

		mTreeRoot->SetPosition(mRootPosition);

		//�������̓�����͈͓��ɂ����
		if (length < 1.0f)
		{

			mRootStep = RootAttackStep::WAIT;
		}
	}
	else
	{
		//���Ȃ�������A���̏�ɍ��������o��
		mRootStep = RootAttackStep::GOUP_ROOT;
	}
}

void ElfTreeBoss::RootAtack_Wait()
{
	mRootWaitTime->update();

	//��莞�ԑ҂�
	if (mRootWaitTime->isTime())
	{
		mRootWaitTime->setTime(1.0f);
		mRootStep = RootAttackStep::GOUP_ROOT;
	}
}

void ElfTreeBoss::RootAtack_GoupRoot()
{
	if (mRootPosition.y >= 0.0f)
	{
		mRootWaitTime->update();

		//��莞�ԑ҂�
		if (mRootWaitTime->isTime())
		{
			mRootWaitTime->setTime(0.5f);
			mRootStep = RootAttackStep::GODOWN_ROOT;
		}
	}
	else
	{
		//����������ɏグ��B
		mRootPosition.y += 2.0f;

		mTreeRoot->SetPosition(mRootPosition);
	}
}

void ElfTreeBoss::RootAtack_GodownRoot()
{
	//�E��ƍ�������ɖ߂�
	if (mRightHandPos.y >= mOffsetRightHand.y && mLeftHandPos.y >= mOffsetLeftHand.y)
	{
		mRightHandPos = mOffsetRightHand;
		mLeftHandPos = mOffsetLeftHand;

		mFinishHandReturn = true;
	}
	else
	{
		//������̈ʒu�ɖ߂�
		mRightHandPos.y++;
		mLeftHandPos.y++;
	}

	//�����������ɖ߂�
	if (mRootPosition.y <= mOffsetRootPos.y)
	{
		mRootPosition = mOffsetRootPos;
		mFinishRootReturn = true;
	}
	else
	{
		//�����������ɂ��낷
		mRootPosition.y -= 0.5f;
		mTreeRoot->SetPosition(mRootPosition);
	}

	//���r�ƍ����������̈ʒu�ɖ߂�����
	if (mFinishHandReturn && mFinishRootReturn)
	{
		mHandAngle = 0.0f;
		mFinishHandReturn = mFinishRootReturn = false;
		//�s���I���Ƃ���
		mActionFlag = false;
		mRootStep = RootAttackStep::PIERCE_HAND;
	}
}

void ElfTreeBoss::CreateObject()
{
	if (mCreateObject) return;

	mManager->Add(mTreeRoot);

	mCreateObject = true;
}

void ElfTreeBoss::EnemyInit()
{
	HP = MAX_HP;
	speed = MOVE_SPEED;
	objType = ObjectType::ENEMY;
	mRadius = 3.0f;

	damage = 5;
	mBulletCount = 0;
	mEnemyNumber = 0;
	mCircleCount = 0;
	mActionCount = 0;

	mRootSpeed = 1.5f;
	mSummonRotate = 0.0f;
	mHandAngle = 0.0f;

	death = false;
	mShotBullet = false;
	mSummonEnemy = false;
	mDrawSummonPoint = false;
	mCreateObject = false;
	mFinishHandReturn = false;
	mFinishRootReturn = false;

	mActionFlag = false;
	mDeathAnimationFlag = false;
	mDeadFlag = false;

	//�e�I�u�W�F�N�g�̏����ʒu���Z�b�g
	mOffsetRightHand = Vector3(position.x /*- 10.0f*/, position.y + 15.0f, position.z);
	mOffsetLeftHand = Vector3(position.x  /*+ 10.0f*/, position.y + 15.0f, position.z);
	mOffsetRootPos = Vector3(position.x, position.y - 20.0f, position.z);
	mRightHandPos = mOffsetRightHand;
	mLeftHandPos = mOffsetLeftHand;
	mRootPosition = mOffsetRootPos;

	//�������I�u�W�F�N�g����
	mTreeRoot = new TreeRoot(mRootPosition, angle, mManager, mRend, number);

	//�T�E���h�̐ݒ�
	mSmallExplosion = std::make_shared<Sound>("Small_Explosion.wav", false);
	mBigExplosion = std::make_shared<Sound>("Big_Explosion.mp3", false);

	//�e��^�C�}�[������
	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(AIMING_TIME);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(RELOAD_TIME);
	mSummonTime = std::make_shared<Timer>();
	mSummonTime->setTime(SUMMON_TIME);
	mRootWaitTime = std::make_shared<Timer>();
	mRootWaitTime->setTime(0.5f);
	mEffectInterval = std::make_shared<Timer>();
	mEffectInterval->setTime(0.5f);
	mExplosionTime = std::make_shared<Timer>();
	mExplosionTime->setTime(3.0f);

	//�������U����Ԃ�������
	mRootStep = RootAttackStep::PIERCE_HAND;
	//���S�A�j���[�V������Ԃ̏�����
	mDeathStep = DeathAnimationStep::EXPLOSION;

	//�l�p�̓����蔻��(�g�����������)
	//SetCollidder(Vector3(-10.0f, -10.0f, -10.0f), Vector3(10.0f, 3.0f, 10.0f));
	//SetCollidder(new AABBCollider(position, Vector3(10.0f, 10.0f, 10.0f)));

	/*�����蔻��𑝂₹�Ȃ��̂ŁA��̕����̂ݓ�����悤�ɂ���*/
	SetCollidder(Vector3(0.0f, 15.0f, 0.0f), mRadius);

	///*memo : �l�p�`�̓����蔻�肪�g���Ȃ��̂ŁA�~�̓����蔻��𑝂₵�܂���*/
	//for (int i = 0; i < 10; i++)
	//{
	//	SetCollidder(Vector3(0.0f, mRadius * i, 0.0f), mRadius);
	//}


#pragma region ���f���ǂݍ��� + ��

	mStringNum = to_string(number);

	//����
	mModelNumName01 = mModelName01 + mStringNum;
	mRend->AddModel(mModelNumName01, "Resouse/EnemyModel/TreeBoss/wood_body.obj", "Resouse/EnemyModel/TreeBoss/wood_body.png");

	//�E��
	mHandRightNum = mHandRight + mStringNum;
	mRend->AddModel(mHandRightNum, "Resouse/EnemyModel/TreeBoss/wood_hands_R.obj", "Resouse/EnemyModel/TreeBoss/wood_hand.png");
	mRend->SetAncPoint(mHandRightNum, Vector3(0.0f, -3.0f, 0.0f));

	//����
	mHandLeftNum = mHandLeft + mStringNum;
	mRend->AddModel(mHandLeftNum, "Resouse/EnemyModel/TreeBoss/wood_hands_L.obj", "Resouse/EnemyModel/TreeBoss/wood_hand.png");
	mRend->SetAncPoint(mHandLeftNum, Vector3(0.0f, -3.0f, 0.0f));

	//�������U���p�x��
	mRootCircleNum = mRootCircle + mStringNum;
	mRend->AddModel(mRootCircleNum, "Resouse/maru.obj", "Resouse/marui.png");
	mRend->SetColor(mRootCircleNum, Vector4(1, 0, 0, 0.2f));

	//�p�[�e�B�N��1
	mParticleEmitter = make_shared<ParticleEmitterBox>(mEffectManager);
	mParticleEmitter->LoadAndSet(PARTICLE_EFFECT, "Resouse/effect.png");
	mParticleEmitter->LoadAndSet(EXPLOSION_EFFECT, "Resouse/Bom.jpg");

	//�z��̏�����
	mSummonPoint.resize(SUMMON_COUNT);
	testNum.resize(SUMMON_COUNT);
	testNumName.resize(SUMMON_COUNT);
	for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
	{
		mSummonPoint[i] = position + AngleToVectorY(360.0f / end * i) * 30.0f;

		//�������鐔���A�����ꏊ�W���p�I�u�W�F�N�g�𐶐�����B
		testNum[i] = to_string(i);
		testNumName[i] = mSummonCircle + testNum[i];
		mRend->AddModel(testNumName[i], "Resouse/maru.obj", "Resouse/marui.png");
	}

#pragma endregion
}

void ElfTreeBoss::EnemyUpdate()
{
	if (HP <= 0)
	{
		mDeathAnimationFlag = true;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	//�v���C���[�̈ʒu���擾�I
	mPlayerPosition = mManager->GetPlayer().GetPosition();

	//���ʂ̒���
	mSmallExplosion->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
	mBigExplosion->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);

	/*��x�����I�u�W�F�N�g�𐶐�*/
	CreateObject();

	/*���S�A�j���[�V����*/
	DeathAnimation();

	///*�U��*/
	//ChangeAttackState();
}

void ElfTreeBoss::EnemyRend()
{
	//������ԂłȂ�(������Ԃ�������)�Ȃ�`�悵�Ȃ�
	if (mDeathStep != DeathAnimationStep::EXPLOSION) return;

	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g
	mRend->Draw(mModelNumName01, position, Vector3(angle.x, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));
	mRend->Draw(mHandRightNum, mRightHandPos, Vector3(mHandAngle, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));
	mRend->Draw(mHandLeftNum, mLeftHandPos, Vector3(mHandAngle, angle.y, angle.z), Vector3(5.0f, 5.0f, 5.0f));

	//���������v���C���[��ǐՂ��Ă��鎞�̂ݕ\��
	if (mRootStep == RootAttackStep::CHASE_PLAYER ||
		mRootStep == RootAttackStep::WAIT)
	{
		mRend->Draw(mRootCircleNum, Vector3(mRootPosition.x, position.y - 3, mRootPosition.z), angle, Vector3(5, 5, 5));
	}

	//�\����Ԃ̎������A�x����\��
	if (mDrawSummonPoint)
	{
		for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
		{
			mRend->Draw(testNumName[i], mSummonPoint[i], Vector3(0.0f, mSummonRotate += 10.0f, 0.0f), Vector3(2, 2, 2));
		}
	}
}

void ElfTreeBoss::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
	}
}

void ElfTreeBoss::EnemyImGuiDebug()
{
}

void ElfTreeBoss::Search()
{
}

void ElfTreeBoss::Warning()
{
}

void ElfTreeBoss::Attack()
{
}
