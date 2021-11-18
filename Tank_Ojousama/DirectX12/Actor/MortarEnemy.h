#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Timer;

//�Œ�C��̓G
class MortarEnemy :public BaseObject
{
public:

	MortarEnemy(
		const Vector3& pos,
		const Vector3& ang,
		ObjectManager * objectManager,
		shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~MortarEnemy();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

	/*�p�x���x�N�g���ɕϊ�*/
	Vector3 AngleToVectorY(float angle)const;

private:
	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;//�_������
	shared_ptr<Timer> mReloadTime;//�����[�h����

	Vector3 mScale;
	Vector3 mTargetPosition;

	enum AttackStep
	{
		AIMING,//�Ə����킹��
		FIRE,  //�e���ˁI
		RELOAD,//�����[�h��
	};
	AttackStep mAttackStep;//�U���p���

	float mRadius;
	float barrelAngle;
	float mHandAngle;//�r�̊p�x

	bool mFireFlag;//�e�𔭎˂�����

	int mBulletNumber;//���˂����e��

	string mHandRight;//�C�g�̖��O�o�^
	string mHandLeft;//�C���̖��O�o�^
	string mBody;  //�ԑ̖̂��O�o�^
	string num;       //string�^�̎��ʔԍ�
	string mHandRNum; //���ʔԍ�+�C�g�̖��O
	string mHandLNum; //���ʔԍ�+�C���̖��O
	string mBodyNum;	  //���ʔԍ�+�ԑ̖̂��O
};