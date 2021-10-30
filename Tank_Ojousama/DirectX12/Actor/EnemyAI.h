#pragma once
#include "../Math/Vector3.h"
#include "ObjectManager.h"
#include <memory>

class TestWayPoint;
class WayPointManager;
class BaseEnemy;

class EnemyAI
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objectManager">オブジェクトマネージャー</param>
	EnemyAI(const shared_ptr<WayPointManager>& manager);

	~EnemyAI();

	/*更新！*/
	void Update();

	/*リストに敵を追加(敵生成時にここに追加してね！)*/
	void AddEnemyList(BaseEnemy * enemy);

	/*移動すべき進行ルートのリストを返す(敵の位置, 目的地)*/
	std::vector<Vector3> GetMoveRoad(const Vector3& enemyPosition,const Vector3& destination);

	void SetValue(const Vector3& position);

	std::vector<int> GetIDList() const;
	Vector3 GetPositionList() const { return pos; }


	///*移動すべき進行ルートのリストを返す(敵の位置, 目的地)*/
//void HitReportArea(const Vector3& enemyPosition, const Vector3& destination);

///*移動すべき進行ルートのリストを返す*/
//std::vector<Vector3> GetMoveList();

private:

	///*プレイヤーの位置を常に監視*/
	//void WatchPlayerPosition();
	/*エネミーの生存状態を監視*/
	void WatchEnemyAlive(const int count);
	///*敵の表示状態の切り替え*/
	//void SwitchingEnemyActive(const int count) ;
	///*プレイヤーとの距離を計算*/
	//bool SearchPlayer(const Vector3& enemyPosition, const float dist) const;

	/*現在位置から最も近いポイントを取得(現在の位置)*/
	shared_ptr<TestWayPoint> NearWayPointToNowPosition(const Vector3& nowPosition);

	/*自身から近いポイントの中から、ゴールに最も近いポイントを1つ取得*/
	shared_ptr<TestWayPoint> NearWayPointToGoal(const vector<shared_ptr<TestWayPoint>>& nearPoints, const Vector3 & goalPosition);

private:

	//ObjectManager * mObjectManager;
	//Vector3 mPlayerPosition;//プレイヤーの位置
	////std::vector<BaseEnemy*> mEnemyList;//敵の情報を格納
	//int enemyCount = 0;//敵の数
	//const float ACTIVE_DISTANCE = 150.0f;//敵の標示範囲
	bool anyEnemyDeath = false;//敵が死んだか？

	std::vector<BaseEnemy*> mEnemyList;
	std::vector<int> mNearEnemyID;
	std::vector<Vector3> mNearEnemyPosition;

	Vector3 pos;
	int num;
	bool mCalculation;//計算中かどうか

	shared_ptr<WayPointManager> mPointManager;

	//static WayPointManager * mManager;
	bool mFindGoal;

	vector<shared_ptr<TestWayPoint>> mPointList;//フィールドにあるポイントを管理

	//vector<shared_ptr<TestWayPoint>> mTarget;   //自分から近いポイントを格納する
};