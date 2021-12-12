#pragma once
#include "BaseEnemy.h"

class Timer;

class Explosion;

//�Œ�C��̓G
class MortarEnemy :public BaseEnemy
{
public:

	MortarEnemy(
		const Vector3& pos,
		const Vector3& ang,
		int num
	);

	~MortarEnemy();

private:

	//BaseEnemy����p��
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

	/*������Ԃ��Ď�*/
	void CheckAlive();

	/*�U��*/
	void Attack();

	/*���S���̉��o*/
	void DeathAnimation();

	/*��ɏ㏸*/
	void DeathAnimeStep_RiseSky();

	/*����&���b��~*/
	void DeathAnimeStep_Explosion();

private:

	/*���S�A�j���[�V�����̑J�ڏ�*/
	enum DeathAnimationStep
	{
		RISE_SKY, //���ɕ����オ��
		EXPLOSION,//���ł���
	};
	DeathAnimationStep mDeathStep;

	/*�U���̑J�ڏ�*/
	enum AttackStep
	{
		AIMING,//�Ə����킹��
		FIRE,  //�e���ˁI
		RELOAD,//�����[�h��
	};
	AttackStep mAttackStep;//�U���p���

	shared_ptr<Timer> mAimingTime;//�_������
	shared_ptr<Timer> mReloadTime;//�����[�h����
	shared_ptr<Timer> mRiseTime;  //�㏸����
	shared_ptr<Timer> mDeathTime; //���S�Ɏ��ʂ܂ł̎���
	shared_ptr<Sound> mAttackSE;  //�U������SE
	shared_ptr<Sound> mDamageSE;  //�U������SE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	std::shared_ptr<Explosion> mExplosion;//������

	Vector3 mScale;         //�傫��
	Vector3 mTargetPosition;//�e��e�̗����n�_

	float mRadius;    //���a
	float barrelAngle;//�U�����̌���
	float mHandAngle; //�r�̊p�x

	int mBulletNumber;//���˂����e��

	bool mFireFlag;//�e�𔭎˂�����
	bool mDeathAnimation;//���S�A�j���[�V�������J�n���邩
	bool mDeadFlag;//���ʏu�Ԃ�true�ɂȂ�t���O(�ی�)

	string mHandRight;      //�C�g�̖��O�o�^
	string mHandLeft;       //�C���̖��O�o�^
	string mBody;           //�ԑ̖̂��O�o�^
	string num;             //string�^�̎��ʔԍ�
	string mHandRNum;       //���ʔԍ�+�C�g�̖��O
	string mHandLNum;       //���ʔԍ�+�C���̖��O
	string mBodyNum;        //���ʔԍ�+�ԑ̖̂��O
	string EXPLOSION_EFFECT;//�G�t�F�N�g��
};