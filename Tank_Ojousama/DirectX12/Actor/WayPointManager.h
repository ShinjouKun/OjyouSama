#pragma once
#include <vector>
#include "ObjectManager.h"
#include "../Render/ModelRenderer.h"

class TestWayPoint;

class WayPointManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">中心位置</param>
	/// <param name="objM">オブジェクトマネージャー</param>
	/// <param name="mRender">モデル描画</param>
	WayPointManager(
		const Vector3 & pos,
		ObjectManager* objM,
		std::shared_ptr<ModelRenderer> mRender,
		const bool breadDraw
	);

	~WayPointManager();

	/*WayPointを作成*/
	void CreateWayPoint();

	/*リストの末尾に要素を追加*/
	void AddTest(std::shared_ptr<TestWayPoint> point);

	/*リストの末尾の要素を削除*/
	void Remove();

	//全てのポイントのフラグを初期化
	void ResetFlag();

	/*PointListを取得*/
	std::vector<std::shared_ptr<TestWayPoint>> GetTestPointList()const;

	bool GetFinishFlag()const;

private:
	ObjectManager * objectManager;
	std::shared_ptr<ModelRenderer> modelRender;

	std::shared_ptr<TestWayPoint> testWay;//各ポイント
	std::vector<std::shared_ptr<TestWayPoint>> testPointList;//ポイントのリスト

	Vector3 position;

	int pointNumber = 0;

	int interval = 20;//配置間隔
	float offsetWidth = 100.0f;
	float offsetHeight = 100.0f;
	float halfWidth;
	float halfHeight;

	bool finishCreatePoint;//生成が完了したか？
	bool mBreadDraw;
};