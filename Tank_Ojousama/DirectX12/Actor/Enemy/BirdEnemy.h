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

	/*Šp“x‚ğƒxƒNƒgƒ‹‚É•ÏŠ·*/
	Vector3 AngleToVectorY(float angle)const;

private:

	ObjectManager * mObjManager;
	shared_ptr<ModelRenderer> mModelRender;
	shared_ptr<ParticleManager> mEffectManager;

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

	const float RISE_HEIGHT = 30.0f;//ã¸—Ê‚Ì‚‚³
	const float ATTACK_LENGTH = 50.0f;
	float mRadius;//”¼Œa
	float mFireAngle;
	float mRiseSpeed = 0.5f;

	bool mFireFlag;//’e‚ğ”­Ë‚µ‚½‚©

	int mBulletNumber;//”­Ë‚µ‚½’e”

	string mWingRight;//–Cg‚Ì–¼‘O“o˜^
	string mWingLeft; //–C“ƒ‚Ì–¼‘O“o˜^
	string mBody;     //Ô‘Ì‚Ì–¼‘O“o˜^
	string num;       //stringŒ^‚Ì¯•Ê”Ô†
	string mWingRNum; //¯•Ê”Ô†+–Cg‚Ì–¼‘O
	string mWingLNum; //¯•Ê”Ô†+–C“ƒ‚Ì–¼‘O
	string mBodyNum;  //¯•Ê”Ô†+Ô‘Ì‚Ì–¼‘O
};