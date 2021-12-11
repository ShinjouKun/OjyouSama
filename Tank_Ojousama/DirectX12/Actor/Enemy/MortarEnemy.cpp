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

	//��_�Ԃ̊p�x�����߂�
	float radian = atan2(distance.x, distance.z);
	//��]�𔽉f
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

	//�r��U��グ��
	mHandAngle -= 10.0f;

	//�U��グ�I�������
	if (mFireFlag && mHandAngle <= -90.0f)
	{
		mAttackStep = AttackStep::RELOAD;
	}
}

void MortarEnemy::AttackStep_RELOAD()
{
	//�����[�h
	mReloadTime->update();

	//�r�����ɖ߂�
	mHandAngle += 3.0f;

	if (mHandAngle >= 0.0f)
	{
		mHandAngle = 0.0f;
	}

	//�����[�h���� && �r�����ɖ߂肫���Ă�����
	if (mReloadTime->isTime() && mHandAngle >= 0.0f)
	{
		//�����̒l�������_������Ȃ��ƁA�S���̍U���ʒu���d�Ȃ����Ⴄ

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

	/*���S�A�j���[�V�������J�n*/
	DeathAnimation();
}

void MortarEnemy::Attack()
{


}

void MortarEnemy::DeathAnimation()
{
	//������ԂłȂ��@�Ȃ珈�����Ȃ�
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

	//���ԂɂȂ��Ă��Ȃ����
	if (!mRiseTime->isTime())
	{
		//��]
		barrelAngle += 50.0f;
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

	//�^�C�}�[������
	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);
	mRiseTime = std::make_shared<Timer>();
	mRiseTime->setTime(1.0f);
	mDeathTime = std::make_shared<Timer>();
	mDeathTime->setTime(1.0f);

	//�T�E���h������
	mAttackSE = std::make_shared<Sound>("SE/Golem_Attack.mp3", false);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Golem_Damage.mp3", false);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//�p�[�e�B�N��������
	EXPLOSION_EFFECT = "Explosion";
	mParticleEmitter = make_shared<ParticleEmitterBox>(mPart);
	mParticleEmitter->LoadAndSet(EXPLOSION_EFFECT, "Resouse/Bom.jpg");

	mExplosion = std::make_shared<Explosion>(Vector3::zero, true);
	mExplosion->Stop();

#pragma region ���f���̓ǂݍ���

	num = to_string(number);

	//�E��
	mHandRight = "MortarHandRight";
	mHandRNum = mHandRight + num;
	mRend->AddModel(mHandRNum, "Resouse/gorem_hands_R.obj", "Resouse/gorem.png");

	//����
	mHandLeft = "MortarHandLeft";
	mHandLNum = mHandLeft + num;
	mRend->AddModel(mHandLNum, "Resouse/gorem_hands_L.obj", "Resouse/gorem.png");

	mRend->SetAncPoint(mHandRNum, Vector3(0.0f, -3.0f, 0.0f));
	mRend->SetAncPoint(mHandLNum, Vector3(0.0f, -3.0f, 0.0f));

	//��
	mBody = "MortarBody";
	mBodyNum = mBody + num;
	mRend->AddModel(mBodyNum, "Resouse/gorem_body.obj", "Resouse/gorem.png");

#pragma endregion
}

void MortarEnemy::EnemyUpdate()
{
	/*������Ԃ��Ď�*/
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

	//������ԂȂ珈�����Ȃ�
	if (mDeathAnimation) return;

	/*�U��*/
	void Attack();
}

void MortarEnemy::EnemyRend()
{
	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	//���f���̕`��
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
		//�J�����ɓ������Ă���Ƃ��A�`����s���B
		SetActive(true);
	}
}

void MortarEnemy::EnemyImGuiDebug()
{
}