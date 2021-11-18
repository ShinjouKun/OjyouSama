#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Timer;
class TreeRoot;

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

	/*攻撃パターンの決定*/
	void ChangeAttackState();

	/*連続射撃*/
	void RapidFire();

	/*敵を召喚*/
	void Summon();

	/*死亡時の演出*/
	void DeathAnimation();

#pragma region 根っこ攻撃まとめ

	/*根っこ攻撃*/
	void RootAttack();

	/*地面に手をツッコむ*/
	void RootAtack_PierceHand();

	/*根っこがプレイヤーを追跡する*/
	void RootAtack_ChasePlayer();

	/*ちょっと停止*/
	void RootAtack_Wait();

	/*根っこを地面から出す*/
	void RootAtack_GoupRoot();

	/*根っこを地面に戻す*/
	void RootAtack_GodownRoot();

#pragma endregion

	/*一度だけオブジェクトを生成*/
	void CreateObject();

	/*角度をベクトルに変換*/
	Vector3 AngleToVectorY(float angle)const;

private:

	//根っこ攻撃の状態遷移
	enum RootAttackStep
	{
		PIERCE_HAND, //地面に手をツッコむ
		CHASE_PLAYER,//プレイヤーを追跡
		WAIT,        //停止
		GOUP_ROOT,   //根っこを出して攻撃
		GODOWN_ROOT, //根っこを引っ込める
	};
	RootAttackStep mRootStep;

	enum DeathAnimationStep
	{
		EXPLOSION,
		DEATH_COUNT,
	};
	DeathAnimationStep mDeathStep;

	ObjectManager* mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	shared_ptr<Timer> mAimingTime;  //狙う時間
	shared_ptr<Timer> mReloadTime;  //リロード時間
	shared_ptr<Timer> mSummonTime;  //召喚時間
	shared_ptr<Timer> mRootWaitTime;//根っこ待機時間
	shared_ptr<Timer> mEffectInterval;//エフェクトの描画間隔
	shared_ptr<Timer> mExplosionTime;//エフェクトの描画時間

	TreeRoot * mTreeRoot;//木の根

	std::vector<Vector3> mSummonPoint;//召喚位置
	std::vector<string> testNum;
	std::vector<string> testNumName;

	const Vector3 FIRE_POSITION = Vector3(0, 50, 0);//弾の発射位置
	Vector3 mPlayerPosition; //プレイヤーの位置
	Vector3 mOffsetRightHand;//右手の固定位置
	Vector3 mOffsetLeftHand; //左手の固定位置
	Vector3 mOffsetRootPos;  //根っこの固定位置
	Vector3 mRightHandPos;   //右手の位置
	Vector3 mLeftHandPos;    //左手の位置
	Vector3 mRootPosition;   //根っこの位置

	const int MAX_HP = 200;       //最大体力
	const int RAPIDFIRE_COUNT = 3;//連続射撃の数
	const int SUMMON_COUNT = 5;   //召喚数
	int mBulletCount;//弾を発射した数
	int mCircleCount;//警告を表示した数
	int mEnemyNumber;//召喚した敵の数
	int mActionCount;//行動の制御

	const float SEARCH_LENGTH = 150.0f;//索敵距離
	const float MOVE_SPEED = 0.0f;    //移動速度
	const float AIMING_TIME = 1.0f;   //狙う時間
	const float RELOAD_TIME = 0.5f;   //装填時間
	const float SUMMON_TIME = 5.0f;   //召喚準備時間
	float mFireAngle;//弾を撃つときの向き
	float mRootSpeed;//根っこの移動速度
	float mSummonRotate;//召喚中の魔法陣の回転
	float mHandAngle;

	bool mShotBullet;      //弾発射の状態管理(狙い→発射)
	bool mSummonEnemy;     //敵を召喚したか
	bool mDrawSummonPoint; //召喚中の警告を表示
	bool mCreateObject;    //オブジェクトを生成したか
	bool mFinishHandReturn;//両手が元の位置に戻ったか？
	bool mFinishRootReturn;//根っこが元の位置に戻ったか？

	bool mActionFlag;//攻撃アクション中かどうか
	bool mDeathAnimationFlag;//死亡アニメーションを開始するか

	//胴体
	string mStringNum;
	string mModelName01 = "TestBoss";
	string mModelNumName01;
	//右手
	string mHandRight = "RightHand";
	string mHandRightNum;
	//左手
	string mHandLeft = "LeftHand";
	string mHandLeftNum;
	//警告用オブジェクト
	string mSummonCircle = "SummonCircle";//召喚用
	string mRootCircle = "RootCircle";    //根っこ攻撃用
	string mRootCircleNum;
	//エフェクト
	string PARTICLE_EFFECT = "DeathBomm";
};