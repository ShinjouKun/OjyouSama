#pragma once
#include "../BaseObject.h"
#include"../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

#include "BaseEnemy.h"

class Timer;

class BirdEnemy : public BaseEnemy
{
public:

	BirdEnemy(
		const Vector3& pos,
		const Vector3& ang,
		int num
	);

	~BirdEnemy();

private:

	//virtual void Init() override;
	//virtual void Update() override;
	//virtual void Rend() override;
	//virtual void ImGuiDebug() override;
	//virtual void OnCollison(BaseCollider * col) override;

	//BaseEnemyから継承
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*索敵状態*/
	virtual void Search() override;

	/*追跡状態*/
	virtual void Warning() override;

	/*攻撃状態*/
	virtual void Attack() override;

	void Move(const Vector3& targetPosition);
	void MoveY(const Vector3& targetPosition);

	void Action_Rising();
	void Action_Move(const Vector3& targetPosition);
	void Action_Fire();
	void Action_Back(const Vector3& targetPosition);
	void Action_Descend();
	void Action_Reload();

	bool InsideDistance(const Vector3& distance, const float length) const;

	/*角度をベクトルに変換*/
	Vector3 AngleToVectorY(float angle)const;

private:

	//ObjectManager * mObjManager;
	//shared_ptr<ModelRenderer> mModelRender;
	//shared_ptr<ParticleManager> mEffectManager;

	std::shared_ptr<Timer> mIntervalTime;
	std::shared_ptr<Timer> mReloadTime;

	Vector3 mScale;
	Vector3 mTargetPosition;//プレイヤーの位置
	Vector3 mOffsetPosition;//帰る位置
	Vector3 mOffsetRisePosition;//帰る位置の真上

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

	const float RISE_HEIGHT = 15.0f;//上昇量の高さ
	const float ATTACK_LENGTH = 50.0f;
	float mRadius;//半径
	float mRiseSpeed = 0.5f;//上昇の速さ

	float mLegRotate = 0.0f;//脚の回転
	const float LEG_SPEED = 10.0f;//脚の回転速度
	const float LEG_RANGE = 100.0f;//脚の最大回転量
	bool mFinishAnimation = false;//アニメーションが終了しているか
	bool mFireFlag;//弾を発射したか

	int mBulletNumber;//発射した弾数

	string num;       //string型の識別番号
	string mBullet;//砲身の名前登録
	string mLeg; //砲塔の名前登録
	string mBody;     //車体の名前登録
	string mBulletNum; //識別番号+砲身の名前
	string mLegNum; //識別番号+砲塔の名前
	string mBodyNum;  //識別番号+車体の名前
};