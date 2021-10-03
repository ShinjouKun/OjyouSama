#include "WayPoint.h"
//
////インスタンスを取得(これで出来てるかはわからないけど、何個も作るものではない！)
//WayPointManager * WayPointManager::GetInstance()
//{
//	WayPointManager instance;
//
//	return &instance;
//}
//
////フィールドにポイントをばらまく
//void WayPointManager::CreateWayPoint()
//{
//	for (int i = 0; i < offsetWidth; i++)
//	{
//		for (int j = 0; j < offsetHeight; j++)
//		{
//			wayPoint.position = Vector3(i - halfWidth, 0,i - halfHeight);
//
//			//フラグの初期化
//			ResetFlag();
//
//			//これでちゃんと25個入るかな?
//			pointList.push_back(wayPoint);
//		}
//	}
//}
//
////Flag類は外からリセット出来てほしいのでpublicにしてる
//void WayPointManager::ResetFlag()
//{
//	wayPoint.useFlag = false;
//	wayPoint.arrivalFlag = false;
//	wayPoint.impossibleFlag = false;
//	wayPoint.closeFlag = false;
//}
//
////本当に一度だけ初期化したいものをここに書く。
//void WayPointManager::Initialize()
//{
//	pointList.clear();
//
//	interval = 10;
//
//	offsetWidth = 50.0f;
//	offsetHeight = 50.0f;
//	halfWidth = offsetWidth / 2.0f;
//	halfHeight = offsetHeight / 2.0f;
//
//	finishCreatePoint = false;
//}
