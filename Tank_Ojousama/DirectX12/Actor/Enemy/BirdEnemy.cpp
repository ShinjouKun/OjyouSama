#include "BirdEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Weapons/LaunchBullet.h"

BirdEnemy::BirdEnemy(
	const Vector3 & pos, 
	const Vector3 & ang,
	ObjectManager * objectManager,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<ParticleManager> effectManager,
	int num
)
{
	position = pos;
	angle = ang;
	mObjManager = objectManager;
	mModelRender = modelRender;
	mEffectManager = effectManager;
	number = num;
}

BirdEnemy::~BirdEnemy()
{
}

void BirdEnemy::Init()
{
	HP = 100;
	damage = 5;

	speed = 0.8f;
	mRadius = 2.0f;
	mFireAngle = 0.0f;

	mIntervalTime = std::make_shared<Timer>();
	mIntervalTime->setTime(1.0f);
	mReloadTime = std::make_shared<Timer>();
	mReloadTime->setTime(1.0f);

	mScale = Vector3(2.0f, 2.0f, 2.0f);
	mOffsetPosition = position;//�A�҈ʒu��ۑ�
	//���A�҈ʒu�̐^���ۑ�
	mOffsetRisePosition = Vector3(mOffsetPosition.x, mOffsetPosition.y + RISE_HEIGHT, mOffsetPosition.z);

	death = false;
	mFireFlag = false;

	objType = ObjectType::ENEMY;

	SetCollidder(Vector3(0.0f, 5.0f, 0.0f), mRadius);

	mActionStep = ActionStep::RISING;

#pragma region ���f���̓ǂݍ���

	num = to_string(number);

	//�E��
	mWingRight = "BirdWingRight";
	mWingRNum = mWingRight + num;
	mModelRender->AddModel(mWingRNum, "Resouse/gorem_hands_R.obj", "Resouse/gorem.png");

	//����
	mWingLeft = "BirdWingLeft";
	mWingLNum = mWingLeft + num;
	mModelRender->AddModel(mWingLNum, "Resouse/gorem_hands_L.obj", "Resouse/gorem.png");

	mModelRender->SetAncPoint(mWingRNum, Vector3(0.0f, -3.0f, 0.0f));
	mModelRender->SetAncPoint(mWingLNum, Vector3(0.0f, -3.0f, 0.0f));

	//��
	mBody = "BirdBody";
	mBodyNum = mBody + num;
	mModelRender->AddModel(mBodyNum, "Resouse/gorem_body.obj", "Resouse/gorem.png");
#pragma endregion
}

void BirdEnemy::Update()
{
	if (HP <= 0)
	{
		death = true;
	}

	switch (mActionStep)
	{
	case BirdEnemy::RISING:
		Action_Rising();//�㏸
		break;
	case BirdEnemy::MOVE:
		//���̎������A�v���C���[�̈ʒu���擾�ł���
		mTargetPosition = mObjManager->GetPlayer().GetPosition();
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

void BirdEnemy::Rend()
{
	//if (!GetActive()) return;

	//���f���̕`��
	DirectXManager::GetInstance()->SetData3D();
	mModelRender->Draw(mWingRNum, Vector3(position.x, position.y + 7.0f, position.z), Vector3(0, mFireAngle, 0), mScale);
	mModelRender->Draw(mWingLNum, Vector3(position.x, position.y + 7.0f, position.z), Vector3(0, mFireAngle, 0), mScale);
	mModelRender->Draw(mBodyNum, Vector3(position.x, position.y, position.z), Vector3(0, mFireAngle, 0), mScale);
}

void BirdEnemy::ImGuiDebug()
{
}

void BirdEnemy::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		HP -= col->GetColObject()->GetDamage();
	}

	if (col->GetColObject()->GetType() == ObjectType::CAMEAR)
	{
		//�J�����ɓ������Ă���Ƃ��A�`����s���B
		SetActive(true);
	}
}

void BirdEnemy::Move(const Vector3 & targetPosition)
{
	Vector3 distance = targetPosition - position;
	float length = distance.Length();
	distance = distance.normal();

	//�e���A�����ʒu��艺�ɍs���Ȃ���I
	velocity = distance * mRiseSpeed;
	position += velocity;
}

void BirdEnemy::MoveY(const Vector3 & targetPosition)
{
	//Y���Ȃ����ʒu����A���������߂�
	Vector3 holdTarget = Vector3(targetPosition.x, 0.0f, targetPosition.z);
	Vector3 holdPosition = Vector3(position.x, 0.0f, position.z);
	Vector3 distance = holdTarget - holdPosition;
	float length = distance.Length();
	distance = distance.normal();

	//Y���Ȃ�������
	Vector3 distHold = Vector3(distance.x, 0.0f, distance.z);

	//�e���A�����ʒu��艺�ɍs���Ȃ���I
	velocity = distHold * speed;
	position += velocity;
}

void BirdEnemy::Action_Rising()
{
	//�^��ɏ㏸
	Vector3 distance = mOffsetRisePosition - position;
	float length = distance.Length();
	distance = distance.normal();

	//�e���A�����ʒu��艺�ɍs���Ȃ���I
	velocity = distance * mRiseSpeed;
	position += velocity;

	if (length < 0.5f)
	{
		//��Ɉړ������Ƃ���B
		mActionStep = ActionStep::MOVE;
	}
}

void BirdEnemy::Action_Move(const Vector3 & targetPosition)
{

	//�v���C���[�̌��ֈړ�
	//Move(targetPosition);
	MoveY(targetPosition);

	if (InsideDistance(targetPosition, ATTACK_LENGTH))
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
		mObjManager->Add(new LaunchBullet(position + firePosition, mTargetPosition, mObjManager, mModelRender, mEffectManager, objType, mBulletNumber++,true));
		mFireFlag = true;
	}

	mIntervalTime->update();

	if (mIntervalTime->isTime() && mFireFlag)
	{
		mIntervalTime->setTime(1.0f);
		mActionStep = ActionStep::BACK;
	}
}

void BirdEnemy::Action_Back(const Vector3 & targetPosition)
{
	//���_�ɋA��
	//Move(targetPosition);
	MoveY(mOffsetRisePosition);

	if (InsideDistance(targetPosition, 0.5f))
	{
		//���_���ɓ��������Ƃ���
		mActionStep = ActionStep::DESCEND;
	}
}

void BirdEnemy::Action_Descend()
{
	//���ɍ~���
	Move(mOffsetPosition);

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
	}
}

bool BirdEnemy::InsideDistance(const Vector3 & distance, const float length) const
{
	Vector3 dist = distance - position;
	float inside = dist.Length();

	if (inside > length)
	{
		//�͈͊O
		return false;
	}

	return true;
}

Vector3 BirdEnemy::AngleToVectorY(float angle) const
{
	Vector3 vector = Vector3(0, 0, 0);

	//�p�x�����W�A���p�ɖ߂�
	float radian = Math::toRadians(angle);

	float x = cosf(radian);
	float z = sinf(radian);

	return Vector3(x, 0, z).normal();
}
