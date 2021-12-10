#pragma once
#include "BaseEnemy.h"

class MemberEnemy;

//�ґ��̑���
class CEnemy : public BaseEnemy
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
	CEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		int num,
		bool advanceFlag = false,
		const Vector3& advanceDirection = Vector3(0.0f, 0.0f, 1.0f)
	);

	~CEnemy();

	//BaseEnemy����p��
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

private:

	/*��x�����Ăяo���Ȃ�����*/
	void OnlyOnceAction();

	/*�����̈ړ��Ǘ�*/
	void MemberMove(int number);

	/*�����ւ̈ړ��w��*/
	void MoveOrder();

	/*�����ւ̍��G�w��*/
	void SearchOrder();

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

private:

	/*���S�A�j���[�V�����̑J�ڏ�*/
	enum DeathAnimationStep
	{
		RISE_SKY, //���ɕ����オ��
		EXPLOSION,//���ł���
	};
	DeathAnimationStep mDeathStep;

	/*�����̍��G�w��*/
	enum SearchCommandStep
	{
		USUALLY,            //�펞����
		FIND_PLAYER,        //�v���C���[�������Ă���
		SEARCH_ORDER,       //���G�J�n���w������
		RECEIVE_RESULT,     //���G���ʂ��󂯎��
		MOVE_RESULTPOSITION,//�񍐈ʒu�Ɍ�����
	};
	SearchCommandStep searchStep;

	vector<MemberEnemy*> mMemberList;   //�������X�g
	vector<Vector3> mMemberPositionList;//�����̂��߂̈ʒu���X�g

	shared_ptr<Timer> mSearchTimer;//�����o�[���v���C���[���������鎞��
	shared_ptr<Timer> mRiseTime;   //�㏸����
	shared_ptr<Timer> mDeathTime;  //���S�Ɏ��ʂ܂ł̎���
	shared_ptr<Sound> mAttackSE;   //�U������SE
	shared_ptr<Sound> mDamageSE;   //�_���[�W���󂯂�����SE
	shared_ptr<Sound> mDeathSE;    //���S�����Ƃ���SE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;//�����̃G�t�F�N�g

	Vector3 mSearchPosition;//���G���ʈʒu

	const float LEG_SPEED = 1.5f; //�r�̉�]���x
	const float LEG_RANGE = 15.0f;//�r�̍ő��]��
	const float SEARCH_INTERVAL = 1.0f;//���G�w�����s���Ԋu
	const float MEMBER_DISTANCE = 25.0f;//�����o�[�Ƃ̋���

	const int MEMBER_COUNT = 5;//�����̐�

	float mLegRotate = 0.0f;//�r�̌��݂̉�]��

	int bulletNumber = 0;//�e�̎��ʔԍ�
	int mEnemyNumber;    //���f���`��p�̔ԍ�

	bool mDeathAnimation; //���S�A�j���[�V�������J�n���邩
	bool mDeadFlag;       //���ʏu�Ԃ�true�ɂȂ�t���O(�ی�)
	bool mOnlyOnceTrigger;//��񂵂��Ăт����Ȃ������p
	bool mAnyDeathFlag;   //���X�g���̒N�������񂾂�
	bool mSearchCommand;  //���G�w��
	bool mSearchResult;   //���G�̌��ʂ��󂯎��
	bool mRotDirection;   //��]����

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