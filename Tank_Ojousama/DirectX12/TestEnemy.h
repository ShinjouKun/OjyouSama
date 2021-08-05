#pragma once
#include <memory>
#include "BaseObject.h"//�I�u�W�F�N�g�̂ЂȌ^
#include "ObjectManager.h"//�I�u�W�F�N�g�Ǘ���
#include "ModelRenderer.h"//���f���\��t��
#include "TexRenderer.h"//�|���S���̕`��

/// <summary>
/// ��`�̎��E
/// </summary>
struct SenserFan
{
	Vector3 fanPoint;//���S�_
	float fanArea;  //�͈�
	float fanLength;//����
	float fanAngle;	//����p
};

/// <summary>
/// �v���C���[�ǐ՗p�Z���T�[
/// </summary>
struct SenserPoint
{
	Vector3 hitPoint;  //�����������S�_
	float senserRadius;//���a
};

/// <summary>
/// �G�N���X
/// </summary>
class TestEnemy : public BaseObject
{

//�֐��n
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="position">�����ʒu</param>
	/// <param name="angle">�����p�x</param>
	/// <param name="objManager">�I�u�W�F�N�g�Ǘ���</param>
	/// <param name="modelRender">���f���`��</param>
	/// <param name="texRender">�|���S���`��</param>
	/// <param name="num">���ʔԍ�</param>
	TestEnemy(
		Vector3 position,
		Vector3 angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<TexRenderer>texRender,
		int num
	);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TestEnemy();

	/// <summary>
	/// �ʏ�U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

private:

	//BaseObject����p��
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollision(const CollisonInfo & info) override;
	virtual void ImGuiDebug() override;

	/// <summary>
	/// �G�̃x�N�g�����擾
	/// </summary>
	/// <param name="vec">�G�̌���</param>
	/// <returns></returns>
	Vector3 GetEnemyVector(Vector3 vec);

//�ϐ��p
private:
	
	ObjectManager* mObj;
	shared_ptr<ModelRenderer> tankModel;
	shared_ptr<TexRenderer> tankSprite;
	Vector3 angleVector;
	SenserPoint senserPoint;

	//�X�e�[�^�X
	int HP;         //�̗�
	int attackCount;//�U���̃C���^�[�o��
	int hitCount;   //�H
	int bulletStock;//�e�̎c��

#pragma region �����o���p�炵��

	int number = 0;   //���ʔԍ�
	int count = 0;    //?
	float attackAngle;//�C���̕`��p�H
	float fireAngle;  //�C�g�̕`��p�H
	bool moveFlag;    //�ړ����Ă��邩
	bool frontMove;   //���ʂɈړ����Ă��邩
	float backMove;   //���Ɉړ����Ă��邩

	string tankAtk;   //�L�[���[�h�o�^
	string tankHou;   //?
	string tankBtm;   //�����H
	string num;       //�����p�i���o�[?
	string numNameAtk;//�ԍ��ƃL�[���[�h�̕���
	string numNameHou;
	string numNameBtm;

#pragma endregion

};