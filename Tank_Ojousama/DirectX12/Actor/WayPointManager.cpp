#include "WayPointManager.h"

#include "TestWayPoint.h"
#include "WayPoint.h"

WayPointManager::WayPointManager(
	const Vector3 & pos,
	ObjectManager* objM,
	shared_ptr<ModelRenderer> mRender,
	const bool breadDraw)
{
	position = pos;
	objectManager = objM;
	modelRender = mRender;
	mBreadDraw = breadDraw;

#pragma region 初期化

	testWay = nullptr;
	testPointList.clear();
	pointNumber = 0;
	halfWidth = offsetWidth / 2.0f;
	halfHeight = offsetHeight / 2.0f;
	finishCreatePoint = false;

#pragma endregion

	CreateWayPoint();
}

WayPointManager::~WayPointManager()
{
}

void WayPointManager::CreateWayPoint()
{

	if (mBreadDraw)
	{
		for (int i = 0; i < offsetWidth; i += interval)
		{
			for (int j = 0; j < offsetHeight; j += interval)
			{
				//ここでリストに追加する
				Vector3 pos = position + Vector3(i - halfWidth, 0, j - halfHeight);
				objectManager->Add(new WayPoint(pos, objectManager, modelRender, pointNumber));
				pointNumber++;
			}
		}
	}



	for (int i = 0; i < offsetWidth; i += interval)
	{
		for (int j = 0; j < offsetHeight; j += interval)
		{
			//ここでリストに追加する
			Vector3 pos = position + Vector3(i - halfWidth, 0, j - halfHeight);

			testWay = std::make_shared<TestWayPoint>(pos);
			testPointList.push_back(testWay);
		}
	}

	finishCreatePoint = true;
}

void WayPointManager::AddTest(std::shared_ptr<TestWayPoint> point)
{
	testPointList.push_back(point);
}

void WayPointManager::Remove()
{
	testPointList.pop_back();
}

void WayPointManager::ResetFlag()
{
	for (int i = 0,end = testPointList.size(); i < end; i++)
	{
		testPointList[i]->FlagInit();
	}
}

std::vector<std::shared_ptr<TestWayPoint>> WayPointManager::GetTestPointList() const
{
	return testPointList;
}

bool WayPointManager::GetFinishFlag() const
{
	return finishCreatePoint;
}
