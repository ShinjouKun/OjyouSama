#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"


#include "../Render/ParticleManager.h"

class Timer;

class MemberEnemy : public BaseObject
{
public:

	MemberEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~MemberEnemy();

	/*���S�������ǂ���*/
	bool GetDeadFlag() const;

	/*�ڕW�ʒu���������󂯎��*/
	void ReceivePosition(const Vector3& targetPosition);

	/*�U���Ώۂ̈ʒu�ƁA�U���w�߂��������󂯎��*/
	void ReceiveAttackCommand(const Vector3& attackTarget, const bool attackCommand);

	/*���G�Ώۂ̈ʒu���������󂯎��*/
	void ReceiveSearchCommand(const Vector3& searchTarget, const bool searchCommand);

	/*�����̎����󂯎��*/
	void ReceiveCaptainLost(const bool captainLost);

	/*���G���ʂ�����ɒm�点��*/
	bool SendSearchResult() const;

	/*���G���ʂ̈ʒu������ɒm�点��*/
	Vector3 SendSearchPosition() const;

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*�ڕW�ʒu�Ɉړ�&�߂��������~*/
	void MoveTarget(const Vector3& targetPosition, const float distance);

	/*�w�苗���ȓ����ǂ����𒲂ׂ�*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

	/*�p�x���x�N�g���ɕϊ�*/
	Vector3 AngleToVectorY(float angle)const;


#pragma region �U���̃X�e�b�v(�ʂ̂������@�����肻��)

	void AttackStep_NONE();
	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

#pragma endregion

private:

	enum AttackStep
	{
		NONE,  //�U�����ĂȂ�
		AIMING,//�Ə����킹��
		FIRE,  //�e���ˁI
		RELOAD,//�����[�h��
	};
	AttackStep mAttackStep;

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;//�_������
	shared_ptr<Timer> mReloadTime;//�����[�h����
	shared_ptr<Timer> mRandomMoveTimer;//���肶��ɂȂ�

	Vector3 scale;
	Vector3 mFixedPosition; //�Œ�ʒu
	Vector3 mAttackTarget;  //�U���ʒu
	Vector3 mSearchTarget;  //���G�ʒu
	Vector3 mSearchPosition;//���G���ʈʒu
	Vector3 mRandomDirection;//�����_���ړ�����
	Vector3 mPreviousPosition;

	float radius;//���a
	float barrelAngle;
	float turretAngle;
	const float SEARCH_RANGE = 30.0f;//���G�͈�(�~)

	int mBulletNumber = 0;//���˂����e��
	int testStep = 0;

	bool mDeadFlag;     //���S�������ǂ���
	bool mAttackCommand;//�U���w��
	bool mSearchCommand;//���G�w��
	bool mSearchResult; //�v���C���[�𔭌�������
	bool mCaptainLost;  //���������������ǂ���

	string tankBarrel;//�C�g�̖��O�o�^
	string tankTurret;//�C���̖��O�o�^
	string tankBody;  //�ԑ̖̂��O�o�^
	string num;       //string�^�̎��ʔԍ�
	string numBarrel; //���ʔԍ�+�C�g�̖��O
	string numTurret; //���ʔԍ�+�C���̖��O
	string numBody;	  //���ʔԍ�+�ԑ̖̂��O
};
