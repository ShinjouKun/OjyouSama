#include "EnemyAI.h"
#include "../TestWayPoint.h"
#include "../WayPointManager.h"
#include "BaseEnemy.h"

EnemyAI::EnemyAI(const shared_ptr<WayPointManager>& manager)
{
	mPointManager = manager;
	num = 0;
	mCalculation = false;
}

EnemyAI::~EnemyAI()
{
}
void EnemyAI::Update()
{
	//WatchPlayerPosition();//プレイヤーの位置を監視
	//敵を一括検索
	for (int i = 0, end = static_cast<int>(mEnemyList.size()); i < end; i++)
	{
		//int debugHP = mEnemyList[i]->GetHP();
		//ImGui::SliderInt("HP-------------------", &debugHP, 0, 500);

		anyEnemyDeath = false;

		if (mEnemyList[i]->GetDeathFlag())
		{

			mEnemyList.erase(mEnemyList.begin() + i);

			/*このまま続けると、指定番号の要素がないって言われるから、
			　一回外に出てから、もう一度ループを行う*/
			anyEnemyDeath = true;
		}

		if (anyEnemyDeath)
		{
			break;
		}


		if (mCalculation)
		{
			Vector3 distance = mEnemyList[i]->GetPosition() - pos;
			float length = distance.Length();

			if (length < 30.0f)
			{
				//4:報告主との距離が近い敵のIDを取得し、リストに格納する
				// -この時、報告主の位置も忘れずに記憶する→BaseEnemy.Updateへ
				mNearEnemyID.push_back(static_cast<int>(mEnemyList[i]->GetID()));
				mNearEnemyPosition.push_back(mEnemyList[i]->GetPosition());
			}
		}
		//else
		//{
		//	//値を受け取っていないとき
		//	//自分の位置を入れておく。
		//	pos = mEnemyList[i]->GetPosition();
		//}
	}


	//for文を抜けたら、計算が終わったとする。
	mCalculation = false;
	mNearEnemyPosition.clear();

	//int debugInt = mNearEnemyID.size();
	//ImGui::SliderInt("MP-------------------", &debugInt, 0, 500);
}

void EnemyAI::AddEnemyList(BaseEnemy * enemy)
{
	mEnemyList.push_back(enemy);
}

std::vector<Vector3> EnemyAI::GetMoveRoad(const Vector3& enemyPosition, const Vector3& destination)
{
	Vector3 goalPoint = destination;

	std::vector<Vector3> moveList;//移動リスト

	mFindGoal = false;//ゴールを見つけたかどうか

	int searchCount = 0;//検索した回数

#pragma region 当たった時に一度だけ行う処理

	//1:目的地の位置に新しいポイントを生成
	shared_ptr<TestWayPoint> wayPoint = std::make_shared<TestWayPoint>(goalPoint);
	//mManager->AddTest(wayPoint);
	mPointManager->AddTest(wayPoint);

	//2:フィールドにあるポイントを全て取得
	//2-1:ポイントのフラグを初期化
	//mManager->ResetFlag();
	mPointManager->ResetFlag();
	//mPointList.resize(mManager->GetTestPointList().size());//最初にサイズの変更
	mPointList.resize(mPointManager->GetTestPointList().size());//最初にサイズの変更
	//2-2:リストにコピー(重い)
	//mPointList = mManager->GetTestPointList();
	mPointList = mPointManager->GetTestPointList();
	//2-3:移動リストのメモリを確保
	moveList.reserve(mPointList.size());
	//2-4:不必要なメモリの削除
	//mManager->Remove();//←1で追加したやつは、もう使わないので削除する
	mPointManager->Remove();//←1で追加したやつは、もう使わないので削除する
	wayPoint.reset();

	//3:自身から近いポイントを格納するリストを生成
	vector<shared_ptr<TestWayPoint>> nearPoints(4);

	//4:最初の検索を行う位置を決定
	moveList.insert(moveList.begin(), enemyPosition);

#pragma endregion

#pragma region ルートを検索

	//ゴールを発見できるまでループする
	while (!mFindGoal)
	{
		//1:自分の位置から、最も近いポイントを指定数取得
		for (int i = 0, end = static_cast<int>(nearPoints.size()); i < end; i++)
		{
			//検索する位置は、現在いるポイント
			nearPoints[i] = NearWayPointToNowPosition(moveList[searchCount]);
		}

		//2:自身から近いポイントの中から、ゴールに最も近いポイントを1つ取得
		Vector3 resultPosition = NearWayPointToGoal(nearPoints, goalPoint)->GetPosition();

		//3:指向回数を増やす
		searchCount++;

		//4:移動リストに格納する
		moveList.push_back(resultPosition);
	}

#pragma endregion

	return moveList;
}

void EnemyAI::SetValue(const Vector3 & position)
{
	//3:値を受け取ったら、全ての敵リストとの、距離の計算を開始する。
	mNearEnemyID.clear();
	mNearEnemyPosition.clear();

	pos = position;

	mCalculation = true;

	//for (int i = 0, end = static_cast<int>(mEnemyList.size()); i < end; i++)
	//{
	//	Vector3 distance = mEnemyList[i]->GetPosition() - pos;
	//	float length = distance.Length();

	//	if (length < 30.0f)
	//	{
	//		//4:報告主との距離が近い敵のIDを取得し、リストに格納する
	//		// -この時、報告主の位置も忘れずに記憶する→BaseEnemy.Updateへ
	//		mNearEnemyID.push_back(static_cast<int>(mEnemyList[i]->GetID()));
	//		mNearEnemyPosition.push_back(mEnemyList[i]->GetPosition());
	//	}
	//}
}

std::vector<int> EnemyAI::GetIDList() const
{
	if (mCalculation)
	{
		//計算中の時は空のリストを返す
		return std::vector<int>();
	}

	return mNearEnemyID;

}

//void EnemyAI::HitReportArea(const Vector3 & enemyPosition, const Vector3 & destination)
//{
//	Vector3 goalPoint = destination;//目的の位置を保存
//
//	std::vector<Vector3> moveList;//移動リスト
//
//	mFindGoal = false;//ゴールを見つけたかどうか
//
//	int searchCount = 0;//検索した回数
//
//#pragma region 当たった時に一度だけ行う処理
//
//	//1:目的地の位置に新しいポイントを生成
//	shared_ptr<TestWayPoint> wayPoint = std::make_shared<TestWayPoint>(goalPoint);
//	mPointManager->AddTest(wayPoint);
//
//	//2:フィールドにあるポイントを全て取得
//	//2-1:ポイントのフラグを初期化
//	mPointManager->ResetFlag();
//	mPointList.resize(mPointManager->GetTestPointList().size());//最初にサイズの変更
//	//2-2:リストにコピー(重い)
//	mPointList = mPointManager->GetTestPointList();
//	//2-3:移動リストのメモリを確保
//	moveList.reserve(mPointList.size());
//	//2-4:不必要なメモリの削除
//	mPointManager->Remove();//←1で追加したやつは、もう使わないので削除する
//	wayPoint.reset();
//
//	//3:自身から近いポイントを格納するリストを生成
//	vector<shared_ptr<TestWayPoint>> nearPoints(4);
//
//	//4:最初の検索を行う位置を決定
//	moveList.insert(moveList.begin(), enemyPosition);
//
//#pragma endregion
//}
//
//std::vector<Vector3> EnemyAI::GetMoveList()
//{
//	if (mFindGoal)
//	{
//		return 
//	}
//}

shared_ptr<TestWayPoint> EnemyAI::NearWayPointToNowPosition(const Vector3 & nowPosition)
{
	float distance = 0;    //対象との距離
	float mostDistance = 0;//最も近い距離
	shared_ptr<TestWayPoint> wayPoint;

	//ポイントリストを全検索して、最も近いポイントを1つ取得する
	for (int i = 0, end = static_cast<int>(mPointList.size()); i < end; i++)
	{
		//対象との距離を計算
		Vector3 dist = mPointList[i]->GetPosition() - nowPosition;
		distance = dist.Length();

		//まだ使用していないもののみ処理を行う
		if (!mPointList[i]->GetUseFlag())
		{
			if (mostDistance == 0 || mostDistance > distance)
			{
				mostDistance = distance;
				wayPoint = mPointList[i];
			}
		}
	}

	//使用済みに変更-----------------------------------エラーポイント------------------------------
	wayPoint->SetUseFlag(true);

	return wayPoint;
}

shared_ptr<TestWayPoint> EnemyAI::NearWayPointToGoal(const vector<shared_ptr<TestWayPoint>>& nearPoints, const Vector3 & goalPosition)
{
	float distance = 0;    //対象との距離
	float mostDistance = 0;//最も近い距離
	shared_ptr<TestWayPoint> wayPoint;

	for (int i = 0, end = static_cast<int>(nearPoints.size()); i < end; i++)
	{
		//ゴールと位置が一致するポイントを探す
		if (nearPoints[i]->GetPosition().x == goalPosition.x &&
			nearPoints[i]->GetPosition().y == goalPosition.y &&
			nearPoints[i]->GetPosition().z == goalPosition.z)
		{
			//ゴールを発見したら処理を終わる
			wayPoint = nearPoints[i];
			mFindGoal = true;
			break;
		}

		//対象との距離を計算
		Vector3 dist = nearPoints[i]->GetPosition() - goalPosition;
		distance = dist.Length();

		if (mostDistance == 0 || mostDistance > distance)
		{
			mostDistance = distance;
			wayPoint = nearPoints[i];
		}
	}

	//if (!finishSearchWay)
	//{
	//	//ゴールしてないときは使用済みにする
	//	wayPoint->SetUseFlag(true);
	//}

	return wayPoint;
}

//void EnemyAI::WatchPlayerPosition()
//{
//	//プレイヤーの位置を取得
//	mPlayerPosition = mObjectManager->GetPlayer().GetPosition();
//	float debugPlayerPosition[3] = { mPlayerPosition.x,mPlayerPosition.y,mPlayerPosition.z };
//	ImGui::SliderFloat3("PlayerPosition----------", debugPlayerPosition, -1000.0f, 1000.0f);
//}
//
void EnemyAI::WatchEnemyAlive(const int count)
{

	int debugHP = mEnemyList[count]->GetHP();
	ImGui::SliderInt("HP-------------------", &debugHP, 0, 500);

	anyEnemyDeath = false;

	if (mEnemyList[count]->GetHP() <= 0)
	{
		mEnemyList.erase(mEnemyList.begin() + count);

		/*このまま続けると、指定番号の要素がないって言われるから、
		　一回外に出てから、もう一度ループを行う*/
		anyEnemyDeath = true;
	}
}
//
//void EnemyAI::SwitchingEnemyActive(const int count)
//{
//	//プレイヤーとの距離が一定以下なら
//	if (SearchPlayer(mEnemyList[count]->GetPosition(), ACTIVE_DISTANCE))
//	{
//		//表示
//		mEnemyList[count]->SetActive(true);
//	}
//	else//そうでないなら
//	{
//		//非表示
//		mEnemyList[count]->SetActive(false);
//	}
//}
//
//bool EnemyAI::SearchPlayer(const Vector3& enemyPosition, const float dist) const
//{
//	//プレイヤーと自分の距離を計る
//	Vector3 distance = mPlayerPosition - enemyPosition;
//	float length = distance.Length();
//
//	ImGui::SliderFloat("Distance2Player", &length, -1000.0f, 1000.0f);
//
//	if (length > dist)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//Vector3 EnemyAI::GetPlayerPosition() const
//{
//	return mPlayerPosition;
//}