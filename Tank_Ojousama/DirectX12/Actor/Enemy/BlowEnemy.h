#pragma once
#include "BaseEnemy.h"

//近距離攻撃の敵
class BlowEnemy : public BaseEnemy
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">初期位置</param>
	/// <param name="angle">初期角度</param>
	/// <param name="num">識別番号</param>
	/// <param name="advanceFlag">拠点進行モードか？</param>
	/// <param name="advanceDirection">拠点進行時に進む方向</param>
	BlowEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		int num,
		bool advanceFlag = false,
		const Vector3& advanceDirection = Vector3(0.0f, 0.0f, 1.0f)
	);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BlowEnemy();

private:

	//BaseEnemyから継承
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*生存状態を監視*/
	void CheckAlive();

	/*移動*/
	void Move();

	/*歩行アニメーション*/
	void MoveAnimation();

	/*攻撃*/
	void Attack();

	/*死亡時の演出*/
	void DeathAnimation();

	/*空に上昇*/
	void DeathAnimeStep_RiseSky();

	/*爆発&数秒停止*/
	void DeathAnimeStep_Explosion();

	/*オブジェクトを一度だけ生成する*/
	void CreateOneObject();

private:

	/*死亡アニメーションの遷移状況*/
	enum DeathAnimationStep
	{
		RISE_SKY, //上空に舞い上がる
		EXPLOSION,//消滅する
	};
	DeathAnimationStep mDeathStep;

	AttackArea* attackArea;  //攻撃範囲クラス
	Vector3 patrolPoint[4];

	shared_ptr<Timer> mRiseTime; //上昇時間
	shared_ptr<Timer> mDeathTime;//完全に死ぬまでの時間
	shared_ptr<Sound> mAttackSE; //攻撃時のSE
	shared_ptr<Sound> mDamageSE; //ダメージを受けた時のSE
	shared_ptr<Sound> mDeathSE;  //死亡したときのSE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;//爆発のエフェクト

	const float LEG_SPEED = 2.0f; //脚の回転速度
	const float LEG_RANGE = 15.0f;//脚の最大回転量

	float mLegRotate;//脚の現在の回転量

	int pointCount;//現在向かっている周回ポイント

	bool mDeathAnimation;//死亡アニメーションを開始するか
	bool mDeadFlag;      //死ぬ瞬間にtrueになるフラグ(保険)
	bool onTrigger;      //一度だけ呼ばれるトリガー
	bool mRotDirection;  //回転方向

	string mLeftLeg;        //左足のモデル名
	string mLLegNumber;     //左足 + 識別番号
	string mRightLeg;       //右足のモデル名
	string mRLegNumber;     //右足 + 識別番号
	string mHead;           //頭の名前
	string mHeadNumber;     //頭 + 識別番号
	string mBody;           //体の名前
	string mBodyNumber;     //体 + 識別番号
	string mHand;           //手の名前
	string mHandNumber;     //手 + 識別番号
	string EXPLOSION_EFFECT;//エフェクト名
};