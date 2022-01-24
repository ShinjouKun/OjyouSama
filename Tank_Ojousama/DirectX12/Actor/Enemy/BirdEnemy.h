#pragma once
#include "BaseEnemy.h"

class Timer;
class TexRenderer;
class Explosion;
class Hit;

//�����ł���G
class BirdEnemy : public BaseEnemy
{
public:

	BirdEnemy(
		const Vector3& pos,
		const Vector3& ang,
		shared_ptr<TexRenderer> texRender,
		int num
	);

	~BirdEnemy();

private:

	//BaseEnemy����p��
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*������Ԃ��Ď�*/
	void CheckAlive();

	/*�U��*/
	void Attack();

	void Action_Rising();
	void Action_Move(const Vector3& targetPosition);
	void Action_Fire();
	void Action_Back(const Vector3& targetPosition);
	void Action_Descend();
	void Action_Reload();

	bool InsideDistanceY(const Vector3& distance, const float length) const;

private:

	enum ActionStep
	{
		RISING, //�㏸
		MOVE,   //�ړ�
		FIRE,   //�e����
		BACK,   //�߂�
		DESCEND,//���~
		RELOAD,
	};
	ActionStep mActionStep;//�U���p���

	shared_ptr<TexRenderer> mTexRender;//�e�N�X�`���`��p
	shared_ptr<Timer> mIntervalTime;
	shared_ptr<Timer> mReloadTime;
	shared_ptr<Sound> mAttackSE;//�U������SE
	shared_ptr<Sound> mDamageSE;     //�_���[�W���󂯂�����SE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;
	shared_ptr<Hit> mDamageParticle;      //�_���[�W���󂯂����̃p�[�e�B�N��
	//shared_ptr<Explosion> mDeathParticle; //���S�������̃p�[�e�B�N��

	Vector3 mScale;             //�傫��
	Vector3 mTargetPosition;    //�v���C���[�̈ʒu
	Vector3 mOffsetPosition;    //�A��ʒu
	Vector3 mOffsetRisePosition;//�A��ʒu�̐^��

	const float RISE_HEIGHT = 10.0f;  //�㏸�ʂ̍���
	const float ATTACK_LENGTH = 50.0f;//�U���͈�
	const float LEG_SPEED = 10.0f;    //�r�̉�]���x
	const float LEG_RANGE = 100.0f;   //�r�̍ő��]��
	const float WALK_SPEED = 1.0f;//�ʏ�̈ړ����x
	const float RUN_SPEED = 1.5f; //�e����������̈ړ����x

	float mRadius;          //���a
	float mRiseSpeed = 0.5f;//�㏸�̑���
	float mLegRotate = 0.0f;//�r�̉�]

	bool mFinishAnimation = false;//�A�j���[�V�������I�����Ă��邩
	bool mFireFlag;               //�e�𔭎˂�����
	bool mCreateItem;             //�A�C�e���𐶐��������H

	int mBulletNumber;//���˂����e��

	string num;       //string�^�̎��ʔԍ�
	string mBullet;   //�C�g�̖��O�o�^
	string mLeg;      //�C���̖��O�o�^
	string mBody;     //�ԑ̖̂��O�o�^
	string mBulletNum;//���ʔԍ�+�C�g�̖��O
	string mLegNum;   //���ʔԍ�+�C���̖��O
	string mBodyNum;  //���ʔԍ�+�ԑ̖̂��O

	string EXPLOSION_EFFECT = "Explosion";//�G�t�F�N�g��
};