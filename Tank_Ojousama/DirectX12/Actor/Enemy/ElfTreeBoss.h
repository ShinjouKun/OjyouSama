#pragma once
#include "BaseEnemy.h"

class Timer;
class TreeRoot;
class Sound;
class SummonEnemy;
class Explosion;
class Hit;

class ElfTreeBoss : public BaseEnemy
{
public:

	ElfTreeBoss(
		const Vector3 & position,
		const Vector3 & angle,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~ElfTreeBoss();

	/*�J�����̃A�j���[�V�������I�����������󂯎��*/
	void EndCameraAnimation(bool value);

	/*�{�X�����񂾂��ǂ������m�F*/
	bool GetDeadFlag();

private:

	//BaseEnemy����p��
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*�U���p�^�[���̌���*/
	void ChangeAttackState();

	/*�A���ˌ�*/
	void RapidFire();

	/*�G������*/
	void Summon();

	/*��ʃ����S���Ƃ�*/
	void ManyAppleDrop();

	/*���S���̉��o*/
	void DeathAnimation();

	/*�{�莞�̐���*/
	void AngryGrowth();

	/*���Ȃ��甚������*/
	void DeathAnimation_Explosion();

	/*���ԂɂȂ����玀�S����*/
	void DeathAnimation_DeathCount();

#pragma region �������U���܂Ƃ�

	/*�������U��*/
	void RootAttack();

	/*�n�ʂɎ���c�b�R��*/
	void RootAtack_PierceHand();

	/*���������v���C���[��ǐՂ���*/
	void RootAtack_ChasePlayer();

	/*������ƒ�~*/
	void RootAtack_Wait();

	/*��������n�ʂ���o��*/
	void RootAtack_GoupRoot();

	/*��������n�ʂɖ߂�*/
	void RootAtack_GodownRoot();

#pragma endregion

	/*��x�����I�u�W�F�N�g�𐶐�*/
	void CreateObject();

private:

	//�������U���̏�ԑJ��
	enum RootAttackStep
	{
		PIERCE_HAND, //�n�ʂɎ���c�b�R��
		CHASE_PLAYER,//�v���C���[��ǐ�
		WAIT,        //��~
		GOUP_ROOT,   //���������o���čU��
		GODOWN_ROOT, //���������������߂�
	};
	RootAttackStep mRootStep;

	//���S�A�j���[�V�����̏�ԑJ��
	enum DeathAnimationStep
	{
		EXPLOSION,
		DEATH_COUNT,
	};
	DeathAnimationStep mDeathStep;


	shared_ptr<ParticleEmitterBox> mParticleEmitter;
	shared_ptr<Sound> mSmallExplosion;   //������
	shared_ptr<Sound> mBigExplosion;     //�唚��
	shared_ptr<Sound> mAttackSE;         //�U������SE
	shared_ptr<Sound> mDamageSE;         //�_���[�W���󂯂�����SE
	shared_ptr<Sound> mNoDeathSE;        //���G�̂Ƃ���SE
	shared_ptr<Sound> mDeathSE;          //���S�����Ƃ���SE
	shared_ptr<Hit> mDamageParticle;     //�_���[�W�p�p�[�e�B�N��
	shared_ptr<Explosion> mDeathParticle;//���S�p�p�[�e�B�N��

	shared_ptr<Timer> mAimingTime;    //�_������
	shared_ptr<Timer> mReloadTime;    //�����[�h����
	shared_ptr<Timer> mSummonTime;    //��������
	shared_ptr<Timer> mRootWaitTime;  //�������ҋ@����
	shared_ptr<Timer> mEffectInterval;//�G�t�F�N�g�̕`��Ԋu
	shared_ptr<Timer> mExplosionTime; //�G�t�F�N�g�̕`�掞��
	shared_ptr<Timer> mAppleDropTime; //�����S�𗎂Ƃ������鎞��
	shared_ptr<Timer> mAppleInterval; //�����S�𗎂Ƃ����o
	shared_ptr<Timer> mColorTimer;    //���G���ɃV�[���h�������o�鎞��

	TreeRoot * mTreeRoot;//�؂̍�
	TreeRoot * mTreeRoot2;//�؂̍�
	TreeRoot * mTreeRoot3;//�؂̍�
	TreeRoot * mTreeRoot4;//�؂̍�
	TreeRoot * mTreeRoot5;//�؂̍�

	std::vector<SummonEnemy*> mSummonList;//���������G�̃��X�g
	std::vector<Vector3> mSummonPoint;    //�����ʒu
	std::vector<string> mSummonName;      //�������郂�f���̖��O
	std::vector<string> mSummonNum;       //�������郂�f���̔ԍ�

	const Vector3 FIRE_POSITION = Vector3(0, 15, 0);//�e�̔��ˈʒu
	Vector3 mScale;
	Vector3 mPlayerPosition; //�v���C���[�̈ʒu
	Vector3 mOffsetRightHand;//�E��̌Œ�ʒu
	Vector3 mOffsetLeftHand; //����̌Œ�ʒu
	Vector3 mOffsetRootPos;  //�������̌Œ�ʒu
	Vector3 mRightHandPos;   //�E��̈ʒu
	Vector3 mLeftHandPos;    //����̈ʒu
	Vector3 mRootPosition;   //�������̈ʒu

	const int MAX_HP = 50;       //�ő�̗�
	const int RAPIDFIRE_COUNT = 5;//�A���ˌ��̐�
	const int SUMMON_COUNT = 5;   //������
	const int DEFENSE = 10;        //�h���
	int damagePool;  //�󂯂�_���[�W�̈ꎞ�ۑ�
	int mBulletCount;//�e�𔭎˂�����
	int mCircleCount;//�x����\��������
	int mEnemyNumber;//���������G�̐�
	int mActionCount;//�s���̐���

	const float SEARCH_LENGTH = 150.0f;//���G����
	const float MOVE_SPEED = 0.0f;    //�ړ����x
	const float AIMING_TIME = 0.5f;   //�_������
	const float RELOAD_TIME = 0.5f;   //���U����
	const float SUMMON_TIME = 3.0f;   //������������

	float mFireAngle;   //�e�����Ƃ��̌���
	float mRootSpeed;   //�������̈ړ����x
	float mSummonRotate;//�������̖��@�w�̉�]
	float mHandAngle;

	bool mShotBullet;        //�e���˂̏�ԊǗ�(�_��������)
	bool mSummonEnemy;       //�G������������
	bool mDrawSummonPoint;   //�������̌x����\��
	bool mCreateObject;      //�I�u�W�F�N�g�𐶐�������
	bool mFinishHandReturn;  //���肪���̈ʒu�ɖ߂������H
	bool mFinishRootReturn;  //�����������̈ʒu�ɖ߂������H
	bool mActionFlag;        //�U���A�N�V���������ǂ���
	bool mDeathAnimationFlag;//���S�A�j���[�V�������J�n���邩
	bool mDeadFlag;          //���ʑO��true�ɂȂ�t���O
	bool mOneShotSound;      //���SSE��炵�����ǂ���
	bool mSummonAlive;       //���������G�������Ă��邩
	bool mEndAnimation;      //�V�[���̓����A�j���[�V�������I����Ă��邩
	bool mAngryFlag;         //�{���Ԃ��ǂ���
	bool mAngryAnimFlag[3];  //�A�j���[�V�������I�����Ă��邩
	bool mColorChange;       //���G���̐F�ɕς��Ă��邩�ǂ���

	//����
	string mStringNum;
	string mModelName01 = "TestBoss";
	string mModelNumName01;
	//�E��
	string mHandRight = "RightHand";
	string mHandRightNum;
	//����
	string mHandLeft = "LeftHand";
	string mHandLeftNum;
	//�x���p�I�u�W�F�N�g
	string mSummonCircle = "SummonCircle";//�����p
	string mRootCircle = "RootCircle";    //�������U���p
	string mRootCircleNum;
	//�G�t�F�N�g
	string PARTICLE_EFFECT = "DeathBomm";
	//�G�t�F�N�g2
	string EXPLOSION_EFFECT = "Explosion";

	//�o���A
	string mBarrier = "Barrier";
	string mBarrierNum;
};