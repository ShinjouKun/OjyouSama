#include "SummonEnemy.h"
#include "../Collision/SpherCollider.h"
#include "../Weapons/NormalBullet.h"
#include "../Utility/Timer/Timer.h"
#include "AttackArea.h"

SummonEnemy::SummonEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	shared_ptr<ModelRenderer> modelRender,
	shared_ptr<ParticleManager> effectManager,
	int num)
{
	position = pos;
	angle = ang;
	mObjManager = objManager;
	mModelRender = modelRender;
	mEffectManager = effectManager;
	number = num;
}

SummonEnemy::~SummonEnemy()
{
}

void SummonEnemy::Init()
{
	//�ŏ��͔�\�����
	SetActive(false);

	HP = 10;
	damage = 5;

	speed = 0.2f;

	death = false;
	mWithinPlayerFlag = false;
	mAttackFlag = false;
	mStep = false;

	mAttackStep = AttackStep::FALL_DOWN;

	mAttackArea = std::make_shared<AttackArea>(position, angle,mObjManager,mModelRender,number);
	mGetupTimer = std::make_shared<Timer>();
	mGetupTimer->setTime(1.0f);

	objType = ObjectType::ENEMY;
	SetCollidder(new SphereCollider(Vector3().zero, 1.0f));


	mModelNum = to_string(number);
	mModelNumName01 = mModelName01 + mModelNum;
	mModelRender->AddModel(mModelNumName01, "Resouse/wood.obj", "Resouse/Big-treeA.png");
}

void SummonEnemy::Update()
{
	mPreviousPosition = position - velocity;

	//�v���C���[�̈ʒu���Ď�
	mPlayerPosition = mObjManager->GetPlayer().GetPosition();

	if (HP <= 0)
	{
		death = true;
	}


	//�͈͓��Ƀv���C���[������Ȃ�
	if (WithinDistance(mPlayerPosition,SEARCH_LENGTH))
	{
		mWithinPlayerFlag = true;

		//�ˌ��͈͓��Ƀv���C���[������Ȃ�
		if (WithinDistance(mPlayerPosition, FIND_LRNGTH))
		{
			speed = 0.3f;

			if (WithinDistance(mPlayerPosition, ATTACK_LENGTH))
			{
				mAttackFlag = true;
			}
		}
		else
		{
			speed = 0.2f;
		}
	}
	else
	{
		mWithinPlayerFlag = false;
	}

	if (mWithinPlayerFlag && !mAttackFlag)
	{

		Vector3 distance = mPlayerPosition - position;
		distance = distance.normal();
		//��_�Ԃ̊p�x�����߂�
		float radian = atan2(distance.x, distance.z);
		//��]�𔽉f
		angle.y = Math::toDegrees(radian) + 180.0f;
		float test = -Math::toDegrees(radian) - 180.0f;

		velocity = distance * speed;
		position += velocity;

#pragma region �A�j���[�V����

		//�E�X�e�b�v
		if (mStep)
		{
			angle.z++;
			if (angle.z > 25.0f)
			{
				mStep = false;
			}
		}
		else
		{
			angle.z--;
			if (angle.z < -25.0f)
			{
				mStep = true;
			}
		}

#pragma endregion
	}
	else
	{
		angle.z = 0.0f;
	}



	if (mAttackFlag)
	{
		/*
		���̏�ɓ|�ꂱ��->���̎������蔻��\��
		->x����90�x��]����΂������ȁH
		���΂炭�|�ꂽ�܂�->���b
		���̏�ŋN���オ��
		�N���オ��I�������U���I��
		*/

		switch (mAttackStep)
		{
		case SummonEnemy::FALL_DOWN:

			angle.x++;
			if (angle.x > 90.0f)
			{
				mAttackStep = AttackStep::WAIT;
			}

			break;
		case SummonEnemy::WAIT:



			break;
		case SummonEnemy::GET_UP:
			break;
		default:
			break;
		}
	}



	ImGui::SliderInt("**********************", &HP, 0, 100);
}

void SummonEnemy::Rend()
{
	//�\����Ԃ��ǂ���
    //if(!GetActive()) return;

	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g
	mModelRender->Draw(mModelNumName01, position, Vector3(angle.x,angle.y,angle.z), Vector3(0.5f, 0.5f, 0.5f));
}

void SummonEnemy::ImGuiDebug()
{
}

void SummonEnemy::OnCollison(BaseCollider * col)
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

	if (col->GetColObject()->GetType() == ObjectType::ENEMY)
	{
		//�����̔ԍ��������菬����������
		if (col->GetColObject()->GetID() > GetID())
		{
			position = mPreviousPosition;
		}
	}
}

bool SummonEnemy::WithinDistance(const Vector3 & targetPosition, const float distance)
{
	//�ړ�&��~
	Vector3 dist = targetPosition - position;
	float length = dist.Length();
	dist = dist.normal();

	//�ڕW�ʒu�Ƃ̋������A�w�苗���ȏゾ������ �������Ȃ�
	if (length > distance)
	{
		return false;
	}

	return true;
}
