#pragma once
#include <memory>
#include "../Math/Vector3.h"

class TestBreadCrumb
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">初期位置</param>
	TestBreadCrumb(const Vector3& position, int breadNumber);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TestBreadCrumb();

	/*時間が経ったら消えるようにする*/
	void DeathInterval();

	/*位置取得*/
	Vector3 GetPosition() const;

	/*識別番号取得*/
	int GetBreadNumber() const;

	/*生存情報取得*/
	bool GetDeathFlag() const;

private:

	/*変数の初期化*/
	void Initialize();

private:

	Vector3 mPosition;

	int mBreadNumber;//識別番号
	int lifeTime;	 //生存時間
	int lifeCount;	 //デスカウントGX

	bool mDeathFlag;
};