#pragma once
#include <memory>
#include "BaseObject.h"    //�I�u�W�F�N�g�̂ЂȌ^
#include "ObjectManager.h" //�I�u�W�F�N�g�Ǘ���
#include "../Render/ModelRenderer.h"//���f���\��t��
#include "../Render/TexRenderer.h"	 //�|���S���̕`��


#include "BreadCrumb.h"
#include "AttackArea.h"

/// <summary>
/// �G�N���X
/// </summary>
class TestEnemy : public BaseObject
{
private:

	//�v���C���[�̏��
	struct SearchPoint
	{
		Vector3 position;//���S�_
		float radius;    //���a
	}searchPoint;

	//��̍��G�͈�
	struct FanInfomation
	{
		Vector3 position;
		float fanRange;//�p�x
		float length;  //����
		float rotate;  //��]�p�x
	}fanInfo;

	//�G�̍s�����
	enum ActionState
	{
		SEARCH,  //���G
		WARNING, //�x��
		ATTACK,  //�U��
	}actionState;

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

private:

	//BaseObject����p��
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* col) override;
	virtual void ImGuiDebug() override;

	/// <summary>
	/// ��̏����Đݒ�
	/// </summary>
	/// <param name="range">�Ȃ��p�̊p�x</param>
	/// <param name="length">����</param>
	void SetFanInfo(float range = 60.0f, float length = 30.0f);

	/// <summary>
	/// �ړ�
	/// </summary>
	/// <param name="otherPosition">�ړ��Ώ�</param>
	void Move(const Vector3& otherPosition);

	/// <summary>
	/// ��ԕύX
	/// </summary>
	void ChangeState();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	void SearchObject();

	/// <summary>
    /// �v���C���[���G
    /// </summary>
    /// <param name="player"></param>
	void SearchPlayer(BaseObject* player);

	void SearchPlayer();

	/// <summary>
	/// �p���������G
	/// </summary>
	void SearchBreadCrumb(BaseObject* breadcrumb);

	/// <summary>
	/// �p��������H��
	/// </summary>
	void TrackingBreadcrumb();

	/// <summary>
	/// ���G
	/// </summary>
	void Search();

	/// <summary>
	/// �x��
	/// </summary>
	void Warning();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// ���G����
	/// </summary>
	/// <param name="time">�w�莞��</param>
	void Invincible(int time);

	/// <summary>
	/// �ړ������������悤�ɒ���
	/// </summary>
	void ChangeDirection();

	/// <summary>
	/// Y���Œ�p�x���x�N�g���ɕϊ�����
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <returns>�p�x�̒P�ʃx�N�g��</returns>
	Vector3& AngleToVectorY(float angle) const;

	/// <summary>
	/// Y���Œ�Ńx�N�g�����p�x�ɕϊ�����
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	float VectorToAngleY(const Vector3& vector) const;

	/// <summary>
	/// ��Ɠ_�̓����蔻��
	/// </summary>
	/// <param name="fan">��</param>
	/// <param name="point">�_</param>
	/// <returns>�������Ă�����true</returns>
	bool IsHitFanToPoint(const FanInfomation& fan, const Vector3& point, float radius = 0.0f) const;

	//�ϐ��p
private:

	ObjectManager* objManager;
	shared_ptr<ModelRenderer> modelRender;
	//key = ���ʔԍ��@�F�@value = �ʒu
	std::unordered_map<int, Vector3> breadMap;//�p���������X�g

	int HP;        //�̗�
	int mapCount;  //�}�b�v�̗v�f��
	int breadCount;//���Ƃ����p�������̑���
	int number = 0;//���ʔԍ�

	int warningCount;   //�x���J�E���g
	int warningTime;    //�x������
	int attackCount;    //�U���J�E���g
	int attackTime;	    //�U������
	int invincibleCount;//���G�J�E���g

	float radius;         //�����蔻��̔��a
	float fanRotateOrigin;//��̍ŏ��̌���
	float swingRange;     //��U��p�x�͈̔�
	float barrelAngle;    //�C���̌���
	float turretAngle;    //�C�g�̌���
	float attackLength;   //�U���͈�

	bool isDamage;          //�_���[�W���󂯂Ă��邩
	bool isInvincible;      //���G���Ԃ�
	bool hitSensor;         //�Z���T�[�������������H
	bool swingSensor;       //��U��̏��(true=���Afalse=�E)
	bool trackingPlayer;    //�v���C���[�ɓ������Ă��邩
	bool trackingBreadcrumb;//�p���������E���Ă��邩
	bool frontMove;         //���ʂɈړ����Ă��邩
	bool backMove;          //���Ɉړ����Ă��邩
	bool oneShot;           //�I�u�W�F�N�g����x������������B

	Vector3 scale;       //�傫��(1.5)
	Vector3 lastBreadPos;//�p�������̍Ō�̈ʒu��ۑ��p
	Vector3 previousPos; //�O�t���[���̈ʒu(�p�x�ύX�p)

	string tankBarrel;//�C�g�̖��O�o�^
	string tankTurret;//�C���̖��O�o�^
	string tankBody;  //�ԑ̖̂��O�o�^
	string num;       //string�^�̎��ʔԍ�
	string numBarrel; //���ʔԍ�+�C�g�̖��O
	string numTurret; //���ʔԍ�+�C���̖��O
	string numBody;	  //���ʔԍ�+�ԑ̖̂��O

	AttackArea* area;//�U������N���X(�����ł���)
};