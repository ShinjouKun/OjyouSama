#pragma once
#include "BaseEnemy.h"

class Timer;
class Explosion;
class Hit;

//固定砲台の敵
class MortarEnemy :public BaseEnemy
{
public:

	MortarEnemy(
		const Vector3& pos,
		const Vector3& ang,
		int num
	);

	~MortarEnemy();

private:

	//BaseEnemyから継承
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

	/*生存状態を監視*/
	void CheckAlive();

	/*攻撃*/
	void Attack();

	/*死亡時の演出*/
	void DeathAnimation();

	/*空に上昇*/
	void DeathAnimeStep_RiseSky();

	/*爆発&数秒停止*/
	void DeathAnimeStep_Explosion();

private:

	/*死亡アニメーションの遷移状況*/
	enum DeathAnimationStep
	{
		RISE_SKY, //上空に舞い上がる
		EXPLOSION,//消滅する
	};
	DeathAnimationStep mDeathStep;

	/*攻撃の遷移状況*/
	enum AttackStep
	{
		AIMING,//照準合わせ中
		FIRE,  //弾発射！
		RELOAD,//リロード中
	};
	AttackStep mAttackStep;//攻撃用状態

	shared_ptr<Timer> mAimingTime;//狙う時間
	shared_ptr<Timer> mReloadTime;//リロード時間
	shared_ptr<Timer> mRiseTime;  //上昇時間
	shared_ptr<Timer> mDeathTime; //完全に死ぬまでの時間
	shared_ptr<Sound> mAttackSE;  //攻撃時のSE
	shared_ptr<Sound> mDamageSE;  //攻撃時のSE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;
	shared_ptr<Hit> mDamageParticle;     //ダメージ用パーティクル
	shared_ptr<Explosion> mDeathParticle;//死亡用パーティクル

	Vector3 mScale;         //大きさ
	Vector3 mTargetPosition;//弾を弾の落下地点

	float mRadius;    //半径
	float barrelAngle;//攻撃時の向き
	float mHandAngle; //腕の角度

	int mBulletNumber;//発射した弾数

	bool mFireFlag;//弾を発射したか
	bool mDeathAnimation;//死亡アニメーションを開始するか
	bool mDeadFlag;//死ぬ瞬間にtrueになるフラグ(保険)

	string mHandRight;      //砲身の名前登録
	string mHandLeft;       //砲塔の名前登録
	string mBody;           //車体の名前登録
	string num;             //string型の識別番号
	string mHandRNum;       //識別番号+砲身の名前
	string mHandLNum;       //識別番号+砲塔の名前
	string mBodyNum;        //識別番号+車体の名前
	string EXPLOSION_EFFECT;//エフェクト名
};