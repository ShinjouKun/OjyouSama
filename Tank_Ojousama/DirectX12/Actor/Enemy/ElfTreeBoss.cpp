#include "ElfTreeBoss.h"
#include "../../Collision/SpherCollider.h"
#include "../../Weapons/LaunchBullet.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Utility/Random.h"
#include "../../Sound/Sound.h"
#include "../../Scene/BaseScene.h"
#include "SummonEnemy.h"
#include "TreeRoot.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"

ElfTreeBoss::ElfTreeBoss(
	const Vector3 & pos,
	const Vector3 & ang,
	shared_ptr<ParticleManager> effectManager,
	int num
)
{
	position = pos;
	angle = ang;
	//mEffectManager = effectManager;
	number = num;
}

ElfTreeBoss::~ElfTreeBoss()
{

}

void ElfTreeBoss::EndCameraAnimation(bool value)
{
	mEndAnimation = value;
}

bool ElfTreeBoss::GetDeadFlag()
{
	return mDeadFlag;
}

void ElfTreeBoss::ChangeAttackState()
{
	//return;

	//�V�[���A�j���[�V������ | ���S�A�j���[�V�������͏������Ȃ�
	if (!mEndAnimation || mDeathAnimationFlag) return;

	mPlayerPosition = mManager->GetPlayer().GetPosition();

	Vector3 dist = mPlayerPosition - position;
	float length = dist.Length();

	//���G�͈͓��ɂ����
	if (length < SEARCH_LENGTH)
	{
		//�{���Ԃ̎�
		if (mAngryFlag)
		{
			if (!mActionFlag)
			{
				Random::initialize();
				/*0�`10�ōs�������߂�*/
				mActionCount = Random::randomRange(0, 13);
				mActionFlag = true;
			}

			//0�`5�̎�
			if (mActionCount >= 0 && mActionCount <= 5)
			{
				//RapidFire();//�A���ˌ�
				ManyAppleDrop();
			}
			//6�`9
			else if (mActionCount >= 6 && mActionCount <= 9)
			{
				RootAttack();//�������U��
			}
			else if (mActionCount >= 10 && mActionCount <= 13)
			{
				Summon();//�����U��
			}
		}
		else
		{
			if (!mActionFlag)
			{
				Random::initialize();
				/*0�`10�ōs�������߂�*/
				mActionCount = Random::randomRange(0, 13);
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
			else if (mActionCount >= 10 && mActionCount <= 13)
			{
				Summon();//�����U��
			}
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

			mManager->Add(new LaunchBullet(test, mPlayerPosition, mManager, mRend, mPart, objType, mBulletCount++, true));

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

	//�G�����łɑ��݂��Ă���Ƃ�
	if (mSummonAlive)
	{
		//�s���I���Ƃ���
		mActionFlag = false;

		return;
	}

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
			//mManager->Add(new SummonEnemy(mSummonPoint[i], angle, mManager, mRend, mPart, mEnemyNumber++));

			auto enemys = new SummonEnemy(mSummonPoint[i], angle, mManager, mRend, mPart, mEnemyNumber++);
			mSummonList.push_back(enemys);
			mManager->Add(enemys);
		}

		//�������I�������t���O��߂�
		mSummonEnemy = false;

		//�s���I���Ƃ���
		mActionFlag = false;
	}
}

void ElfTreeBoss::ManyAppleDrop()
{
	mAppleDropTime->update();

	if (!mAppleDropTime->isTime())
	{
		mAppleInterval->update();

		if (mAppleInterval->isTime())
		{
			Random::initialize();
			//�`��ʒu�𖈉�ς���
			float x = Random::randomRange(position.x - 80.0f, position.x + 80.0f);
			float y = position.y + 100.0f;
			float z = Random::randomRange(position.z - 80.0f, position.z + 80.0f);

			Vector3 test = Vector3(x, y - 100.0f, z);

			Vector3 mTargetPosition = mPlayerPosition - test;
			//mTargetPosition = mTargetPosition.normal();
			//float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
			//mFireAngle = -Math::toDegrees(radian) - 180.0f;
			//angle.y = mFireAngle;

			mManager->Add(new LaunchBullet(Vector3(x, y, z), test, mManager, mRend, mPart, objType, mBulletCount++, true));

			mAppleInterval->setTime(0.1f);
		}


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

			mManager->Add(new LaunchBullet(test, mPlayerPosition, mManager, mRend, mPart, objType, mBulletCount++, true));
		}
	}

	//���ԂɂȂ�����
	if (mAppleDropTime->isTime())
	{
		//�^�C�}�[�����Z�b�g����
		mAppleDropTime->setTime(1.0f);

		//�s���I���Ƃ���
		mActionFlag = false;
	}
}

void ElfTreeBoss::DeathAnimation()
{
	//������� �łȂ��Ȃ珈�����Ȃ�
	if (!mDeathAnimationFlag) return;

	if (mDeathStep == DeathAnimationStep::EXPLOSION)
	{
		DeathAnimation_Explosion();
	}
	else if (mDeathStep == DeathAnimationStep::DEATH_COUNT)
	{
		DeathAnimation_DeathCount();
	}
}

void ElfTreeBoss::DeathAnimation_Explosion()
{
	mExplosionTime->update();

	//��]
	angle.y += 20.0f;

	if (!mExplosionTime->isTime())
	{

		mEffectInterval->update();

		if (!mOneShotSound)
		{
			/*��x�����炵������*/
			mSmallExplosion->setPos(position);
			mSmallExplosion->play();
			mOneShotSound = true;
		}


		if (mEffectInterval->isTime())
		{
			//�p�[�e�B�N����`��
			Random::initialize();
			//�`��ʒu�𖈉�ς���
			float x = Random::randomRange(position.x - 20.0f, position.x + 20.0f);
			float y = Random::randomRange(0.0f, position.y + 30.0f);
			float z = Random::randomRange(position.z - 20.0f, position.z + 20.0f);
			//mParticleEmitter->EmitterUpdate(PARTICLE_EFFECT, Vector3(x, y, z), angle);
			//mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, Vector3(x, y, z), angle);

			mDamageParticle = std::make_shared<Hit>(Vector3(x, y, z), false);

			//mDamageParticle->Stop();
			//mDamageParticle->setPos(Vector3(x, y, z));
			//mDamageParticle->Play();

			//mDeathParticle = std::make_shared<Explosion>(Vector3(x, y, z), false);

			///*�ł����SE���~�����˂��邳���Ȃ����x��*/
			//mSmallExplosion->play();

			mEffectInterval->setTime(0.2f);
		}
	}

	if (mExplosionTime->isTime())
	{

		mExplosionTime->setTime(2.0f);//�������I����ď����āA���S����܂ł̎���

		///*�����łł������������N���Ăق���*/
		//mBigExplosion->setPos(position);
		//mBigExplosion->play();
		mDeathSE->setPos(position);
		mDeathSE->play();

		for (int i = 0; i < 5; i++)
		{
			//�p�[�e�B�N����`��
			Random::initialize();
			float x = Random::randomRange(position.x - 5.0f, position.x + 5.0f);
			float y = Random::randomRange(0.0f, position.y + 20.0f);
			float z = Random::randomRange(position.z - 5.0f, position.z + 5.0f);
			//mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, Vector3(x, y, z), angle);

			//mDeathParticle = std::make_shared<Hit>(Vector3(x, y, z), false);

			mDeathParticle = std::make_shared<Explosion>(Vector3(x, y, z), false);

			//mDeathParticle->setPos(Vector3(x, y, z));
			//mDeathParticle->Play();
		}

		mDeathStep = DeathAnimationStep::DEATH_COUNT;
	}
}

void ElfTreeBoss::DeathAnimation_DeathCount()
{
	mExplosionTime->update();

	if (mExplosionTime->isTime())
	{

		mDeadFlag = true;
	}
}

void ElfTreeBoss::RootAttack()
{
	if (!mActionFlag) return;

	//��]
	Vector3 mTargetPosition = mPlayerPosition - position;
	mTargetPosition = mTargetPosition.normal();
	float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
	mFireAngle = -Math::toDegrees(radian) - 180.0f;
	angle.y = mFireAngle;

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

		if (mAngryFlag)
		{
			mTreeRoot2->SetPosition(mRootPosition);
			mTreeRoot3->SetPosition(mRootPosition);
		}

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

		Vector3 mRightPos = AngleToVectorY(mFireAngle) * 10.0f;


		mTreeRoot->SetPosition(mRootPosition);
		mTreeRoot->SetAngle(angle);

		if (mAngryFlag)
		{
			mTreeRoot2->SetPosition(mRootPosition + mRightPos);
			mTreeRoot3->SetPosition(mRootPosition - mRightPos);
			mTreeRoot2->SetAngle(angle);
			mTreeRoot3->SetAngle(angle);
		}


		//if (mAngryFlag)
		//{
		//	mTreeRoot2->SetPosition(Vector3(mRootPosition.x + 10, mRootPosition.y, mRootPosition.z));
		//	mTreeRoot3->SetPosition(Vector3(mRootPosition.x - 10, mRootPosition.y, mRootPosition.z));
		//}
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

		if (mAngryFlag)
		{
			mTreeRoot2->SetPosition(Vector3(mRootPosition.x + 10, mRootPosition.y, mRootPosition.z));
			mTreeRoot3->SetPosition(Vector3(mRootPosition.x - 10, mRootPosition.y, mRootPosition.z));
		}
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
	mManager->Add(mTreeRoot2);
	mManager->Add(mTreeRoot3);

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
	mOneShotSound = false;
	mSummonAlive = false;
	mEndAnimation = false;

	mAngryFlag = false;
	mAngryAnimFlag[0] = false;
	mAngryAnimFlag[1] = false;
	mAngryAnimFlag[2] = false;

	//�e�I�u�W�F�N�g�̏����ʒu���Z�b�g
	mOffsetRightHand = Vector3(position.x, position.y + 15.0f, position.z);
	mOffsetLeftHand = Vector3(position.x, position.y + 15.0f, position.z);
	mOffsetRootPos = Vector3(position.x, position.y - 20.0f, position.z);
	mRightHandPos = mOffsetRightHand;
	mLeftHandPos = mOffsetLeftHand;
	mRootPosition = mOffsetRootPos;
	mScale = Vector3(5.0f, 5.0f, 5.0f);
	//mScale = Vector3(10.0f, 10.0f, 10.0f);

	//�������I�u�W�F�N�g����
	mTreeRoot = new TreeRoot(mRootPosition, angle, mManager, mRend, number);
	mTreeRoot2 = new TreeRoot(mRootPosition, angle, mManager, mRend, number + 1);
	mTreeRoot3 = new TreeRoot(mRootPosition, angle, mManager, mRend, number + 2);
	//���������G���X�g�̏�����
	mSummonList.clear();

	//�T�E���h�̐ݒ�
	mSmallExplosion = std::make_shared<Sound>("SE/Long.mp3", true);
	mSmallExplosion->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mBigExplosion = std::make_shared<Sound>("SE/Big_Explosion.mp3", true);
	mBigExplosion->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Boss_Death.wav", true);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mNoDeathSE = std::make_shared<Sound>("SE/Boss_NoDamage.wav", true);
	mNoDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

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
	mAppleDropTime = std::make_shared<Timer>();
	mAppleDropTime->setTime(1.0f);
	mAppleInterval = std::make_shared<Timer>();
	mAppleInterval->setTime(0.1f);

	//�������U����Ԃ�������
	mRootStep = RootAttackStep::PIERCE_HAND;
	//���S�A�j���[�V������Ԃ̏�����
	mDeathStep = DeathAnimationStep::EXPLOSION;

	//mDeathParticle = std::make_shared<Explosion>(Vector3::zero, true);
	//mDeathParticle->Stop();
	mDamageParticle = std::make_shared<Hit>(Vector3::zero, true);
	mDamageParticle->Stop();

	//�l�p�̓����蔻��(�g�����������)
	//SetCollidder(Vector3(-10.0f, -10.0f, -10.0f), Vector3(10.0f, 50.0f, 10.0f));

	/*�����蔻��𑝂₹�Ȃ��̂ŁA��̕����̂ݓ�����悤�ɂ���*/
	//SetCollidder(Vector3(0.0f, 15.0f, 0.0f), mRadius);
	SetCollidder(Vector3(0.0f, 14.0f, 0.0f), mRadius);


#pragma region ���f���ǂݍ��� + ��

	mStringNum = to_string(number);

	//����
	mModelNumName01 = mModelName01 + mStringNum;
	mRend->AddModel(mModelNumName01, "Resouse/EnemyModel/TreeBoss/wood_body.obj", "Resouse/EnemyModel/TreeBoss/wood_body.png");
	mRend->SetColor(mModelNumName01, Vector4(0, 0, 0, 0));

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

	//�o���A
	mBarrierNum = mBarrier + mStringNum;
	mRend->AddModel(mBarrierNum, "Resouse/EnemyModel/BossBarrier/boss_barrier.obj", "Resouse/EnemyModel/BossBarrier/boss_barrier.png");
	//mRend->SetColor(mBarrierNum, Vector4(1, 1, 1, 0.5f));

	//�z��̏�����
	mSummonPoint.resize(SUMMON_COUNT);
	mSummonName.resize(SUMMON_COUNT);
	mSummonNum.resize(SUMMON_COUNT);
	for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
	{
		mSummonPoint[i] = position + AngleToVectorY(360.0f / end * i) * 30.0f;

		//�������鐔���A�����ꏊ�W���p�I�u�W�F�N�g�𐶐�����B
		mSummonName[i] = to_string(i);
		mSummonNum[i] = mSummonCircle + mSummonName[i];
		mRend->AddModel(mSummonNum[i], "Resouse/maru.obj", "Resouse/marui.png");
	}

#pragma endregion
}

void ElfTreeBoss::EnemyUpdate()
{
	if (HP <= 0)
	{
		mDeathAnimationFlag = true;
	}

	//�̗͂������ȉ��ɂȂ�����{����
	if (HP <= MAX_HP / 2)
	{
		mAngryFlag = true;
	}

	if (mAngryFlag == true)
	{
		//if (mAngryAnimFlag[0] && mAngryAnimFlag[1] && mAngryAnimFlag[1]) return;

		mScale += Vector3(0.1f, 0.1f, 0.1f);

		if (mScale.x >= 10.0f)
		{
			mScale = Vector3(10.0f, 10.0f, 10.0f);
			mAngryAnimFlag[0] = true;
		}

		mRadius += 1.0f;

		if (mRadius >= 5.0f)
		{

			mRadius = 5.0f;
			mAngryAnimFlag[1] = true;
		}

		mOffsetRightHand = Vector3(position.x, position.y + 25.0f, position.z);
		mOffsetLeftHand =  Vector3(position.x, position.y + 25.0f, position.z);
		mAngryAnimFlag[2] = true;

		//if (mOffsetRightHand.y >= position.y + 25.0f && mOffsetLeftHand.y >= position.y + 25.0f)
		//{

		//	mOffsetRightHand = Vector3(position.x, position.y + 55.0f, position.z);
		//	mOffsetLeftHand =  Vector3(position.x, position.y + 55.0f, position.z);
		//	mAngryAnimFlag[2] = true;
		//}
	}

	if (mAngryAnimFlag[2])
	{

		int test = 0;


		test = 20;
	}

	if (mDeadFlag)
	{
		death = true;
	}

	//�������X�g����łȂ��Ƃ�
	if (!mSummonList.empty())
	{
		//���������G�����݂���
		mSummonAlive = true;

		//���X�g���X�V
		for (int i = 0, end = static_cast<int>(mSummonList.size()); i < end; i++)
		{
			//������Ԃ��m�F
			bool summonDead = mSummonList[i]->GetDeathFlag();

			//����ł����烊�X�g����폜����
			if (summonDead)
			{
				mSummonList.erase(mSummonList.begin() + i);
				break;
			}
		}
	}
	else
	{
		//���������G�����݂��Ȃ�
		mSummonAlive = false;
	}

	//���ʂ̒���
	mSmallExplosion->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);
	mBigExplosion->setVol(BaseScene::mMasterSoundVol*BaseScene::mSESoundVol);

	/*��x�����I�u�W�F�N�g�𐶐�*/
	CreateObject();

	/*���S�A�j���[�V����*/
	DeathAnimation();

	/*�U��*/
	ChangeAttackState();
}

void ElfTreeBoss::EnemyRend()
{
	//������ԂłȂ�(������Ԃ�������)�Ȃ�`�悵�Ȃ�
	if (mDeathStep != DeathAnimationStep::EXPLOSION) return;

	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g

	mRend->Draw(mModelNumName01, position, Vector3(angle.x, angle.y, angle.z), mScale);
	mRend->Draw(mHandRightNum, mRightHandPos, Vector3(mHandAngle, angle.y, angle.z), mScale);
	mRend->Draw(mHandLeftNum, mLeftHandPos, Vector3(mHandAngle, angle.y, angle.z), mScale);

	//���������v���C���[��ǐՂ��Ă��鎞�̂ݕ\��
	if (mRootStep == RootAttackStep::CHASE_PLAYER ||
		mRootStep == RootAttackStep::WAIT)
	{
		mRend->Draw(mRootCircleNum, Vector3(mRootPosition.x, position.y - 3, mRootPosition.z), angle, mScale);
	}

	//if (mSummonAlive)
	//{
	//	mRend->Draw(mBarrierNum, Vector3(position.x, position.y, position.z), angle, Vector3(5, 5, 5));
	//}

	//�\����Ԃ̎������A�x����\��
	if (mDrawSummonPoint)
	{
		for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
		{
			mRend->Draw(mSummonNum[i], mSummonPoint[i], Vector3(0.0f, mSummonRotate += 10.0f, 0.0f), Vector3(2, 2, 2));
		}
	}

	mRend->SetColor(mModelNumName01, Vector4(0, 0, 0, 0));
}

void ElfTreeBoss::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//�G���G����������Ă��� & �{���Ԃ̎��A�_���[�W���󂯂Ȃ�
		if (mSummonAlive && mAngryFlag)
		{
			//SE����
			mNoDeathSE->setPos(position);
			mNoDeathSE->play();
		}
		else
		{
			//SE����
			mDamageSE->setPos(position);
			mDamageSE->play();
			//���f���̐F�ς�
			mRend->SetColor(mModelNumName01, Vector4(1, 0, 0, 1));

			mDamageParticle->setPos(position);
			mDamageParticle->Play();

			//�_���[�W����h��͂�����
			damagePool = col->GetColObject()->GetDamage() - DEFENSE;

			if (damagePool <= 0)
			{
				damagePool = 1;
			}

			HP -= damagePool;
		}
	}
}

void ElfTreeBoss::EnemyImGuiDebug()
{
}