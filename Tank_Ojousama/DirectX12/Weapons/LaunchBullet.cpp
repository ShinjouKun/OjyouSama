#include "LaunchBullet.h"
#include"../Collision/BaseCollider.h"

LaunchBullet::LaunchBullet(
	const Vector3 & pos, 
	const Vector3 & targetPosition, 
	ObjectManager * objectManager, 
	shared_ptr<ModelRenderer> modelRender, 
	shared_ptr<ParticleManager> particleManager,
	ObjectType objectType, 
	int num)
{
	position = pos;
	mTargetPosition = targetPosition;
	mObjectManager = objectManager;
	mModelRender = modelRender;
	mParticleManager = particleManager;
	setType = objectType;
	number = num;
}

LaunchBullet::~LaunchBullet()
{
}

void LaunchBullet::Init()
{
	SetBulletType();

	//���N���X�̕ϐ�
	damage = BULLET_DAMAGE;
	alive = 0;
	death = false;
	speed = MOVE_SPEED;

	//���N���X�̕ϐ�
	mScale = Vector3().one;
	mHalfLength = (mTargetPosition - position).Length() / 2.0f;

	//�R���C�_�[�̃Z�b�g
	SetCollidder(Vector3().zero, 0.5f);

	//���f���̓ǂݍ���
	name = "LaunchBullet";
	num = to_string(number);
	numName = name + num;
	mModelRender->AddModel(numName, "Resouse/Bullet.obj", "Resouse/Bullet.png");

	//�p�[�e�B�N���̐ݒ�
	mParticleEmitter = make_shared<ParticleEmitterBox>(mParticleManager);
	mParticleEmitter->LoadAndSet("Bom", "Resouse/Bom.jpg");
}

void LaunchBullet::Update()
{
	Move();
}

void LaunchBullet::Rend()
{
	DirectXManager::GetInstance()->SetData3D();//���f���p���Z�b�g
	mModelRender->Draw(numName, position, Vector3().zero, mScale);
}

void LaunchBullet::ImGuiDebug()
{
}

void LaunchBullet::OnCollison(BaseCollider * col)
{
	if (objType == BULLET && 
		(col->GetColObject()->GetType() == ObjectType::ENEMY || 
		 col->GetColObject()->GetType() == ObjectType::BOSS || 
		 col->GetColObject()->GetType() == ObjectType::ENEMYBULLET || 
		 col->GetColObject()->GetType() == ObjectType::BLOCK)
	)
	{
		mParticleEmitter->EmitterUpdate(PARTICLE_NAME, position, angle);
		death = true;
	}

	if (objType == ENEMYBULLET && 
		(col->GetColObject()->GetType() == ObjectType::PLAYER || 
		 col->GetColObject()->GetType() == ObjectType::BULLET || 
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
		//�e�͏㏸
		distHold.y = mVelocityY;
	}
	else
	{
		//�e�͉��~
		distHold.y = -mVelocityY;
	}

	velocity = distHold * speed;
	position += velocity;

	//�ڕW�ʒu�ɋ߂Â����玀�ʁB
	if (length <= 1.0f)
	{
		mParticleEmitter->EmitterUpdate(PARTICLE_NAME, position, angle);
		death = true;
	}

}
