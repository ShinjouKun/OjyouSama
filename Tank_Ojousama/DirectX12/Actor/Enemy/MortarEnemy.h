#pragma once
#include "../BaseObject.h"
#include"../ObjectManager.h"
#include "../../Render/ModelRenderer.h"
#include "../../Render/ParticleManager.h"

#include "BaseEnemy.h"

class Timer;

//ŒÅ’è–C‘ä‚Ì“G
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

	//virtual void Init() override;
	//virtual void Update() override;
	//virtual void Rend() override;
	//virtual void ImGuiDebug() override;
	//virtual void OnCollison(BaseCollider * col) override;

	//BaseEnemy‚©‚çŒp³
	virtual void EnemyInit() override;
	virtual void EnemyUpdate() override;
	virtual void EnemyRend() override;
	virtual void EnemyOnCollision(BaseCollider* col) override;
	virtual void EnemyImGuiDebug() override;

	/*õ“Gó‘Ô*/
	virtual void Search() override;

	/*’ÇÕó‘Ô*/
	virtual void Warning() override;

	/*UŒ‚ó‘Ô*/
	virtual void Attack() override;



	void AttackStep_AIMING();
	void AttackStep_FIRE();
	void AttackStep_RELOAD();

	/*Šp“x‚ğƒxƒNƒgƒ‹‚É•ÏŠ·*/
	Vector3 AngleToVectorY(float angle)const;

private:
	//ObjectManager * mObjManager;
	//shared_ptr<ModelRenderer> mModelRender;
	//shared_ptr<ParticleManager> mEffectManager;

	shared_ptr<Timer> mAimingTime;//‘_‚¤ŠÔ
	shared_ptr<Timer> mReloadTime;//ƒŠƒ[ƒhŠÔ

	Vector3 mScale;
	Vector3 mTargetPosition;

	enum AttackStep
	{
		AIMING,//Æ€‡‚í‚¹’†
		FIRE,  //’e”­ËI
		RELOAD,//ƒŠƒ[ƒh’†
	};
	AttackStep mAttackStep;//UŒ‚—pó‘Ô

	float mRadius;
	float barrelAngle;
	float mHandAngle;//˜r‚ÌŠp“x

	bool mFireFlag;//’e‚ğ”­Ë‚µ‚½‚©

	int mBulletNumber;//”­Ë‚µ‚½’e”

	string mHandRight;//–Cg‚Ì–¼‘O“o˜^
	string mHandLeft;//–C“ƒ‚Ì–¼‘O“o˜^
	string mBody;  //Ô‘Ì‚Ì–¼‘O“o˜^
	string num;       //stringŒ^‚Ì¯•Ê”Ô†
	string mHandRNum; //¯•Ê”Ô†+–Cg‚Ì–¼‘O
	string mHandLNum; //¯•Ê”Ô†+–C“ƒ‚Ì–¼‘O
	string mBodyNum;	  //¯•Ê”Ô†+Ô‘Ì‚Ì–¼‘O
};