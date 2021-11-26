#pragma once
#include <memory>
#include "../BaseObject.h"             //�I�u�W�F�N�g�̂ЂȌ^
#include "../ObjectManager.h"          //�I�u�W�F�N�g�Ǘ���
#include "../../Render/ModelRenderer.h"//���f���\��t��
#include "../../Render/TexRenderer.h"	//�|���S���̕`��
#include "AttackArea.h"//�q�N���X�ō���Ă邩�炱���ɏ����Ă�

#include "EnemyAI.h"

class TestWayPoint;
class WayPointManager;
class ReportArea;
class BreadCrumbCreater;
class TestBreadCrumb;
class Timer;

/// <summary>
/// �G�̊��N���X
/// </summary>
class BaseEnemy : public BaseObject
{
protected:

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
		CHASE_PLAYER,//�v���C���[�ǐՒ�
		CHASE_BREADCRUMB,//�p�������ǐՒ�
		TURN_AROUND,//��]��
		REPORT,//�񍐒�
		RECEIVE_REPORT,
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

	/*�I�u�W�F�N�g�����܂Ƃ�()*/
	virtual void SearchObject();

	/*�ړ�(�ړ���̈ʒu)*/
	void Move(const Vector3& otherPosition);

	/*���G����(�_���[�W���󂯂Ȃ�����)*/
	virtual void Invincible(int time);

	/*��U����G�@�\(���E���x�܂ŉ�]���邩)*/
	virtual void SwingDirection(float range);

	/*�z����̃|�C���g�����Ԃɏ��񂷂�(�ʒu�̔z��,�z��̗v�f��)*/
	virtual void PatrolPoint(const Vector3 points[],int pointCount);

	/*�v���C���[ or �p������ ��ǂ�������*/
	virtual void TrackingObject();

	/*�m�����Ɏ�����ԂɕύX����(������ԂɂȂ�̗�,�������邩�ǂ���)*/
	virtual void DestructMode(int hpLine, bool destructMode);

	/*�����s��(objManager, modelRender)(�����蔻��I�u�W�F�N�g��Base���Ő�������)*/
	virtual void DestructAction(shared_ptr<ModelRenderer> modelRender);

	/*WayPoint�ǐՊJ�n(�񍐎�̈ʒu)*/
	virtual void InitSearch();

	///*WayPoint�ǐՂ����s(�K��InitSearch���s���Ă��Ȃ��Ƃ����Ȃ�)*/
	//virtual void SerachWayPoint();

	/*WayPoint�ϐ��̏�����*/
	virtual void InitWayPoint();

	/*WayPoint�Ɍ������Ĉړ��J�n*/
	virtual void WayPointMove();

	///*�񍐔͈͂�\������*/
	//virtual void Report(shared_ptr<ModelRenderer> modelRender);

	/*�p�x���x�N�g���ɕϊ�(Degree�p�x)(Y�͕ύX�Ȃ�)*/
	virtual Vector3 AngleToVectorY(float angle)const;

	/*�x�N�g�����p�x�ɕύX(���K���ς݂̃x�N�g��)(Y�͕ύX�Ȃ�)*/
	virtual float VectorToAngleY(const Vector3& vector) const;

	/*�ΏۂƂ̋������v��A�͈͓��Ȃ�true(�Ώۂ̈ʒu,�w�肵��������)*/
	virtual bool InsideDistance(const Vector3& otherPos, float dist);

	/*��̓����蔻��*/
	virtual bool IsHitFanToPoint(const FanInfomation& fan, const Vector3& point, const float radius = 0.0f) const;

	/*�p������������擾*/
	static void SetBreadCreator(BreadCrumbCreater * breadCreator);

	/*�̗͎擾*/
	int GetHP()const;

	/*���S�t���O�擾*/
	bool GetDeathFlag() const;

	/*�GAI���Z�b�g����*/
	static void SetEnemyAi(EnemyAI* enemyAI);

	/*�I�u�W�F�N�g�}�l�[�W���[�Z�b�g*/
	static void SetObjectManager(ObjectManager* manager);

private:

	/*�ϐ��̏�����*/
	void Initialize();

	/*��̏��ύX*/
	void SetFanInfo(float range = 60.0f, float length = 30.0f);

	/*�v���C���[����*/
	void SearchPlayer();

	/*�p����������*/
	void SearchBreadCrumbTest(const TestBreadCrumb& breadCrumb);

	/*�I�u�W�F�N�g�ǐՂɐ؂�ւ�*/
	void StartTracking();

	/*�p�������ǐ�*/
	void TrackingBreadcrumb();

	/*�U������@�\����*/
	void DicideTurnAround();

	/*�U��Ԃ�@�\(�U������̂ɂ����鎞��)*/
	void TurnAround(int time);

	///*�V�[�����WayPoint���擾���A���X�g�Ɋi�[*/
	//void GetAllWayPoint();

	///*�w��ʒu����ł��߂��|�C���g���擾(�w��ʒu)*/
	//shared_ptr<TestWayPoint> NearWayPointStartTest(const Vector3 & point) const;

	///*���g����ł��߂��|�C���gn��z��Ɋi�[(�i�[�����)*/
	//void SearchPointToArrayTest(int length);

	///*�z����ōł��Ώۂɋ߂�WayPoint���擾(�w��z��,�Ώۈʒu)*/
	//shared_ptr<TestWayPoint> NearWayPointArrayTest(const vector<shared_ptr<TestWayPoint>>& trans, const Vector3 & goal);

	///*�����ς݂ƈړ��s�\�t���O��������*/
	//void ClearFlag();

protected:

	//int HP;         //�̗�
	int number;     //���ʔԍ�
	int warningTime;//�x������
	int attackTime;	//�U������
	int attackCount;//�U���J�E���g

	float radius;         //�����蔻��̔��a
	float fanRotateOrigin;//��̍ŏ��̌���
	float swingRange;     //��U��p�x�͈̔�
	float barrelAngle;    //�C���̌���
	float turretAngle;    //�C�g�̌���
	float attackLength;   //�U���͈�
	
	bool trackingPlayer;    //�v���C���[��ǐՒ���
	bool trackingBreadcrumb;//�p���������E���Ă��邩
	bool breadcrumbMode;    //�p�������ǐՂ��s�����ǂ���
	bool DESTRUCT_MODE;     //�m�����Ɏ������邩�ǂ���
	bool TURNAROUND_MODE;   //�U���ɓ����������ɂ������U��������ǂ���
	bool moveWayPoint;      //WayPoint�ړ�����
	bool RECEIVEREPORT_MODE;//�񍐂��󂯎�邩�ǂ���
	bool moveFlag;          //�ړ����Ă��邩�ǂ���

	Vector3 scale;       //�傫��
	Vector3 mPlayerPosition;//�v���C���[�̈ʒu
	Vector3 mPreviousPosition;//�O�t���[���̈ʒu

	//key = ���ʔԍ��@�F�@value = �ʒu
	std::unordered_map<int, Vector3> breadMap;//�p���������X�g

	string tankBarrel;//�C�g�̖��O�o�^
	string tankTurret;//�C���̖��O�o�^
	string tankBody;  //�ԑ̖̂��O�o�^
	string num;       //string�^�̎��ʔԍ�
	string numBarrel; //���ʔԍ�+�C�g�̖��O
	string numTurret; //���ʔԍ�+�C���̖��O
	string numBody;	  //���ʔԍ�+�ԑ̖̂��O

private:
	int warningCount;   //�x���J�E���g
	int invincibleCount;//���G�J�E���g
	int destructCount;  //�����J�E���g
	int breadCount;     //���Ƃ����p�������̑���
	int patrolCount;    //���݂̏���ԍ�
	int searchCount;    //�g�p���郋�[�g�̑���
	int loopCount;      //���[�g�̑�������
	int moveCount;      //�������ׂ��I�u�W�F�N�g�̔ԍ�
	int arrayCount;     //�z��̗v�f��

	bool isInvincible;   //���G���Ԃ�
	bool hitSensor;      //�Z���T�[�������������H
	bool swingSensor;    //��U��̏��(true=���Afalse=�E)
	bool isDestruct;     //������ԂɂȂ��Ă��邩�ǂ����B
	bool finishSearchWay;//���[�g����������������
	bool hitReportArea;  //�񍐔͈͂ɓ���������
	//bool moveWayPoint;   //WayPoint�ړ�����
	bool goalFlag;       //�ړI�n�ɓ���������
	bool isActive;       //�s����Ԃ��ǂ���

	Vector3 lastBreadPos;//�p�������̍Ō�̈ʒu��ۑ��p
	Vector3 previousPos; //�O�t���[���̈ʒu(�p�x�ύX�p)
	Vector3 hitPos;      //�U�������������ʒu��ۑ�
	Vector3 hitAngle;    //�U�������������p�x��ۑ�
	Vector3 goalPoint;   //�񍐂��o�����I�u�W�F�N�g�̈ʒu
	Vector3 resultPoint; //�������ׂ�WayPoint�̈ʒu

	static BreadCrumbCreater* mBreadCreator;    //�p�������쐬�N���X
	static ObjectManager* mManager;
	AttackArea* destructArea;                   //�����͈̓N���X
	//shared_ptr<TestWayPoint> mWay;              //�����p
	//vector<shared_ptr<TestWayPoint>> mTarget;   //��������߂��|�C���g���i�[����
	//vector<shared_ptr<TestWayPoint>> mPointList;//�t�B�[���h�ɂ���|�C���g���Ǘ�
	std::vector<Vector3> moveList;              //���ۂɈړ�����ʒu���Ǘ�



	//ReportArea* mReportArea;//�񍐔͈̓N���X

	//int testNumber = 0;
	int mIntervalCount = 0;
	bool mDeathFlag = false;

	static EnemyAI * mEnemyAI;

};