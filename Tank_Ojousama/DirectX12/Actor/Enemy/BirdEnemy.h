#pragma once
#include "../BaseObject.h"
#include"../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

class Timer;

class BirdEnemy : public BaseObject
{
public:

	BirdEnemy(
		const Vector3& pos,
		const Vector3& ang,
		ObjectManager * objectManager,
		shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~BirdEnemy();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	void Move(const Vector3& targetPosition);
	void MoveY(const Vector3& targetPosition);

	void Action_Rising();
	void Action_Move(const Vector3& targetPosition);
	void Action_Fire();
	void Action_Back(const Vector3& targetPosition);
	void Action_Descend();
	void Action_Reload();

	bool InsideDistance(const Vector3& distance, const float length) const;

	/*�p�x���x�N�g���ɕϊ�*/
	Vector3 AngleToVectorY(float angle)const;

private:

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

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

	const float RISE_HEIGHT = 30.0f;//�㏸�ʂ̍���
	const float ATTACK_LENGTH = 50.0f;
	float mRadius;//���a
	float mFireAngle;
	float mRiseSpeed = 0.5f;

	bool mFireFlag;//�e�𔭎˂�����

	int mBulletNumber;//���˂����e��

	string mWingRight;//�C�g�̖��O�o�^
	string mWingLeft; //�C���̖��O�o�^
	string mBody;     //�ԑ̖̂��O�o�^
	string num;       //string�^�̎��ʔԍ�
	string mWingRNum; //���ʔԍ�+�C�g�̖��O
	string mWingLNum; //���ʔԍ�+�C���̖��O
	string mBodyNum;  //���ʔԍ�+�ԑ̖̂��O
};