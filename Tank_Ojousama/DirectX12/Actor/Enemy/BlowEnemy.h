#pragma once
#include "BaseEnemy.h"

class Sound;
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

	/*���G���*/
	virtual void Search() override;

	/*�ǐՏ��*/
	virtual void Warning() override;

	/*�U�����*/
	virtual void Attack() override;

private:

	/*�I�u�W�F�N�g����x������������*/
	void CreateOneObject();

private:
	//ObjectManager* objManager;                //�I�u�W�F�N�g�Ǘ���
	//shared_ptr<ModelRenderer> modelRender;	  //���f���`��
	//shared_ptr<ParticleManager> effectManager;//�G�t�F�N�g�`��

	AttackArea* attackArea;  //�U���͈̓N���X

	Vector3 patrolPoint[4];
	int pointCount;

	bool onTrigger;

	//���s�A�j���[�V�����֘A
	float mLegRotate = 0.0f;//�r�̌��݂̉�]��
	const float LEG_SPEED = 2.0f; //�r�̉�]���x
	const float LEG_RANGE = 15.0f;//�r�̍ő��]��
	bool mRotDirection = false;   //��]����

	string mLeftLeg;   //�����̃��f����
	string mLLegNumber;//���� + ���ʔԍ�
	string mRightLeg;  //�E���̃��f����
	string mRLegNumber;//�E�� + ���ʔԍ�
	string mHead;      //���̖��O
	string mHeadNumber;//�� + ���ʔԍ�
	string mBody;      //�̖̂��O
	string mBodyNumber;//�� + ���ʔԍ�

	std::shared_ptr<Sound> mSound;
	std::shared_ptr<Sound> mSE;
};