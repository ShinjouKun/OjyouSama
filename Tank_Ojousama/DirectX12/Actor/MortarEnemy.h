#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"

class Timer;

//固定砲台の敵
class MortarEnemy :public BaseObject
{
public:

	MortarEnemy(
		const Vector3& pos,
		const Vector3& ang,
		ObjectManager * objectManager,
		shared_ptr<ModelRenderer> modelRender,
		shared_ptr<ParticleManager> effectManager,
		int num
	);

	~MortarEnemy();

private:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Rend() override;
	virtual void ImGuiDebug() override;
	virtual void OnCollison(BaseCollider * col) override;

	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

	/*角度をベクトルに変換*/
	Vector3 AngleToVectorY(float angle)const;

private:
	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;//狙う時間
	shared_ptr<Timer> mReloadTime;//リロード時間

	Vector3 mScale;
	Vector3 mTargetPosition;

	enum AttackStep
	{
		AIMING,//照準合わせ中
		FIRE,  //弾発射！
		RELOAD,//リロード中
	};
	AttackStep mAttackStep;//攻撃用状態

	float mRadius;
	float barrelAngle;
	float turretAngle;

	int mBulletNumber;//発射した弾数

	string tankBarrel;//砲身の名前登録
	string tankTurret;//砲塔の名前登録
	string tankBody;  //車体の名前登録
	string num;       //string型の識別番号
	string numBarrel; //識別番号+砲身の名前
	string numTurret; //識別番号+砲塔の名前
	string numBody;	  //識別番号+車体の名前
};