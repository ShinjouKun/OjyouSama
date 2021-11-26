#pragma once
#include "BaseEnemy.h"
#include "../../Render/ParticleManager.h"

#include "EnemyAI.h"

class Random;

//�ߋ����U���̓G
class BlowEnemy : public BaseEnemy
{
public:

	/// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="position">�����ʒu</param>
    /// <param name="angle">�����p�x</param>
    /// <param name="objManager">�I�u�W�F�N�g�Ǘ���</param>
    /// <param name="modelRender">���f���`��</param>
    /// <param name="texRender">�摜�`��</param>
    /// <param name="effect">�G�t�F�N�g�`��</param>
    /// <param name="num">���ʔԍ�</param>
	BlowEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<TexRenderer>texRender,
		shared_ptr<ParticleManager> effect,
		int num
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

	/*�������*/
	virtual void Destruct() override;

private:

	/*�I�u�W�F�N�g����x������������*/
	void CreateOneObject();

private:
	ObjectManager* objManager;                //�I�u�W�F�N�g�Ǘ���
	shared_ptr<ModelRenderer> modelRender;	  //���f���`��
	shared_ptr<ParticleManager> effectManager;//�G�t�F�N�g�`��

	AttackArea* attackArea;  //�U���͈̓N���X

	Vector3 patrolPoint[4];
	int pointCount;

	bool onTrigger;

	//���s�A�j���[�V�����֘A
	float mLegRotate = 0.0f;//�r�̌��݂̉�]��
	const float LEG_SPEED = 1.5f; //�r�̉�]���x
	const float LEG_RANGE = 15.0f;//�r�̍ő��]��
	bool mRotDirection = false;   //��]����

	shared_ptr<EnemyAI> mEnemyAI;

	string tankBarrel2;
	string numBarrel2;
};