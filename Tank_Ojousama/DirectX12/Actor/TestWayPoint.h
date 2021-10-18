#pragma once
#include <memory>
#include "../Math/Vector3.h"

class TestWayPoint
{
public:
	/// <summary>
	/// コンストラクト
	/// </summary>
	/// <param name="position">初期位置</param>
	TestWayPoint(const Vector3 & pos = Vector3().zero);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TestWayPoint();

	/*全てのフラグを初期化*/
	void FlagInit();

	/*位置取得*/
	Vector3 GetPosition();

	/*すでに検索で使用したか?*/
	bool GetUseFlag();

	/*障害物があるか？*/
	bool GetImpossibleFlag();

	/*到着したか?*/
	bool GetArrivalFlag();

	/*進行不能か？*/
	bool GetCloseFlag();

	/*検索したか？の値を変更*/
	void SetUseFlag(const bool value);

	/*進行不能か？の値を変更*/
	void SetCloseFlag(const bool value);

	/*障害物があるか？の値を変更*/
	void SetImpossibleFlag(const bool value);

private:

	std::shared_ptr<Vector3> mPosition;//位置

	bool useFlag;       //検索済みか？
	bool arrivalFlag;	//到着したか？(使わない)
	bool impossibleFlag;//障害物があるか？(使わないかも)
	bool closeFlag;		//進行不能か？
};