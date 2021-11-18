#pragma once
#include "BaseEnemy.h"
#include "../Render/ParticleManager.h"

class Timer;
class MemberEnemy;

class CEnemy : public BaseEnemy
{
public:
	CEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~CEnemy();

	//BaseEnemy����p��
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	virtual void Search() override;
	virtual void Warning() override;
	virtual void Attack() override;
	virtual void Destruct() override;

private:

	/*��x�����Ăяo���Ȃ�����*/
	void OnlyOnceAction();

	/*�����̈ړ��Ǘ�*/
	void MemberMove(int number);

private:

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;
	shared_ptr<Timer> mSearchTimer;//�����o�[���v���C���[���������鎞��

	std::vector<MemberEnemy*> mMemberList;     //�������X�g
	std::vector<Vector3> mMemberPositionList;//�����̂��߂̈ʒu���X�g

	Vector3 mSearchPosition;//���G���ʈʒu

	enum SearchCommandStep
	{
		USUALLY,            //�펞����
		FIND_PLAYER,        //�v���C���[�������Ă���
		SEARCH_ORDER,       //���G�J�n���w������
		RECEIVE_RESULT,     //���G���ʂ��󂯎��
		MOVE_RESULTPOSITION,//�񍐈ʒu�Ɍ�����
	};
	SearchCommandStep searchStep;

	bool mOnlyOnceTrigger;//��񂵂��Ăт����Ȃ������p
	bool mAnyDeathFlag;   //���X�g���̒N�������񂾂�
	bool mSearchCommand;  //���G�w��
	bool mSearchResult;   //���G�̌��ʂ��󂯎��

	int mEnemyNumber;//���f���`��p�̔ԍ�

	const int MEMBER_COUNT = 2;//�����̐�

	const float SEARCH_INTERVAL = 1.5f;
	const float MEMBER_DISTANCE = 30.0f;//�����o�[�Ƃ̋���

	string mCircle;      //�~�̃��f��
	string mCircleNumber;//�~�̃��f��

	//���s�A�j���[�V�����֘A
	float mLegRotate = 0.0f;//�r�̌��݂̉�]��
	const float LEG_SPEED = 1.5f; //�r�̉�]���x
	const float LEG_RANGE = 15.0f;//�r�̍ő��]��
	bool mRotDirection = false;   //��]����

	string tankBarrel2;
	string numBarrel2;
};