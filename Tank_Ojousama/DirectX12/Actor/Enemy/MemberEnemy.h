#pragma once
#include "../BaseObject.h"
#include"../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

class Timer;
class Sound;

//編隊の小隊の敵
class MemberEnemy : public BaseObject
{
public:

	MemberEnemy(
		const Vector3 & position,
		const Vector3 & angle,
		ObjectManager * objManager,
		std::shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~MemberEnemy();

	/*死亡したかどうか*/
	bool GetDeadFlag() const;

	/*目標位置を隊長から受け取る*/
	void ReceivePosition(const Vector3& targetPosition);

	/*攻撃対象の位置と、攻撃指令を隊長から受け取る*/
	void ReceiveAttackCommand(const Vector3& attackTarget, const bool attackCommand);

	/*索敵対象の位置を隊長から受け取る*/
	void ReceiveSearchCommand(const Vector3& searchTarget, const bool searchCommand);

	/*隊長の死を受け取る*/
	void ReceiveCaptainLost(const bool captainLost);

	/*索敵結果を隊長に知らせる*/
	bool SendSearchResult() const;

	/*索敵結果の位置を隊長に知らせる*/
	Vector3 SendSearchPosition() const;

private:

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

	/*攻撃*/
	void Attack();

	/*隊長を失って混乱*/
	void Confusion();

	/*目標位置に移動&近かったら停止*/
	void MoveTarget(const Vector3& targetPosition, const float distance);

	/*指定距離以内かどうかを調べる*/
	bool WithinDistance(const Vector3& targetPosition, const float distance);

	/*角度をベクトルに変換*/
	Vector3 AngleToVectorY(float angle)const;

	/*死亡時の演出*/
	void DeathAnimation();

	/*空に上昇*/
	void DeathAnimeStep_RiseSky();

	/*爆発&数秒停止*/
	void DeathAnimeStep_Explosion();


#pragma region 攻撃のステップ(別のいい方法がありそう)

	void AttackStep_NONE();
	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

#pragma endregion

private:

	/*攻撃の状態遷移*/
	enum AttackStep
	{
		NONE,  //攻撃してない
		AIMING,//照準合わせ中
		FIRE,  //弾発射！
		RELOAD,//リロード中
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

	shared_ptr<Timer> mAimingTime;     //狙う時間
	shared_ptr<Timer> mReloadTime;     //リロード時間
	shared_ptr<Timer> mRandomMoveTimer;//ちりじりになる
	shared_ptr<Timer> mRiseTime;       //上昇時間
	shared_ptr<Timer> mDeathTime;      //完全に死ぬまでの時間
	shared_ptr<Sound> mAttackSE;       //攻撃時のSE
	shared_ptr<Sound> mDamageSE;       //攻撃時のSE
	shared_ptr<Sound> mDeathSE;        //死亡したときのSE
	shared_ptr<ParticleEmitterBox> mParticleEmitter;

	Vector3 scale;
	Vector3 mFixedPosition; //固定位置
	Vector3 mAttackTarget;  //攻撃位置
	Vector3 mSearchTarget;  //索敵位置
	Vector3 mSearchPosition;//索敵結果位置
	Vector3 mRandomDirection;//ランダム移動方向
	Vector3 mPreviousPosition;


	const float SEARCH_RANGE = 30.0f;//索敵範囲(円)
	const float LEG_SPEED = 1.5f; //脚の回転速度
	const float LEG_RANGE = 15.0f;//脚の最大回転量

	float mRadius;   //半径
	float mFireAngle;//攻撃時の向き
	float mLegRotate;//脚の現在の回転量

	int mBulletNumber = 0;//発射した弾数
	int mMoveDirection = 0;//ランダム移動時の移動方向

	bool mAttackCommand; //攻撃指令
	bool mSearchCommand; //索敵指令
	bool mSearchResult;  //プレイヤーを発見したか
	bool mCaptainLost;   //隊長を失ったかどうか
	bool mRotDirection;  //回転方向
	bool mDeathAnimation;//死亡アニメーションを開始するか
	bool mDeadFlag;      //死ぬ瞬間にtrueになるフラグ(保険)

	string mMyNumber;       //識別番号
	string mLeftLeg;        //左足のモデル名
	string mLLegNumber;     //左足 + 識別番号
	string mRightLeg;       //右足のモデル名
	string mRLegNumber;     //右足 + 識別番号
	string mHead;           //頭の名前
	string mHeadNumber;     //頭 + 識別番号
	string mBody;           //体の名前
	string mBodyNumber;     //体 + 識別番号
	string EXPLOSION_EFFECT;//エフェクト名
};
