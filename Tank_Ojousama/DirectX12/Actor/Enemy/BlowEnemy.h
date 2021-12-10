#pragma once
#include "BaseEnemy.h"

//�ߋ����U���̓G
class BlowEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="position">�����ʒu</param>
	/// <param name="angle">�����p�x</param>
	/// <param name="num">���ʔԍ�</param>
	/// <param name="advanceFlag">���_�i�s���[�h���H</param>
	/// <param name="advanceDirection">���_�i�s���ɐi�ޕ���</param>
	BlowEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		int num,
		bool advanceFlag = false,
		const Vector3& advanceDirection = Vector3(0.0f, 0.0f, 1.0f)
	);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BlowEnemy();

private:

	//BaseEnemy����p��
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*������Ԃ��Ď�*/
	void CheckAlive();

	/*�ړ�*/
	void Move();

	/*���s�A�j���[�V����*/
	void MoveAnimation();

	/*�U��*/
	void Attack();

	/*���S���̉��o*/
	void DeathAnimation();

	/*��ɏ㏸*/
	void DeathAnimeStep_RiseSky();

	/*����&���b��~*/
	void DeathAnimeStep_Explosion();

	/*�I�u�W�F�N�g����x������������*/
	void CreateOneObject();

private:

	/*���S�A�j���[�V�����̑J�ڏ�*/
	enum DeathAnimationStep
	{
		RISE_SKY, //���ɕ����オ��
		EXPLOSION,//���ł���
	};
	DeathAnimationStep mDeathStep;

	AttackArea* attackArea;  //�U���͈̓N���X
	Vector3 patrolPoint[4];

	shared_ptr<Timer> mRiseTime; //�㏸����
	shared_ptr<Timer> mDeathTime;//���S�Ɏ��ʂ܂ł̎���
	shared_ptr<Sound> mAttackSE; //�U������SE
	shared_ptr<Sound> mDamageSE; //�_���[�W���󂯂�����SE
	shared_ptr<Sound> mDeathSE;  //���S�����Ƃ���SE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;//�����̃G�t�F�N�g

	const float LEG_SPEED = 2.0f; //�r�̉�]���x
	const float LEG_RANGE = 15.0f;//�r�̍ő��]��

	float mLegRotate;//�r�̌��݂̉�]��

	int pointCount;//���݌������Ă������|�C���g

	bool mDeathAnimation;//���S�A�j���[�V�������J�n���邩
	bool mDeadFlag;      //���ʏu�Ԃ�true�ɂȂ�t���O(�ی�)
	bool onTrigger;      //��x�����Ă΂��g���K�[
	bool mRotDirection;  //��]����

	string mLeftLeg;        //�����̃��f����
	string mLLegNumber;     //���� + ���ʔԍ�
	string mRightLeg;       //�E���̃��f����
	string mRLegNumber;     //�E�� + ���ʔԍ�
	string mHead;           //���̖��O
	string mHeadNumber;     //�� + ���ʔԍ�
	string mBody;           //�̖̂��O
	string mBodyNumber;     //�� + ���ʔԍ�
	string mHand;           //��̖��O
	string mHandNumber;     //�� + ���ʔԍ�
	string EXPLOSION_EFFECT;//�G�t�F�N�g��
};