#pragma once
#include "../BaseObject.h"
#include"../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

#include "BaseEnemy.h"

class Timer;

class BirdEnemy : public BaseEnemy
{
public:

	BirdEnemy(
		const Vector3& pos,
		const Vector3& ang,
		int num
	);

	~BirdEnemy();

private:

	//virtual void Init() override;
	//virtual void Update() override;
	//virtual void Rend() override;
	//virtual void ImGuiDebug() override;
	//virtual void OnCollison(BaseCollider * col) override;

	//BaseEnemy����p��
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*���G���*/
	virtual void Search() override;

	/*�ǐՏ��*/
	virtual void Warning() override;

	/*�U�����*/
	virtual void Attack() override;

	void Move(const Vector3& targetPosition);
	void MoveY(const Vector3& targetPosition);

	void Action_Rising();
	void Action_Move(const Vector3& targetPosition);
	void Action_Fire();
	void Action_Back(const Vector3& targetPosition);
	void Action_Descend();
	void Action_Reload();

	bool InsideDistanceY(const Vector3& distance, const float length) const;

	/*�p�x���x�N�g���ɕϊ�*/
	Vector3 AngleToVectorY(float angle)const;

private:

	//ObjectManager * mObjManager;
	//shared_ptr<ModelRenderer> mModelRender;
	//shared_ptr<ParticleManager> mEffectManager;

	std::shared_ptr<Timer> mIntervalTime;
	std::shared_ptr<Timer> mReloadTime;

	Vector3 mScale;
	Vector3 mTargetPosition;//�v���C���[�̈ʒu
	Vector3 mOffsetPosition;//�A��ʒu
	Vector3 mOffsetRisePosition;//�A��ʒu�̐^��

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

	const float RISE_HEIGHT = 20.0f;//�㏸�ʂ̍���
	const float ATTACK_LENGTH = 50.0f;
	float mRadius;//���a
	float mRiseSpeed = 0.5f;//�㏸�̑���

	float mLegRotate = 0.0f;//�r�̉�]
	const float LEG_SPEED = 10.0f;//�r�̉�]���x
	const float LEG_RANGE = 100.0f;//�r�̍ő��]��
	bool mFinishAnimation = false;//�A�j���[�V�������I�����Ă��邩
	bool mFireFlag;//�e�𔭎˂�����

	int mBulletNumber;//���˂����e��

	string num;       //string�^�̎��ʔԍ�
	string mBullet;//�C�g�̖��O�o�^
	string mLeg; //�C���̖��O�o�^
	string mBody;     //�ԑ̖̂��O�o�^
	string mBulletNum; //���ʔԍ�+�C�g�̖��O
	string mLegNum; //���ʔԍ�+�C���̖��O
	string mBodyNum;  //���ʔԍ�+�ԑ̖̂��O
};