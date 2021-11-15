#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Timer;

class TestBoss : public BaseObject
{
public:

	TestBoss(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		shared_ptr<ModelRenderer>modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~TestBoss();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*連続射撃*/
	void RapidFire();

	/*敵を召喚*/
	void Summon();

	/*角度をベクトルに変換*/
	Vector3 AngleToVectorY(float angle)const;

private:

	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;//狙う時間
	shared_ptr<Timer> mReloadTime;//リロード時間
	shared_ptr<Timer> mSummonTime;//召喚時間

	std::vector<Vector3> mSummonPoint;//召喚位置

	const Vector3 FIRE_POSITION = Vector3(0, 30, 0);//弾の発射位置

	const int MAX_HP = 200;       //最大体力
	const int RAPIDFIRE_COUNT = 3;//連続射撃の数
	const int SUMMON_COUNT = 5;   //召喚数
	int mBulletCount;//弾を発射した数
	int mCircleCount;//警告を表示した数
	int mEnemyNumber;//召喚した敵の数

	const float MOVE_SPEED = 0.0f; //移動速度
	const float AIMING_TIME = 1.0f;//狙う時間
	const float RELOAD_TIME = 0.5f;//装填時間
	const float SUMMON_TIME = 5.0f;//召喚準備時間
	float mFireAngle;//弾を撃つときの向き

	bool mShotNormalBullet;
	bool mSummonEnemy;    //敵を召喚したか
	bool mDrawSummonPoint;//召喚中の警告を表示
	bool mSummonFlag;     //召喚攻撃を行うときtrueにする

	string mModelNum;

	string mModelName01 = "TestBoss";
	string mModelNumName01;

	//警告用オブジェクト
	string mCircleNum;
	string mCircleName = "SummonCircle";
	string numCircle;

	std::vector<string> testNum;
	std::vector<string> testNumName;
};