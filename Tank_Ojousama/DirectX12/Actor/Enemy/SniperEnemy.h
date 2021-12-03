#pragma once
#include "BaseEnemy.h"
#include "../../Render/ParticleManager.h"

class SniperEnemy : public BaseEnemy
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
	SniperEnemy(
		const Vector3& position,
		const Vector3& angle,
		int num,
		bool advanceFlag = false,
		const Vector3& advanceDirection = Vector3(0.0f, 0.0f, 1.0f)
	);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SniperEnemy();

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
	//ObjectManager* objManager;                //�I�u�W�F�N�g�Ǘ���
	//shared_ptr<ModelRenderer> modelRender;	  //���f���`��
	//shared_ptr<ParticleManager> effectManager;//�G�t�F�N�g�`��

	/*�e�̎��ʔԍ�*/
	int bulletNumber = 0;

	//���s�A�j���[�V�����֘A
	float mLegRotate = 0.0f;//�r�̌��݂̉�]��
	const float LEG_SPEED = 1.5f; //�r�̉�]���x
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
};