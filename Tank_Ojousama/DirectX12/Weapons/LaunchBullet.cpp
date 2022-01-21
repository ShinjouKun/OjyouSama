#include "LaunchBullet.h"
#include"../Collision/BaseCollider.h"

LaunchBullet::LaunchBullet(
	const Vector3 & pos, 
	const Vector3 & targetPosition, 
	ObjectManager * objectManager, 
	std::shared_ptr<ModelRenderer> modelRender,
	shared_ptr<ParticleManager> particleManager,
	ObjectType objectType, 
	int num,
	const bool isApple
)
{
	position = pos;
	mTargetPosition = targetPosition;
	mObjectManager = objectManager;
	mModelRender = modelRender;
	mParticleManager = particleManager;
	setType = objectType;
	number = num;
	mAppleFlag = isApple;
}

LaunchBullet::~LaunchBullet()
{
}

void LaunchBullet::ChangeApple(const bool value)
{
	mAppleFlag = value;
	num = to_string(number);

}

void LaunchBullet::Init()
{
	SetBulletType();

	//���N���X�̕ϐ�
	damage = BULLET_DAMAGE;
	HP = 10;
	alive = 0;
	death = false;
	speed = MOVE_SPEED;

	//���N���X�̕ϐ�
	mScale = Vector3().one;
	mCircleScale = Vector3(8.0f, 8.0f, 8.0f);
	mHalfLength = (mTargetPosition - position).Length() / 2.0f;
	mDrawArea = false;

	//�R���C�_�[�̃Z�b�g
	SetCollidder(Vector3().zero, 1.5f);

	num = to_string(number);

	if (mAppleFlag)
	{
		mAppleNum = mAppleName + num;
		mModelRender->AddModel(mAppleNum, "Resouse/EnemyModel/AppleBullet/apple.obj", "Resouse/EnemyModel/AppleBullet/apple.png");
		mModelRender->SetAncPoint(mAppleNum, Vector3(0, -1, 0));
	}
	else
	{
		//���f���̓ǂݍ���
		name = "LaunchBullet";
		numName = name + num;
		mModelRender->AddModel(numName, "Resouse/throw.obj", "Resouse/throw_ss.png");
	}

	//�����n�_�p�I�u�W�F�N�g
	numBlock = mBlockName + num;
	mModelRender->AddModel(numBlock, "Resouse/maru.obj", "Resouse/marui.png");
	mModelRender->SetColor(numBlock, Vector4(0, 0, 0, 0.2f));

	//�p�[�e�B�N���̐ݒ�
	mParticleEmitter = make_shared<ParticleEmitterBox>(mParticleManager);
	mParticleEmitter->LoadAndSet(PARTICLE_NAME, "Resouse/effect.png");
	mParticleEmitter->LoadAndSet(EXPLOSION, "Resouse/Bom.jpg");
}

void LaunchBullet::Update()
{
	Move();

	angle.y+=10.0f;
	angle.x+=10.0f;
	angle.z += 10.0f;
}

void LaunchBullet::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g

	if (mAppleFlag)
	{
		mModelRender->Draw(mAppleNum, position, angle, mScale * 2.0f);
	}
	else
	{
		mModelRender->Draw(numName, position, angle, mScale);
		if (mDrawArea)
		{
			mModelRender->Draw(numBlock, Vector3(mTargetPosition.x, mTargetPosition.y - test, mTargetPosition.z), Vector3().zero, mCircleScale);
		}
	}
}

void LaunchBullet::ImGuiDebug()
{
}

void LaunchBullet::OnCollison(BaseCollider * col)
{
	if (objType == ENEMYBULLET &&
		(col->GetColObject()->GetType() == ObjectType::PLAYER)
		)
	{
		mParticleEmitter->EmitterUpdate(EXPLOSION, position, angle);
		death = true;
	}

	if (objType == ENEMYBULLET && 
		(col->GetColObject()->GetType() == ObjectType::BULLET || 
		 col->GetColObject()->GetType() == ObjectType::BLOCK ||
		 col->GetColObject()->GetType() == ObjectType::ITEM)
	)
	{
		mParticleEmitter->EmitterUpdate(PARTICLE_NAME, position, angle);
		death = true;
	}
}

void LaunchBullet::Move()
{
	if (!mAppleFlag)
	{
		//Y���Ȃ����ʒu����A���������߂�
		Vector3 holdTarget = Vector3(mTargetPosition.x, 0.0f, mTargetPosition.z);
		Vector3 holdPosition = Vector3(position.x, 0.0f, position.z);
		Vector3 distance = holdTarget - holdPosition;
		float length = distance.Length();
		distance = distance.normal();

		//Y���Ȃ�������
		Vector3 distHold = Vector3(distance.x, 0.0f, distance.z);

		//�����������ȏ�Ȃ�
		if (length > mHalfLength)
		{
			mDrawArea = false;
			//�e�͏㏸
			distHold.y = mVelocityY;
		}
		else
		{
			mDrawArea = true;
			//#1�I�t�Z�b�g�X�P�[���������Ŋ���
			mCircleScale = Vector3(10.0f, 10.0f, 10.0f) / (length / 10.0f);

			test = 4.5f / (length / 10);

			//test += 0.01f;
			//mCircleScale *= test;

			if (mCircleScale.x < 1.0f) mCircleScale = Vector3().one;
			if (mCircleScale.x > 8.0f)mCircleScale = Vector3(10, 10, 10);
			if (test <= -4.5f) test = -4.5f;

			//�e�͉��~
			distHold.y = -mVelocityY;
		}

		//�e���A�����ʒu��艺�ɍs���Ȃ���I
		velocity = distHold * speed;
		position += velocity;

		//�ڕW�ʒu�ɋ߂Â����玀�ʁB
		if (length <= 0.8f)
		{
			mParticleEmitter->EmitterUpdate(PARTICLE_NAME, position, angle);
			death = true;
		}
	}
	else
	{
		mDrawArea = false;
		Vector3 distance = mTargetPosition - position;
		float length = distance.Length();
		distance = distance.normal();
		velocity = distance * speed;
		position += velocity;

		//�ڕW�ʒu�ɋ߂Â����玀�ʁB
		if (length <= 0.8f)
		{
			mParticleEmitter->EmitterUpdate(PARTICLE_NAME, position, angle);
			death = true;
		}
	}
}
