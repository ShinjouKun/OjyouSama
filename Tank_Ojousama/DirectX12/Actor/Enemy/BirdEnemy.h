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

	void Move(const Vector3& targetPosition);
	void MoveY(const Vector3& targetPosition);

	void Action_Rising();
	void Action_Move(const Vector3& targetPosition);
	void Action_Fire();
	void Action_Back(const Vector3& targetPosition);
	void Action_Descend();
	void Action_Reload();

	bool InsideDistanceY(const Vector3& distance, const float length) const;

	/*Šp“x‚ğƒxƒNƒgƒ‹‚É•ÏŠ·*/
	Vector3 AngleToVectorY(float angle)const;

private:

	//ObjectManager * mObjManager;
	//shared_ptr<ModelRenderer> mModelRender;
	//shared_ptr<ParticleManager> mEffectManager;

	std::shared_ptr<Timer> mIntervalTime;
	std::shared_ptr<Timer> mReloadTime;

	Vector3 mScale;
	Vector3 mTargetPosition;//ƒvƒŒƒCƒ„[‚ÌˆÊ’u
	Vector3 mOffsetPosition;//‹A‚éˆÊ’u
	Vector3 mOffsetRisePosition;//‹A‚éˆÊ’u‚Ì^ã

	enum ActionStep
	{
		RISING, //ã¸
		MOVE,   //ˆÚ“®
		FIRE,   //’e”­Ë
		BACK,   //–ß‚é
		DESCEND,//‰º~
		RELOAD,
	};
	ActionStep mActionStep;//UŒ‚—pó‘Ô

	const float RISE_HEIGHT = 20.0f;//ã¸—Ê‚Ì‚‚³
	const float ATTACK_LENGTH = 50.0f;
	float mRadius;//”¼Œa
	float mRiseSpeed = 0.5f;//ã¸‚Ì‘¬‚³

	float mLegRotate = 0.0f;//‹r‚Ì‰ñ“]
	const float LEG_SPEED = 10.0f;//‹r‚Ì‰ñ“]‘¬“x
	const float LEG_RANGE = 100.0f;//‹r‚ÌÅ‘å‰ñ“]—Ê
	bool mFinishAnimation = false;//ƒAƒjƒ[ƒVƒ‡ƒ“‚ªI—¹‚µ‚Ä‚¢‚é‚©
	bool mFireFlag;//’e‚ğ”­Ë‚µ‚½‚©

	int mBulletNumber;//”­Ë‚µ‚½’e”

	string num;       //stringŒ^‚Ì¯•Ê”Ô†
	string mBullet;//–Cg‚Ì–¼‘O“o˜^
	string mLeg; //–C“ƒ‚Ì–¼‘O“o˜^
	string mBody;     //Ô‘Ì‚Ì–¼‘O“o˜^
	string mBulletNum; //¯•Ê”Ô†+–Cg‚Ì–¼‘O
	string mLegNum; //¯•Ê”Ô†+–C“ƒ‚Ì–¼‘O
	string mBodyNum;  //¯•Ê”Ô†+Ô‘Ì‚Ì–¼‘O
};