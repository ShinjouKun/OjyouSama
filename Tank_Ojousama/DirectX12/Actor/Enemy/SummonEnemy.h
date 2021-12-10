#pragma once
#include "../BaseObject.h"
#include "../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

class Timer;
class Sound;

//ボスに召喚される雑魚敵
class SummonEnemy : public BaseObject
{
public:

	SummonEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		std::shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~SummonEnemy();

private:

	/*親クラスから継承*/
	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	/*生存状態を監視*/
	void CheckAlive();

	/*移動*/
	void Move();

	/*歩行アニメーション*/
	void MoveAnimation();

	/*倒れこみ攻撃*/
	void DownAttack();

	/*倒れこみ*/
	void AttackStep_FallDown();
	/*数秒停止*/
	void AttackStep_Wait();
	/*起き上がり*/
	void AttackStep_GetUp();

	/*死亡時の演出*/
	void DeathAnimation();

	/*空に上昇*/
	void DeathAnimeStep_RiseSky();
	/*爆発&数秒停止*/
	void DeathAnimeStep_Explosion();

	/*指定距離以内かどうかを調べる*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

private:

	/*攻撃アニメーションの遷移状況*/
	enum AttackStep
	{
		FALL_DOWN,//倒れこむ
		WAIT,     //倒れたまま停止
		GET_UP,   //起き上がる
	};
	AttackStep mAttackStep;

	/*死亡アニメーションの遷移状況*/
	enum DeathAnimationStep
	{
		RISE_SKY, //上空に舞い上がる
		EXPLOSION,//消滅する
	};
	DeathAnimationStep mDeathStep;

	ObjectManager * mObjManager;

	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;
	shared_ptr<Timer> mGetupTimer;//倒れこんでいる時間
	shared_ptr<Timer> mRiseTime;  //上昇時間
	shared_ptr<Timer> mDeathTime; //完全に死ぬまでの時間
	shared_ptr<Sound> mAttackSE;  //攻撃時のSE
	shared_ptr<Sound> mDamageSE;  //ダメージを受けた時のSE
	shared_ptr<Sound> mDeathSE;   //死亡したときのSE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;//爆発のエフェクト

	Vector3 mScale;           //大きさ
	Vector3 mPlayerPosition;  //プレイヤーの位置
	Vector3 mPreviousPosition;//前フレームの位置

	const float SEARCH_LENGTH = 50.0f;//この距離以内になったら追跡する
	const float FIND_LRNGTH = 20.0f;  //この距離以内になったら突撃する
	const float ATTACK_LENGTH = 5.0f; //この距離以内になったら攻撃する
	float mFireAngle = 0.0f;//攻撃時の向き
	float mDownSpeed = 3.0f;//倒れこむアニメーションの速度

	bool mWithinPlayerFlag;//範囲内にプレイヤーがいるか？
	bool mAttackFlag;      //攻撃中か
	bool mStep;            //true = 右, false = 左
	bool mDeathAnimation;  //死亡アニメーションを開始するか
	bool mDeadFlag;        //死ぬ瞬間にtrueになるフラグ(保険)

	string mMyNumber;       //識別番号
	string mSummon;         //モデル名
	string mSummonNum;      //モデル名 + 識別番号
	string EXPLOSION_EFFECT;//エフェクト名
};