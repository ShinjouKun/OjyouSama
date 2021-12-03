#pragma once
#include <memory>
#include "../BaseObject.h"              //�I�u�W�F�N�g�̂ЂȌ^
#include "../ObjectManager.h"           //�I�u�W�F�N�g�Ǘ���
#include "../../Render/ModelRenderer.h" //���f���\��t��
#include "../../Render/TexRenderer.h"	//�|���S���̕`��
#include "../../Render/ParticleManager.h"//�p�[�e�B�N���`��
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
	enum MoveState
	{
		SEARCH,  //���G
		WARNING, //�x��
		ATTACK,  //�U��
		DESTRUCT,//����
		NOT_FIND,            //���������
		CHASE_PLAYER,        //�v���C���[�ǐՒ�
		CHASE_BREADCRUMB,    //�p�������ǐՒ�
		CHASE_ATTACKTARGET,  //�U�����_�ǐՒ�
		CHASE_TURRET,        //�^���b�g�ǐՒ�
		ADVANCE_BORDWERLINE, //�{�[�_�[���C���܂Œ����ړ���
		ADVANCE_ATTACKTARGET,//�U�����_�܂ňړ���
		TURN_AROUND,//��]��
		REPORT,//�񍐒�

	}mMoveState;

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

	virtual void Search() = 0; //���������
	virtual void Warning() = 0;//�������
	virtual void Attack() = 0; //�U����

	/*�s����Ԃ̕ύX*/
	void ChangeState();

	/*�I�u�W�F�N�g�����܂Ƃ�()*/
	virtual void SearchObject();

	/*�ړ�(�ړ���̈ʒu)*/
	void MovePoint(const Vector3& otherPosition);

	/*Y���𖳎������ړ�(�ړ���̈ʒu)*/
	void MovePointY(const Vector3& otherPosition);

	/*�ړ�(�ړ�����)*/
	void MoveDirection(const Vector3& direction);

	/*���G����(�_���[�W���󂯂Ȃ�����)*/
	virtual void Invincible(int time);

	/*�X���[�N�ɂ��Ӗڏ��*/
	virtual void SmokeBlind();

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

	/*�p������������擾------------------------------------------����Ȃ�-------------------------------------*/
	static void SetBreadCreator(BreadCrumbCreater * breadCreator);

	/*�̗͎擾*/
	int GetHP()const;

	/*���S�t���O�擾*/
	bool GetDeathFlag() const;

	/*�GAI���Z�b�g����*/
	static void SetEnemyAi(EnemyAI* enemyAI);

	/*�I�u�W�F�N�g�}�l�[�W���[�Z�b�g*/
	static void SetObjectManager(ObjectManager* manager);

	/*���_�i�U���[�h�̍U���Ώۂ̈ʒu���Z�b�g*/
	static void SetAttackTarget(const Vector3& attackTarget = Vector3(0.0f, 0.0f, 0.0f));

	/*�I�u�W�F�N�g�����ɕK�v�ȃf�[�^���Z�b�g(ObjectManager, ModelRenderer,ParticleManager)*/
	static void SetImportantObject(ObjectManager * manager, shared_ptr<ModelRenderer> modelRender, shared_ptr<ParticleManager> particleManager, shared_ptr<BreadCrumbCreater> breadCreator);


private:

	/*�ϐ��̏�����*/
	void Initialize();

	/*������Ԃ̊Ď�*/
	void AliveSurveillance();

	/*��̏��ύX*/
	void SetFanInfo(float range = 60.0f, float length = 30.0f);

	/*�v���C���[����*/
	void SearchPlayer();

	/*�U�����_����*/
	void SearchAttackTarget();

	/*�^���b�g����*/
	void SearchTurret();

	/*�p����������*/
	void SearchBreadCrumb(const TestBreadCrumb& breadCrumb);

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

	int mOriginNumber;//���ʔԍ�
	int warningTime;  //�x������
	int attackTime;	  //�U������
	int attackCount;  //�U���J�E���g

	float mRadius;         //�����蔻��̔��a
	float mFanRotateOrigin;//��̍ŏ��̌���
	float mSwingRange;     //��U��p�x�͈̔�
	float mFireAngle;     //�U���������
	float mAttackLength;   //�U���͈�
	
	bool mTrackingPlayer;    //�v���C���[��ǐՒ���
	bool mTrackingBreadcrumb;//�p���������E���Ă��邩
	bool mTrackingAttackArea;//���_�i�U����
	bool mTrackingTurret;    //�^���b�g�ǐՒ���
	bool breadcrumbMode;     //�p�������ǐՂ��s�����ǂ���
	bool DESTRUCT_MODE;      //�m�����Ɏ������邩�ǂ���
	bool TURNAROUND_MODE;    //�U���ɓ����������ɂ������U��������ǂ���
	bool moveWayPoint;       //WayPoint�ړ�����
	bool RECEIVEREPORT_MODE; //�񍐂��󂯎�邩�ǂ���
	bool moveFlag;           //�ړ����Ă��邩�ǂ���
	bool mAdvanceFlag = false;       //���_�i�U���[�h���H
	bool mHitBorderLine = false;     //���_�i�U���[�h�̃^�[�Q�b�g�ǐՊJ�n
	bool mAttackFlag = false;//�U���M���t���O

	bool mHitSmokeFlag = false;//�X���[�N�ɐG�ꂽ��

	Vector3 scale;       //�傫��
	Vector3 mPlayerPosition;//�v���C���[�̈ʒu
	Vector3 mPreviousPosition;//�O�t���[���̈ʒu
	Vector3 mAdvanceDirection;//���_�i�U���̐i�s����

	//key = ���ʔԍ��@�F�@value = �ʒu
	std::unordered_map<int, Vector3> breadMap;//�p���������X�g

	string mMyNumber;       //string�^�̎��ʔԍ�

	static ObjectManager* mManager;
	static std::shared_ptr<ModelRenderer> mRend;
	static std::shared_ptr<ParticleManager> mPart;

	//static std::shared_ptr<ParticleManager> mEffect;

	static Vector3 mAttackTarget;//���_�i�U���[�h�̃^�[�Q�b�g

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
	bool mHitSensor;      //�Z���T�[�������������H
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

	static std::shared_ptr<BreadCrumbCreater> mBreadCreator;    //�p�������쐬�N���X
	AttackArea* destructArea;                   //�����͈̓N���X
	std::vector<Vector3> moveList;              //���ۂɈړ�����ʒu���Ǘ�
	int mIntervalCount = 0;
	bool mDeathFlag = false;

	std::shared_ptr<Timer> mSmokeTimer;//�ڂ������Ȃ�����

	static EnemyAI * mEnemyAI;
};