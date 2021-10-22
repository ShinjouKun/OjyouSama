#pragma once
#include "../Math/Vector3.h"
#include "ObjectManager.h"
#include <memory>

class TestWayPoint;
class WayPointManager;

class EnemyAI
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="objectManager">�I�u�W�F�N�g�}�l�[�W���[</param>
	EnemyAI(/*ObjectManager* objectManager*/const shared_ptr<WayPointManager>& m);

	~EnemyAI();

	//void Initialize();
	//void Update();
	///*�v���C���[�̈ʒu���擾*/
	//Vector3 GetPlayerPosition()const;

	/*�}�l�[�W���[�擾*/
	//static void SetWayPointManager(WayPointManager * pointManager);

	/*�ړ����ׂ��i�s���[�g�̃��X�g��Ԃ�(�G�̈ʒu, �ړI�n)*/
	std::vector<Vector3> GetMoveRoad(const Vector3& enemyPosition,const Vector3& destination);

private:

	///*�v���C���[�̈ʒu����ɊĎ�*/
	//void WatchPlayerPosition();
	///*�G�l�~�[�̐�����Ԃ��Ď�*/
	//void WatchEnemyAlive(const int count);
	///*�G�̕\����Ԃ̐؂�ւ�*/
	//void SwitchingEnemyActive(const int count) ;
	///*�v���C���[�Ƃ̋������v�Z*/
	//bool SearchPlayer(const Vector3& enemyPosition, const float dist) const;

	/*���݈ʒu����ł��߂��|�C���g���擾(���݂̈ʒu)*/
	shared_ptr<TestWayPoint> NearWayPointToNowPosition(const Vector3& nowPosition);

	/*���g����߂��|�C���g�̒�����A�S�[���ɍł��߂��|�C���g��1�擾*/
	shared_ptr<TestWayPoint> NearWayPointToGoal(const vector<shared_ptr<TestWayPoint>>& nearPoints, const Vector3 & goalPosition);


private:

	//ObjectManager * mObjectManager;
	//Vector3 mPlayerPosition;//�v���C���[�̈ʒu
	////std::vector<BaseEnemy*> mEnemyList;//�G�̏����i�[
	//int enemyCount = 0;//�G�̐�
	//const float ACTIVE_DISTANCE = 150.0f;//�G�̕W���͈�
	//bool anyEnemyDeath = false;//�G�����񂾂��H


	shared_ptr<WayPointManager> manager;

	//static WayPointManager * mManager;
	bool mFindGoal;

	vector<shared_ptr<TestWayPoint>> mPointList;//�t�B�[���h�ɂ���|�C���g���Ǘ�

	//vector<shared_ptr<TestWayPoint>> mTarget;   //��������߂��|�C���g���i�[����
};