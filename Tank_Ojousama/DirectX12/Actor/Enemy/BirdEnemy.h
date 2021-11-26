#pragma once
#include "../BaseObject.h"
#include"../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

class Timer;

class BirdEnemy : public BaseObject
{
public:

	BirdEnemy(
		const Vector3& pos,
		const Vector3& ang,
		ObjectManager * objectManager,
		shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~BirdEnemy();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

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

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

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

	const float RISE_HEIGHT = 30.0f;//上昇量の高さ
	const float ATTACK_LENGTH = 50.0f;
	float mRadius;//半径
	float mFireAngle;
	float mRiseSpeed = 0.5f;

	bool mFireFlag;//弾を発射したか

	int mBulletNumber;//発射した弾数

	string mWingRight;//砲身の名前登録
	string mWingLeft; //砲塔の名前登録
	string mBody;     //車体の名前登録
	string num;       //string型の識別番号
	string mWingRNum; //識別番号+砲身の名前
	string mWingLNum; //識別番号+砲塔の名前
	string mBodyNum;  //識別番号+車体の名前
};