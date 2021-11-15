#include "TestBoss.h"
#include "../Collision/SpherCollider.h"
#include "../Collision/AABBCollider.h"
#include "../Weapons/NormalBullet.h"
#include "../Weapons/LaunchBullet.h"
#include "../Utility/Timer/Timer.h"


#include "SummonEnemy.h"


TestBoss::TestBoss(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	shared_ptr<ModelRenderer> modelRender,
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
}

TestBoss::~TestBoss()
{
}

void TestBoss::Init()
{
	HP = MAX_HP;
	speed = MOVE_SPEED;
	objType = ObjectType::ENEMY;

	damage = 5;
	mBulletCount = 0;
	mEnemyNumber = 0;
	mCircleCount = 0;

	death = false;
	mShotNormalBullet = false;
	mSummonEnemy = false;
	mDrawSummonPoint = false;

	mSummonFlag = true;

	mAimingTime = std::make_shared<Timer>();
	mAimingTime->setTime(AIMING_TIME);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(RELOAD_TIME);
	mSummonTime = std::make_shared<Timer>();
	mSummonTime->setTime(SUMMON_TIME);

	//�l�p�̓����蔻��(�g�����������)
	//SetCollidder(new AABBCollider(Vector3().zero, Vector3(10.0f, 10.0f, 10.0f)));
	SetCollidder(Vector3().zero, 2.0f);

	mModelNum = to_string(number);
	mModelNumName01 = mModelName01 + mModelNum;
	mModelRender->AddModel(mModelNumName01, "Resouse/wood.obj", "Resouse/Big-treeA.png");


	//�z��̏�����
	mSummonPoint.resize(SUMMON_COUNT);
	testNum.resize(SUMMON_COUNT);
	testNumName.resize(SUMMON_COUNT);
	for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
	{
		mSummonPoint[i] = position + AngleToVectorY(360.0f / end * i) * 30.0f;

		testNum[i] = to_string(i);
		testNumName[i] = mCircleName + testNum[i];
		mModelRender->AddModel(testNumName[i], "Resouse/maru.obj", "Resouse/marui.png");
	}

	////�����n�_�p�I�u�W�F�N�g
	//mCircleNum
	//numCircle = mCircleName + num;
	//mModelRender->AddModel(numCircle, "Resouse/maru.obj", "Resouse/marui.png");
}

void TestBoss::Update()
{
	//���̓G�́A�v���C���[�Ƃ̋����ŕ`��𕪂���K�v������
	//RapidFire();//�A���ˌ�

	Summon();


	ImGui::Checkbox("-------------------", &mDrawSummonPoint);
}

void TestBoss::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g
	mModelRender->Draw(mModelNumName01, position, angle, Vector3(5.0f, 5.0f, 5.0f));

	//�\����Ԃ̎������A�x����\��
	if (mDrawSummonPoint)
	{
		for (int i = 0, end = static_cast<int>(mSummonPoint.size()); i < end; i++)
		{
			mModelRender->Draw(testNumName[i], mSummonPoint[i], angle, Vector3(2,2,2));
		}
	}
}

void TestBoss::ImGuiDebug()
{
}

void TestBoss::OnCollison(BaseCollider * col)
{
}

void TestBoss::RapidFire()
{
	//�_����
	if (!mShotNormalBullet)
	{
		mAimingTime->update();

		if (mAimingTime->isTime())
		{
			mShotNormalBullet = true;
			mAimingTime->setTime(AIMING_TIME);
		}
	}

	//����
	if (mShotNormalBullet)
	{
		mReloadTime->update();

		if (mReloadTime->isTime())
		{
			mReloadTime->setTime(RELOAD_TIME);

			Vector3 test = position + FIRE_POSITION;

			Vector3 mTargetPosition = mObjManager->GetPlayer().GetPosition() - test;
			mTargetPosition = mTargetPosition.normal();
			float radian = Math::atan2(mTargetPosition.x, mTargetPosition.z);
			mFireAngle = -Math::toDegrees(radian) - 180.0f;

			Vector3 firePosition = AngleToVectorY(mFireAngle) + test;

			float t = Math::atan2(mTargetPosition.y, mTargetPosition.z);
			float tt = -Math::toDegrees(t);

			mObjManager->Add(new NormalBullet(test, Vector3(tt, mFireAngle, 0.0f), mObjManager, mModelRender, mEffectManager, objType, mBulletCount++));

			//�w�萔�A���Ŏˌ�����
			if (mBulletCount >= RAPIDFIRE_COUNT)
			{
				mBulletCount = 0;
				mShotNormalBullet = false;
			}
		}
	}
}

void TestBoss::Summon()
{
	if (!mSummonFlag) return;

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
			mObjManager->Add(new SummonEnemy(mSummonPoint[i], angle, mObjManager, mModelRender, mEffectManager, mEnemyNumber++));
		}

		//�������I�������t���O��߂�
		mSummonEnemy = false;
		mSummonFlag = false;
	}
}

Vector3 TestBoss::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//�p�x�����W�A���p�ɖ߂�
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}
