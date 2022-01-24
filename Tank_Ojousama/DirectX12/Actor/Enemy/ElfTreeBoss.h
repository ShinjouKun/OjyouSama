#pragma once
#include "BaseEnemy.h"

class Timer;
class TreeRoot;
class Sound;
class SummonEnemy;
class Explosion;
class Hit;

class ElfTreeBoss : public BaseEnemy
{
public:

	ElfTreeBoss(
		const Vector3 & position,
		const Vector3 & angle,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~ElfTreeBoss();

	/*カメラのアニメーションが終了したかを受け取る*/
	void EndCameraAnimation(bool value);

	/*ボスが死んだかどうかを確認*/
	bool GetDeadFlag();

private:

	//BaseEnemyから継承
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*攻撃パターンの決定*/
	void ChangeAttackState();

	/*連続射撃*/
	void RapidFire();

	/*敵を召喚*/
	void Summon();

	/*大量リンゴ落とし*/
	void ManyAppleDrop();

	/*死亡時の演出*/
	void DeathAnimation();

	/*怒り時の成長*/
	void AngryGrowth();

	/*回りながら爆発する*/
	void DeathAnimation_Explosion();

	/*時間になったら死亡する*/
	void DeathAnimation_DeathCount();

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

	//死亡アニメーションの状態遷移
	enum DeathAnimationStep
	{
		EXPLOSION,
		DEATH_COUNT,
	};
	DeathAnimationStep mDeathStep;


	shared_ptr<ParticleEmitterBox> mParticleEmitter;
	shared_ptr<Sound> mSmallExplosion;   //小爆発
	shared_ptr<Sound> mBigExplosion;     //大爆発
	shared_ptr<Sound> mAttackSE;         //攻撃時のSE
	shared_ptr<Sound> mDamageSE;         //ダメージを受けた時のSE
	shared_ptr<Sound> mNoDeathSE;        //無敵のときのSE
	shared_ptr<Sound> mDeathSE;          //死亡したときのSE
	shared_ptr<Hit> mDamageParticle;     //ダメージ用パーティクル
	shared_ptr<Explosion> mDeathParticle;//死亡用パーティクル

	shared_ptr<Timer> mAimingTime;    //狙う時間
	shared_ptr<Timer> mReloadTime;    //リロード時間
	shared_ptr<Timer> mSummonTime;    //召喚時間
	shared_ptr<Timer> mRootWaitTime;  //根っこ待機時間
	shared_ptr<Timer> mEffectInterval;//エフェクトの描画間隔
	shared_ptr<Timer> mExplosionTime; //エフェクトの描画時間
	shared_ptr<Timer> mAppleDropTime; //リンゴを落とし続ける時間
	shared_ptr<Timer> mAppleInterval; //リンゴを落とす感覚
	shared_ptr<Timer> mColorTimer;    //無敵時にシールドが浮き出る時間

	TreeRoot * mTreeRoot;//木の根
	TreeRoot * mTreeRoot2;//木の根
	TreeRoot * mTreeRoot3;//木の根
	TreeRoot * mTreeRoot4;//木の根
	TreeRoot * mTreeRoot5;//木の根

	std::vector<SummonEnemy*> mSummonList;//召喚した敵のリスト
	std::vector<Vector3> mSummonPoint;    //召喚位置
	std::vector<string> mSummonName;      //召喚するモデルの名前
	std::vector<string> mSummonNum;       //召喚するモデルの番号

	const Vector3 FIRE_POSITION = Vector3(0, 15, 0);//弾の発射位置
	Vector3 mScale;
	Vector3 mPlayerPosition; //プレイヤーの位置
	Vector3 mOffsetRightHand;//右手の固定位置
	Vector3 mOffsetLeftHand; //左手の固定位置
	Vector3 mOffsetRootPos;  //根っこの固定位置
	Vector3 mRightHandPos;   //右手の位置
	Vector3 mLeftHandPos;    //左手の位置
	Vector3 mRootPosition;   //根っこの位置

	const int MAX_HP = 50;       //最大体力
	const int RAPIDFIRE_COUNT = 5;//連続射撃の数
	const int SUMMON_COUNT = 5;   //召喚数
	const int DEFENSE = 10;        //防御力
	int damagePool;  //受けるダメージの一時保存
	int mBulletCount;//弾を発射した数
	int mCircleCount;//警告を表示した数
	int mEnemyNumber;//召喚した敵の数
	int mActionCount;//行動の制御

	const float SEARCH_LENGTH = 150.0f;//索敵距離
	const float MOVE_SPEED = 0.0f;    //移動速度
	const float AIMING_TIME = 0.5f;   //狙う時間
	const float RELOAD_TIME = 0.5f;   //装填時間
	const float SUMMON_TIME = 3.0f;   //召喚準備時間

	float mFireAngle;   //弾を撃つときの向き
	float mRootSpeed;   //根っこの移動速度
	float mSummonRotate;//召喚中の魔法陣の回転
	float mHandAngle;

	bool mShotBullet;        //弾発射の状態管理(狙い→発射)
	bool mSummonEnemy;       //敵を召喚したか
	bool mDrawSummonPoint;   //召喚中の警告を表示
	bool mCreateObject;      //オブジェクトを生成したか
	bool mFinishHandReturn;  //両手が元の位置に戻ったか？
	bool mFinishRootReturn;  //根っこが元の位置に戻ったか？
	bool mActionFlag;        //攻撃アクション中かどうか
	bool mDeathAnimationFlag;//死亡アニメーションを開始するか
	bool mDeadFlag;          //死ぬ前にtrueになるフラグ
	bool mOneShotSound;      //死亡SEを鳴らしたかどうか
	bool mSummonAlive;       //召喚した敵が生きているか
	bool mEndAnimation;      //シーンの導入アニメーションが終わっているか
	bool mAngryFlag;         //怒り状態かどうか
	bool mAngryAnimFlag[3];  //アニメーションが終了しているか
	bool mColorChange;       //無敵中の色に変えているかどうか

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
	//エフェクト2
	string EXPLOSION_EFFECT = "Explosion";

	//バリア
	string mBarrier = "Barrier";
	string mBarrierNum;
};