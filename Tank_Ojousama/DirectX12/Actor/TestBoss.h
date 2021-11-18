#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Timer;
class TreeRoot;

class TestBoss : public BaseObject
{
public:

	TestBoss(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~TestBoss();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*�U���p�^�[���̌���*/
	void ChangeAttackState();

	/*�A���ˌ�*/
	void RapidFire();

	/*�G������*/
	void Summon();

	/*���S���̉��o*/
	void DeathAnimation();

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

	/*�p�x���x�N�g���ɕϊ�*/
	Vector3 AngleToVectorY(float angle)const;

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

	enum DeathAnimationStep
	{
		EXPLOSION,
		DEATH_COUNT,
	};
	DeathAnimationStep mDeathStep;

	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	shared_ptr<Timer> mAimingTime;  //�_������
	shared_ptr<Timer> mReloadTime;  //�����[�h����
	shared_ptr<Timer> mSummonTime;  //��������
	shared_ptr<Timer> mRootWaitTime;//�������ҋ@����
	shared_ptr<Timer> mEffectInterval;//�G�t�F�N�g�̕`��Ԋu
	shared_ptr<Timer> mExplosionTime;//�G�t�F�N�g�̕`�掞��

	TreeRoot * mTreeRoot;//�؂̍�

	std::vector<Vector3> mSummonPoint;//�����ʒu
	std::vector<string> testNum;
	std::vector<string> testNumName;

	const Vector3 FIRE_POSITION = Vector3(0, 50, 0);//�e�̔��ˈʒu
	Vector3 mPlayerPosition; //�v���C���[�̈ʒu
	Vector3 mOffsetRightHand;//�E��̌Œ�ʒu
	Vector3 mOffsetLeftHand; //����̌Œ�ʒu
	Vector3 mOffsetRootPos;  //�������̌Œ�ʒu
	Vector3 mRightHandPos;   //�E��̈ʒu
	Vector3 mLeftHandPos;    //����̈ʒu
	Vector3 mRootPosition;   //�������̈ʒu

	const int MAX_HP = 200;       //�ő�̗�
	const int RAPIDFIRE_COUNT = 3;//�A���ˌ��̐�
	const int SUMMON_COUNT = 5;   //������
	int mBulletCount;//�e�𔭎˂�����
	int mCircleCount;//�x����\��������
	int mEnemyNumber;//���������G�̐�
	int mActionCount;//�s���̐���

	const float SEARCH_LENGTH = 150.0f;//���G����
	const float MOVE_SPEED = 0.0f;    //�ړ����x
	const float AIMING_TIME = 1.0f;   //�_������
	const float RELOAD_TIME = 0.5f;   //���U����
	const float SUMMON_TIME = 5.0f;   //������������
	float mFireAngle;//�e�����Ƃ��̌���
	float mRootSpeed;//�������̈ړ����x
	float mSummonRotate;//�������̖��@�w�̉�]
	float mHandAngle;

	bool mShotBullet;      //�e���˂̏�ԊǗ�(�_��������)
	bool mSummonEnemy;     //�G������������
	bool mDrawSummonPoint; //�������̌x����\��
	bool mCreateObject;    //�I�u�W�F�N�g�𐶐�������
	bool mFinishHandReturn;//���肪���̈ʒu�ɖ߂������H
	bool mFinishRootReturn;//�����������̈ʒu�ɖ߂������H

	bool mActionFlag;//�U���A�N�V���������ǂ���
	bool mDeathAnimationFlag;//���S�A�j���[�V�������J�n���邩

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
};