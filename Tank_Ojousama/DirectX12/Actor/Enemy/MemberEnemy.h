#pragma once
#include "../BaseObject.h"
#include"../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

class Timer;
class Sound;

//�ґ��̏����̓G
class MemberEnemy : public BaseObject
{
public:

	MemberEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		std::shared_ptr<ModelRenderer> modelRender,
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

	/*������Ԃ��Ď�*/
	void CheckAlive();

	/*�ړ�*/
	void Move();

	/*���s�A�j���[�V����*/
	void MoveAnimation();

	/*�U��*/
	void Attack();

	/*�����������č���*/
	void Confusion();

	/*�ڕW�ʒu�Ɉړ�&�߂��������~*/
	void MoveTarget(const Vector3& targetPosition, const float distance);

	/*�w�苗���ȓ����ǂ����𒲂ׂ�*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

	/*�p�x���x�N�g���ɕϊ�*/
	Vector3 AngleToVectorY(float angle)const;

	/*���S���̉��o*/
	void DeathAnimation();

	/*��ɏ㏸*/
	void DeathAnimeStep_RiseSky();

	/*����&���b��~*/
	void DeathAnimeStep_Explosion();


#pragma region �U���̃X�e�b�v(�ʂ̂������@�����肻��)

	void AttackStep_NONE();
	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

#pragma endregion

private:

	/*�U���̏�ԑJ��*/
	enum AttackStep
	{
		NONE,  //�U�����ĂȂ�
		AIMING,//�Ə����킹��
		FIRE,  //�e���ˁI
		RELOAD,//�����[�h��
	};
	AttackStep mAttackStep;

	/*���S�A�j���[�V�����̑J�ڏ�*/
	enum DeathAnimationStep
	{
		RISE_SKY, //���ɕ����オ��
		EXPLOSION,//���ł���
	};
	DeathAnimationStep mDeathStep;

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;     //�_������
	shared_ptr<Timer> mReloadTime;     //�����[�h����
	shared_ptr<Timer> mRandomMoveTimer;//���肶��ɂȂ�
	shared_ptr<Timer> mRiseTime;       //�㏸����
	shared_ptr<Timer> mDeathTime;      //���S�Ɏ��ʂ܂ł̎���
	shared_ptr<Sound> mAttackSE;       //�U������SE
	shared_ptr<Sound> mDamageSE;       //�U������SE
	shared_ptr<Sound> mDeathSE;        //���S�����Ƃ���SE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	Vector3 scale;
	Vector3 mFixedPosition; //�Œ�ʒu
	Vector3 mAttackTarget;  //�U���ʒu
	Vector3 mSearchTarget;  //���G�ʒu
	Vector3 mSearchPosition;//���G���ʈʒu
	Vector3 mRandomDirection;//�����_���ړ�����
	Vector3 mPreviousPosition;


	const float SEARCH_RANGE = 30.0f;//���G�͈�(�~)
	const float LEG_SPEED = 1.5f; //�r�̉�]���x
	const float LEG_RANGE = 15.0f;//�r�̍ő��]��

	float mRadius;   //���a
	float mFireAngle;//�U�����̌���
	float mLegRotate;//�r�̌��݂̉�]��

	int mBulletNumber = 0;//���˂����e��
	int mMoveDirection = 0;//�����_���ړ����̈ړ�����

	bool mAttackCommand; //�U���w��
	bool mSearchCommand; //���G�w��
	bool mSearchResult;  //�v���C���[�𔭌�������
	bool mCaptainLost;   //���������������ǂ���
	bool mRotDirection;  //��]����
	bool mDeathAnimation;//���S�A�j���[�V�������J�n���邩
	bool mDeadFlag;      //���ʏu�Ԃ�true�ɂȂ�t���O(�ی�)

	string mMyNumber;       //���ʔԍ�
	string mLeftLeg;        //�����̃��f����
	string mLLegNumber;     //���� + ���ʔԍ�
	string mRightLeg;       //�E���̃��f����
	string mRLegNumber;     //�E�� + ���ʔԍ�
	string mHead;           //���̖��O
	string mHeadNumber;     //�� + ���ʔԍ�
	string mBody;           //�̖̂��O
	string mBodyNumber;     //�� + ���ʔԍ�
	string EXPLOSION_EFFECT;//�G�t�F�N�g��
};
