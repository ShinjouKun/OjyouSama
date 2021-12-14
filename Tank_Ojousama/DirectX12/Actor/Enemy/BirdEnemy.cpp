#include "BirdEnemy.h"
#include "../../Weapons/LaunchBullet.h"
#include "../../Scene/BaseScene.h"

#include "../../Utility/Random.h"
#include "../../Items/Repair.h"
#include "../../Items/Shield.h"
#include "../../Items/Smoke.h"
#include "../../Render/TexRenderer.h"


BirdEnemy::BirdEnemy(
	const Vector3 & pos, 
	const Vector3 & ang,
	shared_ptr<TexRenderer> texRender,
	int num
)
{
	position = pos;
	angle = ang;
	mTexRender = texRender;
	number = num;
}

BirdEnemy::~BirdEnemy()
{
}

void BirdEnemy::Attack()
{
	switch (mActionStep)
	{
	case BirdEnemy::RISING:
		Action_Rising();//�㏸
		break;
	case BirdEnemy::MOVE:
		mTargetPosition = mManager->GetPlayer().GetPosition();
		Action_Move(mTargetPosition);
		break;
	case BirdEnemy::FIRE:
		Action_Fire();
		break;
	case BirdEnemy::BACK:
		Action_Back(mOffsetRisePosition);//���_�̏��ɖ߂�
		break;
	case BirdEnemy::DESCEND:
		Action_Descend();
		break;
	case BirdEnemy::RELOAD:
		Action_Reload();
		break;
	default:
		break;
	}
}

void BirdEnemy::Action_Rising()
{
	MovePoint(mOffsetRisePosition);

	if (InsideDistance(mOffsetRisePosition, 0.5f))
	{
		//��Ɉړ������Ƃ���B
		mActionStep = ActionStep::MOVE;
	}
}

void BirdEnemy::Action_Move(const Vector3 & targetPosition)
{
	//�v���C���[�̌��ֈړ�
	MovePointY(targetPosition);

	if (InsideDistanceY(targetPosition, ATTACK_LENGTH))
	{
		//�U�����J�n����
		mActionStep = ActionStep::FIRE;
	}
}

void BirdEnemy::Action_Fire()
{
	//�v���C���[�Ɍ����Ēe�𔭎�
	if (!mFireFlag)
	{
		Vector3 firePosition = AngleToVectorY(mFireAngle);
		mManager->Add(new LaunchBullet(position + firePosition, mTargetPosition, mManager, mRend, mPart, objType, mBulletNumber++,true));
		mFireFlag = true;

		mAttackSE->setPos(position);
		mAttackSE->play();
	}

	mLegRotate -= LEG_SPEED;

	if (mLegRotate < -LEG_RANGE)
	{
		mFinishAnimation = true;
		mLegRotate = -LEG_RANGE;
	}

	MovePointY(mPlayerPosition);

	if (InsideDistanceY(mPlayerPosition, 1.0f) && mFireFlag && mFinishAnimation)
	{
		mActionStep = ActionStep::BACK;
		mLegRotate = 0.0f;
	}
}

void BirdEnemy::Action_Back(const Vector3 & targetPosition)
{
	//���_�ɋA��
	MovePointY(mOffsetRisePosition);

	if (InsideDistanceY(targetPosition, 0.5f))
	{
		//���_���ɓ��������Ƃ���
		mActionStep = ActionStep::DESCEND;
	}
}

void BirdEnemy::Action_Descend()
{
	//���ɍ~���
	MovePoint(mOffsetPosition);

	if (InsideDistance(mOffsetPosition, 0.5f))
	{
		mActionStep = ActionStep::RELOAD;
	}
}

void BirdEnemy::Action_Reload()
{
	mReloadTime->update();

	if (mReloadTime->isTime())
	{
		mReloadTime->setTime(1.0f);
		mActionStep = ActionStep::RISING;
		mFireFlag = false;
		mFinishAnimation = false;
		mLegRotate = 0.0f;
	}
}

bool BirdEnemy::InsideDistanceY(const Vector3 & distance, const float length) const
{
	Vector3 dist = Vector3(distance.x, 0.0f, distance.z) - Vector3(position.x, 0.0f, position.z);
	float inside = dist.Length();

	if (inside > length)
	{
		//�͈͊O
		return false;
	}

	return true;
}

void BirdEnemy::EnemyInit()
{
	HP = 5;
	damage = 5;

	speed = 0.8f;
	mRadius = 2.0f;

	mFinishAnimation = false;
	death = false;
	mFireFlag = false;
	mCreateItem = false;

	objType = ObjectType::ENEMY;
	mActionStep = ActionStep::RISING;

	mScale = Vector3(2.0f, 2.0f, 2.0f);
	mOffsetPosition = position;//�A�҈ʒu��ۑ�
	//���A�҈ʒu�̐^���ۑ�
	mOffsetRisePosition = Vector3(mOffsetPosition.x, mOffsetPosition.y + RISE_HEIGHT, mOffsetPosition.z);
	SetCollidder(Vector3(0.0f, 5.0f, 0.0f), mRadius);

	//�^�C�}�[������
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);

	//�T�E���h������
	mAttackSE = std::make_shared<Sound>("SE/Bird_Attack.mp3", true);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//�p�[�e�B�N��������
	EXPLOSION_EFFECT = "Explosion";
	mParticleEmitter = make_shared<ParticleEmitterBox>(mPart);
	mParticleEmitter->LoadAndSet(EXPLOSION_EFFECT, "Resouse/Bom.jpg");

#pragma region ���f���̓ǂݍ���

	num = to_string(number);

	//�e
	mBullet = "BirdWingRight";
	mBulletNum = mBullet + num;
	mRend->AddModel(mBulletNum, "Resouse/EnemyModel/AppleBullet/apple.obj", "Resouse/EnemyModel/AppleBullet/apple.png");

	//�r
	mLeg = "BirdWingLeft";
	mLegNum = mLeg + num;
	mRend->AddModel(mLegNum, "Resouse/EnemyModel/Eagle/eagle_leg.obj", "Resouse/EnemyModel/Eagle/eagle.png");
	mRend->SetAncPoint(mLegNum, Vector3(0.0f, -3.5f, 0.0f));

	//��
	mBody = "BirdBody";
	mBodyNum = mBody + num;
	mRend->AddModel(mBodyNum, "Resouse/EnemyModel/Eagle/eagle_body.obj", "Resouse/EnemyModel/Eagle/eagle.png");
#pragma endregion
}

void BirdEnemy::EnemyUpdate()
{
	//������Ԃ̊Ď�
	AliveSurveillance();

	/*���S��ԊĎ�*/
	CheckAlive();

	/*�U��*/
	Attack();
}

void BirdEnemy::EnemyRend()
{
	//if (!GetActive()) return;

    //���f���̕`��
	DirectXManager::GetInstance()->SetData3D();

	if (!mFireFlag)
	{
		mRend->Draw(mBulletNum, Vector3(position.x, position.y + 2.0f, position.z), Vector3(0, mFireAngle, 0), mScale);
	}
	mRend->Draw(mLegNum, Vector3(position.x, position.y + 7.5f, position.z), Vector3(mLegRotate, mFireAngle, 0), mScale);
	mRend->Draw(mBodyNum, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), mScale);
}

void BirdEnemy::EnemyOnCollision(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		mDamageSE->setPos(position);
		mDamageSE->play();
		HP -= col->GetColObject()->GetDamage();
	}

	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		//�J�����ɓ������Ă���Ƃ��A�`����s���B
		SetActive(true);
	}
}

void BirdEnemy::EnemyImGuiDebug()
{

}

void BirdEnemy::CheckAlive()
{
	if (HP <= 0)
	{
		//�G�t�F�N�g����
		mParticleEmitter->EmitterUpdateBIG(EXPLOSION_EFFECT, position, angle);

		if (mCreateItem) return;

		Random::initialize();

		int count = Random::randomRange(0, 2);

		if (count == 0)
		{
			mManager->Add(new Repair(Vector3(position.x, position.y, position.z), Vector3(0, 0, 0), mManager, mRend, mTexRender, ItemState::Low, 0, 500000, 20));
			mCreateItem = true;
		}
		else /* if (count == 1)*/
		{
			mManager->Add(new Shield(position, angle, mManager, mRend, mTexRender, ItemState::Low, 0, 20));
			mCreateItem = true;
		}
		//else
		//{
		//	mManager->Add(new Smoke(position, angle, mManager, mRend, mPart, mTexRender, ItemState::Low, 0, 5 * 60));
		//}
	}
}
