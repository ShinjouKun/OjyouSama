#pragma once
#include "BaseEnemy.h"

class Timer;
class TexRenderer;
class Explosion;
class Hit;

//空を飛んでいる敵
class BirdEnemy : public BaseEnemy
{
public:

	BirdEnemy(
		const Vector3& pos,
		const Vector3& ang,
		shared_ptr<TexRenderer> texRender,
		int num
	);

	~BirdEnemy();

private:

	//BaseEnemyから継承
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*生存状態を監視*/
	void CheckAlive();

	/*攻撃*/
	void Attack();

	void Action_Rising();
	void Action_Move(const Vector3& targetPosition);
	void Action_Fire();
	void Action_Back(const Vector3& targetPosition);
	void Action_Descend();
	void Action_Reload();

	bool InsideDistanceY(const Vector3& distance, const float length) const;

private:

	enum ActionStep
	{
		RISING, //上昇
		MOVE,   //移動
		FIRE,   //弾発射
		BACK,   //戻る
		DESCEND,//下降
		RELOAD,
	};
	ActionStep mActionStep;//攻撃用状態

	shared_ptr<TexRenderer> mTexRender;//テクスチャ描画用
	shared_ptr<Timer> mIntervalTime;
	shared_ptr<Timer> mReloadTime;
	shared_ptr<Sound> mAttackSE;//攻撃時のSE
	shared_ptr<Sound> mDamageSE;     //ダメージを受けた時のSE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;
	shared_ptr<Hit> mDamageParticle;      //ダメージを受けた時のパーティクル
	//shared_ptr<Explosion> mDeathParticle; //死亡した時のパーティクル

	Vector3 mScale;             //大きさ
	Vector3 mTargetPosition;    //プレイヤーの位置
	Vector3 mOffsetPosition;    //帰る位置
	Vector3 mOffsetRisePosition;//帰る位置の真上

	const float RISE_HEIGHT = 10.0f;  //上昇量の高さ
	const float ATTACK_LENGTH = 50.0f;//攻撃範囲
	const float LEG_SPEED = 10.0f;    //脚の回転速度
	const float LEG_RANGE = 100.0f;   //脚の最大回転量
	const float WALK_SPEED = 1.0f;//通常の移動速度
	const float RUN_SPEED = 1.5f; //弾を撃った後の移動速度

	float mRadius;          //半径
	float mRiseSpeed = 0.5f;//上昇の速さ
	float mLegRotate = 0.0f;//脚の回転

	bool mFinishAnimation = false;//アニメーションが終了しているか
	bool mFireFlag;               //弾を発射したか
	bool mCreateItem;             //アイテムを生成したか？

	int mBulletNumber;//発射した弾数

	string num;       //string型の識別番号
	string mBullet;   //砲身の名前登録
	string mLeg;      //砲塔の名前登録
	string mBody;     //車体の名前登録
	string mBulletNum;//識別番号+砲身の名前
	string mLegNum;   //識別番号+砲塔の名前
	string mBodyNum;  //識別番号+車体の名前

	string EXPLOSION_EFFECT = "Explosion";//エフェクト名
};