#include "SummonEnemy.h"
#include "../../Collision/SpherCollider.h"
#include "../../Utility/Timer/Timer.h"
#include "../../Sound/Sound.h"
#include"../../Scene/BaseScene.h"
#include "../../ParticleSystem/ParticleType/Explosion.h"
#include "../../ParticleSystem/ParticleType/Hit.h"

SummonEnemy::SummonEnemy(
	const Vector3 & pos,
	const Vector3 & ang,
	ObjectManager * objManager,
	std::shared_ptr<ModelRenderer> modelRender,
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

bool SummonEnemy::GetDeathFlag() const
{
	return mDeadFlag;
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
	mDeathAnimation = false;
	mDeadFlag = false;

	mScale = Vector3(0.5f, 0.5f, 0.5f);
	SetCollidder(Vector3(0.0f, 1.0f, 0.0f), 1.5f);

	//�񋓌^������
	objType = ObjectType::ENEMY;
	mAttackStep = AttackStep::FALL_DOWN;
	mDeathStep = DeathAnimationStep::RISE_SKY;

	//�T�E���h������
	mAttackSE = std::make_shared<Sound>("SE/punti.mp3", true);
	mAttackSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDamageSE = std::make_shared<Sound>("SE/Summon_Damage.wav", true);
	mDamageSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);
	mDeathSE = std::make_shared<Sound>("SE/Small_Explosion.wav", true);
	mDeathSE->setVol(BaseScene::mMasterSoundVol * BaseScene::mSESoundVol);

	//�^�C�}�[������
	mGetupTimer = std::make_shared<Timer>();
	mGetupTimer->setTime(1.0f);
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

	//���f���ǂݍ���
	mMyNumber = to_string(number);
	mSummon = "SummonEnemy";
	mSummonNum = mSummon + mMyNumber;
	mModelRender->AddModel(mSummonNum, "Resouse/wood.obj", "Resouse/Big-treeA.png");
}

void SummonEnemy::Update()
{
	/*���S��ԊĎ�*/
	CheckAlive();

	//������ԂȂ珈�����Ȃ�
	if (mDeathAnimation) return;

	/*�ړ�*/
	Move();

	/*�|�ꂱ�ݍU��*/
	DownAttack();
}

void SummonEnemy::Rend()
{
	//�\����Ԃ��ǂ���
	//if(!GetActive()) return;

	if (mDeathStep == DeathAnimationStep::EXPLOSION) return;

	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g
	mModelRender->Draw(mSummonNum, position, Vector3(angle.x, mFireAngle, angle.z), mScale);
}

void SummonEnemy::ImGuiDebug()
{
}

void SummonEnemy::OnCollison(BaseCollider * col)
{
	if (col->GetColObject()->GetType() == ObjectType::BULLET)
	{
		//SE����
		mDamageSE->setPos(position);
		mDamageSE->play();

		//�p�[�e�B�N������
		mDamageParticle->setPos(Vector3(position.x, position.y + 5.0f, position.z));
		mDamageParticle->Play();

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
	if (col->GetColObject()->GetType() == ObjectType::BLOCK)
	{
		position = mPreviousPosition;
	}
}

void SummonEnemy::CheckAlive()
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

	/*���S�A�j���[�V�����J�n*/
	DeathAnimation();
}

void SummonEnemy::Move()
{
	//�O�t���[���̈ʒu���擾
	mPreviousPosition = position - velocity;

	//�v���C���[�̈ʒu���Ď�
	mPlayerPosition = mObjManager->GetPlayer().GetPosition();

	//����͈͓��Ƀv���C���[������Ȃ�
	if (WithinDistance(mPlayerPosition, SEARCH_LENGTH))
	{
		mWithinPlayerFlag = true;

		//�ˌ��͈͓��Ƀv���C���[������Ȃ�
		if (WithinDistance(mPlayerPosition, FIND_LRNGTH))
		{
			//�ˌ�����Ƃ��A�ړ����x���������オ��
			speed = 0.3f;

			//�U���͈͓��Ƀv���C���[��������
			if (WithinDistance(mPlayerPosition, ATTACK_LENGTH))
			{
				//�U�����ɂ���
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

	//�͈͓��Ƀv���C���[������ & �U�����Ă��Ȃ���
	if (mWithinPlayerFlag && !mAttackFlag)
	{
		Vector3 distance = mPlayerPosition - position;
		distance = distance.normal();
		//��_�Ԃ̊p�x�����߂�
		float radian = atan2(distance.x, distance.z);
		//��]�𔽉f
		angle.y = Math::toDegrees(radian) + 180.0f;
		mFireAngle = -Math::toDegrees(radian) - 180.0f;

		//�ړ�����
		velocity = distance * speed;
		position += velocity;

		/*�ړ��A�j���[�V����*/
		MoveAnimation();
	}
	else
	{
		//�r�̊p�x�����ɖ߂�
		angle.z = 0.0f;
	}
}

void SummonEnemy::MoveAnimation()
{
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
}

void SummonEnemy::DownAttack()
{
	//�U�����o�Ȃ���Ώ������Ȃ�
	if (!mAttackFlag) return;

	switch (mAttackStep)
	{
	case SummonEnemy::FALL_DOWN:
		AttackStep_FallDown();
		break;
	case SummonEnemy::WAIT:
		AttackStep_Wait();
		break;
	case SummonEnemy::GET_UP:
		AttackStep_GetUp();
		break;
	default:
		break;
	}
}

void SummonEnemy::AttackStep_FallDown()
{
	angle.x += mDownSpeed;
	if (angle.x > 90.0f)
	{
		mAttackStep = AttackStep::WAIT;
		mAttackSE->setPos(position);
		mAttackSE->play();
	}
}

void SummonEnemy::AttackStep_Wait()
{
	mGetupTimer->update();

	if (mGetupTimer->isTime())
	{
		mGetupTimer->setTime(1.0f);
		mAttackStep = AttackStep::GET_UP;
	}
}

void SummonEnemy::AttackStep_GetUp()
{
	angle.x -= mDownSpeed;
	if (angle.x <= 0.0f)
	{
		mAttackFlag = false;
		mAttackStep = AttackStep::FALL_DOWN;
	}
}

void SummonEnemy::DeathAnimation()
{
	//������ԂłȂ��@�Ȃ珈�����Ȃ�
	if (!mDeathAnimation) return;

	switch (mDeathStep)
	{
	case SummonEnemy::RISE_SKY:
		DeathAnimeStep_RiseSky();
		break;
	case SummonEnemy::EXPLOSION:
		DeathAnimeStep_Explosion();
		break;
	default:
		break;
	}
}

void SummonEnemy::DeathAnimeStep_RiseSky()
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
		//������SE��炵����A�����������肷��
		//SE����
		mDeathSE->setPos(position);
		mDeathSE->play();

		//�p�[�e�B�N������
		mDeathParticle->setPos(position);
		mDeathParticle->Play();

		mDeathStep = DeathAnimationStep::EXPLOSION;
	}
}

void SummonEnemy::DeathAnimeStep_Explosion()
{
	mDeathTime->update();

	if (mDeathTime->isTime())
	{
		mDeadFlag = true;
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
