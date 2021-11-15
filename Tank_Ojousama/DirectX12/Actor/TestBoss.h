#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Timer;

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

	/*�A���ˌ�*/
	void RapidFire();

	/*�G������*/
	void Summon();

	/*�p�x���x�N�g���ɕϊ�*/
	Vector3 AngleToVectorY(float angle)const;

private:

	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;//�_������
	shared_ptr<Timer> mReloadTime;//�����[�h����
	shared_ptr<Timer> mSummonTime;//��������

	std::vector<Vector3> mSummonPoint;//�����ʒu

	const Vector3 FIRE_POSITION = Vector3(0, 30, 0);//�e�̔��ˈʒu

	const int MAX_HP = 200;       //�ő�̗�
	const int RAPIDFIRE_COUNT = 3;//�A���ˌ��̐�
	const int SUMMON_COUNT = 5;   //������
	int mBulletCount;//�e�𔭎˂�����
	int mCircleCount;//�x����\��������
	int mEnemyNumber;//���������G�̐�

	const float MOVE_SPEED = 0.0f; //�ړ����x
	const float AIMING_TIME = 1.0f;//�_������
	const float RELOAD_TIME = 0.5f;//���U����
	const float SUMMON_TIME = 5.0f;//������������
	float mFireAngle;//�e�����Ƃ��̌���

	bool mShotNormalBullet;
	bool mSummonEnemy;    //�G������������
	bool mDrawSummonPoint;//�������̌x����\��
	bool mSummonFlag;     //�����U�����s���Ƃ�true�ɂ���

	string mModelNum;

	string mModelName01 = "TestBoss";
	string mModelNumName01;

	//�x���p�I�u�W�F�N�g
	string mCircleNum;
	string mCircleName = "SummonCircle";
	string numCircle;

	std::vector<string> testNum;
	std::vector<string> testNumName;
};