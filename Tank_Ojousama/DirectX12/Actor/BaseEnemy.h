#pragma once
#include <memory>
#include "BaseObject.h"             //�I�u�W�F�N�g�̂ЂȌ^
#include "ObjectManager.h"          //�I�u�W�F�N�g�Ǘ���
#include "BreadCrumb.h"             //�p������
#include "../Render/ModelRenderer.h"//���f���\��t��
#include "../Render/TexRenderer.h"	//�|���S���̕`��

#include "AttackArea.h"

/// <summary>
/// �G�̊��N���X
/// </summary>
class BaseEnemy : public BaseObject
{
protected:
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
		DESTRUCT,//����
	}actionState;

public:
	BaseEnemy() = default;
	virtual ~BaseEnemy();

	//BaseObject�N���X����p��
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void OnCollison(BaseCollider* col) override;
	virtual void ImGuiDebug() override;

	//���̃N���X�̎q�ǂ����g���p���\�b�h
	virtual void EnemyInit() = 0;
	virtual void EnemyUpdate() = 0;
	virtual void EnemyRend() = 0;
	virtual void EnemyOnCollision(BaseCollider* col) = 0;
	virtual void EnemyImGuiDebug() = 0;

	virtual void Search() = 0; //���G
	virtual void Warning() = 0;//�ǐ�
	virtual void Attack() = 0; //�U��
	virtual void Destruct() = 0;//����

	/*�s����Ԃ̕ύX*/
	void ChangeState();

	/*�I�u�W�F�N�g�����܂Ƃ�(objManager)*/
	virtual void SearchObject(ObjectManager* objManager);

	/*�_���[�W����(�_���[�W��,objManager)*/
	virtual void Damage(int damage, ObjectManager* objManager);

	/*���G����(�_���[�W���󂯂Ȃ�����)*/
	virtual void Invincible(int time);

	/*��U����G�@�\(���E���x�܂ŉ�]���邩)*/
	virtual void SwingDirection(float range);

	/*�z����̃|�C���g�����Ԃɏ��񂷂�(�ʒu�̔z��,�z��̗v�f��)*/
	virtual void PatrolPoint(const Vector3 points[],int pointCount);

	/*�v���C���[ or �p������ ��ǂ�������*/
	virtual void TrackingObject();

	/*�m�����Ɏ�����ԂɕύX����(������ԂɂȂ�̗�,�������邩�ǂ���)*/
	void DestructMode(int hpLine, bool destructMode);

	/*�����s��(objManager, modelRender)(�����蔻��I�u�W�F�N�g��Base���Ő�������)*/
	virtual void DestructAction(ObjectManager * objManager, shared_ptr<ModelRenderer> modelRender);

	/*�p�x���x�N�g���ɕϊ�(Degree�p�x)(Y�͕ύX�Ȃ�)*/
	virtual Vector3 AngleToVectorY(float angle)const;

	/*�x�N�g�����p�x�ɕύX(���K���ς݂̃x�N�g��)(Y�͕ύX�Ȃ�)*/
	virtual float VectorToAngleY(const Vector3& vector) const;

	/*�ΏۂƂ̋������v��A�͈͓��Ȃ�true(�Ώۂ̈ʒu,�w�肵��������)*/
	virtual bool InsideDistance(const Vector3& otherPos, float dist);

	/*��̓����蔻��*/
	virtual bool IsHitFanToPoint(const FanInfomation& fan, const Vector3& point, float radius = 0.0f) const;

private:
	/*�ړ�(�ړ���̈ʒu)*/
	void Move(const Vector3& otherPosition);

	/*��̏��ύX*/
	void SetFanInfo(float range = 60.0f, float length = 30.0f);

	/*�v���C���[����(objManager)*/
	void SearchPlayer(ObjectManager* objManager);

	/*�p����������(objManager)*/
	void SearchBreadCrumb(BaseObject* breadcrumb);

	/*�I�u�W�F�N�g�ǐՂɐ؂�ւ�*/
	void StartTracking();

	/*�p�������ǐ�*/
	void TrackingBreadcrumb();

	/*�U������@�\����(objManager)*/
	void DicideTurnAround(ObjectManager* objManager);

	/*�U��Ԃ�@�\(�U������̂ɂ����鎞��)*/
	void TurnAround(int time);

	/*�����ύX(�g���ĂȂ�)*/
	void ChangeDirection();


protected:

	//key = ���ʔԍ��@�F�@value = �ʒu
	std::unordered_map<int, Vector3> breadMap;//�p���������X�g

	int HP;        //�̗�
	int mapCount = 0;  //�}�b�v�̗v�f��
	int breadCount = 0;//���Ƃ����p�������̑���
	int number = 0;//���ʔԍ�

	int warningTime;     //�x������
	int attackTime;	     //�U������
	int attackCount = 0;       //�U���J�E���g
	int warningCount = 0;      //�x���J�E���g
	int invincibleCount = 0;   //���G�J�E���g
	int destructCount = 0;     //�����J�E���g
	int turnCount = 0;         //�U������J�E���g
	int currentPointNumber = 0;//���݂̏���ԍ�

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
	bool isDestruct;        //������ԂɂȂ��Ă��邩�ǂ����B
	bool breadcrumbMode;    //�p�������ǐՂ��s�����ǂ���
	bool destructMode;      //�m�����Ɏ������邩�ǂ���
	bool turnaroundMode;    //�U���ɓ����������ɂ������U��������ǂ���

	Vector3 scale;       //�傫��
	Vector3 lastBreadPos;//�p�������̍Ō�̈ʒu��ۑ��p
	Vector3 previousPos; //�O�t���[���̈ʒu(�p�x�ύX�p)
	Vector3 hitPos = Vector3().zero;
	Vector3 hitAngle = Vector3().zero;

	string tankBarrel;//�C�g�̖��O�o�^
	string tankTurret;//�C���̖��O�o�^
	string tankBody;  //�ԑ̖̂��O�o�^
	string num;       //string�^�̎��ʔԍ�
	string numBarrel; //���ʔԍ�+�C�g�̖��O
	string numTurret; //���ʔԍ�+�C���̖��O
	string numBody;	  //���ʔԍ�+�ԑ̖̂��O

	AttackArea* destructArea;//�����͈̓N���X
};